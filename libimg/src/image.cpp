#include <algorithm>
#include <chrono>
#include <cstdint>
#include <execution>
#include <list>
#include <memory>
#include <random>
#include <ranges>
#include <unordered_map>

#include "image.hpp"
#include "img_assert.hpp"
#include "pixel.hpp"

#include "stb_image.h"
#include "stb_image_write.h"

namespace img {

    namespace {
        std::unordered_map<std::string, img::Image::ImageFmt> imageTypeMap{
            {".JPEG", img::Image::IF_JPEG},
            { ".JPG",  img::Image::IF_JPG},
            { ".PNG",  img::Image::IF_PNG},
            { ".BMP",  img::Image::IF_BMP},
            { ".PSD",  img::Image::IF_PSD},
            { ".TGA",  img::Image::IF_TGA},
            { ".GIF",  img::Image::IF_GIF},
            { ".HDR",  img::Image::IF_HDR},
            { ".PIC",  img::Image::IF_PIC},
            { ".PNM",  img::Image::IF_PNM}
        };

        std::unordered_map<img::Image::PixelFmt, std::string> PixelFormatMap{
            { img::Image::PF_GREY8,  "GREY8"},
            {img::Image::PF_GREYa8, "GREYa8"},
            {  img::Image::PF_RGB8,   "RGB8"},
            { img::Image::PF_RGBa8,  "RGBa8"},
            {  img::Image::PF_BGR8,   "BGR8"},
            { img::Image::PF_BGRa8,  "BGRa8"},
        };

        enum ChannelCountMask : u16 {
            M_1C = Image::PF_GREY8,
            M_2C = Image::PF_GREY8 | Image::PF_GREYa8,
            M_3C = Image::PF_RGB8 | Image::PF_BGR8,
            M_4C = Image::PF_RGBa8 | Image::PF_BGRa8,
        };

        inline bool is1Channel(Image::PixelFmt pf) {
            return static_cast<u16>(pf) & M_1C;
        }

        inline bool is2Channel(Image::PixelFmt pf) {
            return static_cast<u16>(pf) & M_2C;
        }

        inline bool is3Channel(Image::PixelFmt pf) {
            return static_cast<u16>(pf) & M_3C;
        }

        inline bool is4Channel(Image::PixelFmt pf) {
            return static_cast<u16>(pf) & M_4C;
        }

        inline u32 channelCountFromPixelFormat(Image::PixelFmt pf) {
            if (static_cast<u16>(pf) & M_1C)
                return 1;
            else if (static_cast<u16>(pf) & M_2C)
                return 2;
            else if (static_cast<u16>(pf) & M_3C)
                return 3;
            else if (static_cast<u16>(pf) & M_4C)
                return 4;
            else
                IMG_ABORT("something went wrong! can't get channelCountFromPixelFormat!, %u", pf);
        }

        inline bool isGreyScale(Image::PixelFmt pf) {
            return pf & (Image::PF_GREY8 | Image::PF_GREYa8);
        }

        inline bool isRGB(Image::PixelFmt pf) {
            return pf & Image::PF_RGB8;
        }

        inline bool isRGBa(Image::PixelFmt pf) {
            return pf & Image::PF_RGBa8;
        }

    } // namespace

    Image::Image(uint32_t width, uint32_t height, PixelFmt pf)
        : m_pixelFormat(pf),
          m_width(width),
          m_height(height),
          m_channelCount(channelCountFromPixelFormat(m_pixelFormat)),
          m_pixelCount(width * height),
          m_d({nullptr}) {
        initPixels();
    }

    Image::Image()
        : m_pixelFormat(PF_UNKOWN),
          m_width(0),
          m_height(0),
          m_channelCount(0),
          m_pixelCount(0),
          m_d({nullptr}) {
    }

    Image::Image(const fs::path& filePath) : m_pixelFormat(PF_UNKOWN), m_d({nullptr}) {
        if (!fs::exists(filePath)) {
            IMG_ABORT("file does not exist: %s", filePath.c_str());
        }

        u8* d;
        int w, h, c;
        if ((d = stbi_load(filePath.c_str(), &w, &h, &c, 0))) {
            m_width        = static_cast<u32>(w);
            m_height       = static_cast<u32>(h);
            m_channelCount = static_cast<u32>(c);

            IMG_DEBUG_ASSERT((m_width * m_height) <= LIB_IMG_MAX_SIZE,
                             "Image %s pixel count exceeded `LIB_IMG_MAX_SIZE`: %u, image pixel count: %d"
                             "are you sure this file is valid?",
                             filePath.c_str(),
                             LIB_IMG_MAX_SIZE,
                             m_width * m_height);

            m_pixelCount = m_width * m_height;

            initPixels();
            std::copy(d, d + (m_pixelCount * m_channelCount), m_d.data_1B);
            stbi_image_free(d);

            // clang-format off
            switch (m_channelCount) {
                case 1:  m_pixelFormat = PF_GREY8;  break;
                case 2:  m_pixelFormat = PF_GREYa8; break;
                case 3:  m_pixelFormat = PF_RGB8;   break;
                case 4:  m_pixelFormat = PF_RGBa8;  break;
                default: IMG_ABORT("got image with %d channels, something must be wrong!", m_channelCount); break;
            }
            // clang-format on

        } else {
            IMG_ABORT("stb couldn't load image file: %s", filePath.c_str());
        }
    }

    Image::Image(const Image& other)
        : m_pixelFormat(other.m_pixelFormat),
          m_width(other.m_width),
          m_height(other.m_height),
          m_channelCount(other.m_channelCount),
          m_pixelCount(m_width * m_height),
          m_d({nullptr}) {
        initPixels();
        copyPixelsFromOtherImage(other);
    }

    Image::Image(Image&& other)
        : m_pixelFormat(std::move(other.m_pixelFormat)),
          m_width(std::move(other.m_width)),
          m_height(std::move(other.m_height)),
          m_channelCount(std::move(other.m_channelCount)),
          m_pixelCount(std::move(other.m_pixelCount)),
          m_d(std::move(other.m_d)) {
        other.m_d = {nullptr};
    }

    Image::~Image() {
        if (m_d.data_1B) {
            delete[] m_d.data_1B;
        }
    }

    Image Image::GaussianRandomNoise(uint32_t width, uint32_t height, PixelFmt pf, float mean, float dev) {
        Image img(width, height, pf);

        auto gen     = std::bind(std::normal_distribution<float>{mean, dev}, std::mt19937(std::random_device{}()));
        auto rand_u8 = [&gen]() { return static_cast<u8>(gen()); };

        // clang-format off
        switch (img.pixelFormat()) {
            case PF_GREY8:  std::for_each(img.m_d.g8,    img.m_d.g8    + img.pixelCount(), [&rand_u8](GREY8&  p) { p = { .g = rand_u8() }; });                                 break;
            case PF_GREYa8: std::for_each(img.m_d.ga8,   img.m_d.ga8   + img.pixelCount(), [&rand_u8](GREYa8& p) { p = { .g = rand_u8() }; });                                 break;
            case PF_RGB8:   std::for_each(img.m_d.rgb8,  img.m_d.rgb8  + img.pixelCount(), [&rand_u8](RGB8&   p) { p = { .r = rand_u8(), .g = rand_u8(), .b = rand_u8() }; }); break;
            case PF_RGBa8:  std::for_each(img.m_d.rgba8, img.m_d.rgba8 + img.pixelCount(), [&rand_u8](RGBa8&  p) { p = { .r = rand_u8(), .g = rand_u8(), .b = rand_u8() }; }); break;
            case PF_BGR8:   std::for_each(img.m_d.bgr8,  img.m_d.bgr8  + img.pixelCount(), [&rand_u8](BGR8&   p) { p = { .b = rand_u8(), .g = rand_u8(), .r = rand_u8() }; }); break;
            case PF_BGRa8:  std::for_each(img.m_d.bgra8, img.m_d.bgra8 + img.pixelCount(), [&rand_u8](BGRa8&  p) { p = { .b = rand_u8(), .g = rand_u8(), .r = rand_u8() }; }); break;
            default:        IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[img.pixelFormat()].c_str());                                                      break;
        }
        // clang-format on

        return img;
    }

    Image::ImageFmt Image::getImageFormat(const fs::path& filePath) {
        if (!filePath.has_extension()) {
            IMG_ABORT("Image extention is missing: %s", filePath.c_str());
        }

        std::string ext = filePath.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::toupper(c); });
        return imageTypeMap[ext];
    }

    Image::PixelFmt Image::getPixelFormat(const fs::path& filePath) {
        IMG_ABORT("Unimplemented");
    }

    Image& Image::operator=(const Image& other) {
        if (this == &other) {
            return *this;
        }

        m_pixelFormat  = other.m_pixelFormat;
        m_width        = other.m_width;
        m_height       = other.m_height;
        m_channelCount = other.m_channelCount;
        m_pixelCount   = other.m_pixelCount;

        if (m_d.data_1B) {
            delete[] m_d.data_1B;
            m_d = {nullptr};
        }

        initPixels();
        copyPixelsFromOtherImage(other);

        return *this;
    }

    Image& Image::operator=(Image&& other) {
        if (this == &other) {
            return *this;
        }

        m_pixelFormat  = std::move(other.m_pixelFormat);
        m_width        = std::move(other.m_width);
        m_height       = std::move(other.m_height);
        m_channelCount = std::move(other.m_channelCount);
        m_pixelCount   = std::move(other.m_pixelCount);

        if (m_d.data_1B) {
            delete[] m_d.data_1B;
            m_d = {nullptr};
        }

        m_d.data_1B = std::move(other.m_d.data_1B);
        other.m_d   = {nullptr};

        return *this;
    }

    Image operator+(const Image& LHS, const Image& RHS) {
        IMG_ASSERT(LHS.pixelFormat() == RHS.pixelFormat(), "can't do `op+` on images with diff pixel format!");

        uint32_t w_max = LHS.width() > RHS.width() ? LHS.width() : RHS.width();
        uint32_t h_max = LHS.height() > RHS.height() ? LHS.height() : RHS.height();
        Image    ret{w_max, h_max, LHS.pixelFormat()};

        switch (LHS.pixelFormat()) {
        case Image::PF_RGB8:
            for (u32 y = 0; y < ret.height(); ++y) {
                for (u32 x = 0; x < ret.width(); ++x) {
                    u32 i           = (ret.width() * y) + x;
                    u32 i_rhs       = (RHS.width() * (y % RHS.height())) + (x % RHS.width());
                    u32 i_lhs       = (LHS.width() * (y % LHS.height())) + (x % LHS.width());
                    ret.m_d.rgb8[i] = LHS.m_d.rgb8[i_lhs] + RHS.m_d.rgb8[i_rhs];
                }
            }
            break;
        default:
            IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[LHS.pixelFormat()].c_str());
        }

        return ret;
    }

    Image operator-(const Image& LHS, const Image& RHS) {
        IMG_ASSERT(LHS.pixelFormat() == RHS.pixelFormat(), "can't do `op-` on images with diff pixel format!");

        u32   w_max = LHS.width() > RHS.width() ? LHS.width() : RHS.width();
        u32   h_max = LHS.height() > RHS.height() ? LHS.height() : RHS.height();
        Image ret{w_max, h_max, LHS.pixelFormat()};

        switch (LHS.pixelFormat()) {
        case Image::PF_RGB8: {
            for (u32 y = 0; y < ret.height(); ++y) {
                for (u32 x = 0; x < ret.width(); ++x) {
                    u32 i           = (ret.width() * y) + x;
                    u32 i_rhs       = (RHS.width() * (y % RHS.height())) + (x % RHS.width());
                    u32 i_lhs       = (LHS.width() * (y % LHS.height())) + (x % LHS.width());
                    ret.m_d.rgb8[i] = LHS.m_d.rgb8[i_lhs] - RHS.m_d.rgb8[i_rhs];
                }
            }
            break;
        }
        default:
            IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[LHS.pixelFormat()].c_str());
        }

        return ret;
    }

    bool Image::save(fs::path filePath, bool png_for_unsupported_format) const {
        if (!filePath.has_filename() || !filePath.has_extension()) {
            IMG_ABORT("Invalid image path: %s", filePath.c_str());
        }

        int ret;
        int w = static_cast<int>(m_width);
        int h = static_cast<int>(m_height);
        int c = static_cast<int>(m_channelCount);

        // clang-format off
        switch (Image::getImageFormat(filePath)) {
            case IF_JPG:
            case IF_JPEG: ret = stbi_write_jpg(filePath.c_str(), w, h, c, m_d.data_1B, 100);   break;
            case IF_PNG:  ret = stbi_write_png(filePath.c_str(), w, h, c, m_d.data_1B, w * c); break;
            case IF_BMP:  ret = stbi_write_bmp(filePath.c_str(), w, h, c, m_d.data_1B);        break;
            case IF_TGA:  ret = stbi_write_tga(filePath.c_str(), w, h, c, m_d.data_1B);        break;
            default:
                if (png_for_unsupported_format /* allow implicit conversion on unsupported format ??*/) {
                    IMG_LOG_WARN("saving \"%s\" extension is not supported, defaulting to \".png\"", filePath.extension().c_str());
                    filePath.replace_extension(".png");
                    ret = stbi_write_png(filePath.c_str(), w, h, c, m_d.data_1B, w * c);
                } else { 
                    IMG_ABORT("unsupported format: %s", filePath.extension().c_str()); 
                }
                break;
            }
        // clang-format on

        if (ret < 0) {
            IMG_LOG_WARN("file save returned error id: %d", ret);
            return false;
        }

        return true;
    }

    Image& Image::colorMask(float r, float g, float b) {
        IMG_ASSERT(m_pixelFormat & (PF_RGB8 | PF_RGBa8), "color mask requires image with 3 channels.");

        // clang-format off
        switch (m_pixelFormat) {
            case PF_RGB8:  std::for_each(m_d.rgb8,  m_d.rgb8  + m_pixelCount, [&r, &g, &b](RGB8&  p) { p *= arr3<float>{ r, g, b }; }); break;
            case PF_RGBa8: std::for_each(m_d.rgba8, m_d.rgba8 + m_pixelCount, [&r, &g, &b](RGBa8& p) { p *= arr3<float>{ r, g, b }; }); break;
            case PF_BGR8:  std::for_each(m_d.bgr8,  m_d.bgr8  + m_pixelCount, [&r, &g, &b](BGR8&  p) { p *= arr3<float>{ r, g, b }; }); break; 
            case PF_BGRa8: std::for_each(m_d.bgra8, m_d.bgra8 + m_pixelCount, [&r, &g, &b](BGRa8& p) { p *= arr3<float>{ r, g, b }; }); break; 
            default:       IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[m_pixelFormat].c_str());                    break;
        }
        // clang-format on

        return *this;
    }

    Image& Image::alphaMask(float a) {
        IMG_ASSERT(m_pixelFormat & (PF_GREYa8 | PF_RGBa8 | PF_BGRa8), "alpha mask requires image with 3 channels.");

        // clang-format off
        switch (m_pixelFormat) {
        case PF_GREYa8: std::for_each(m_d.ga8,   m_d.ga8   + m_pixelCount, [&a](GREYa8& p) { p.a = clamp_8b(p.a + a); }); break;
        case PF_RGBa8:  std::for_each(m_d.rgba8, m_d.rgba8 + m_pixelCount, [&a](RGBa8&  p) { p.a = clamp_8b(p.a + a); }); break;
        case PF_BGRa8:  std::for_each(m_d.bgra8, m_d.bgra8 + m_pixelCount, [&a](BGRa8&  p) { p.a = clamp_8b(p.a + a); }); break;
        default:        IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[m_pixelFormat].c_str());         break;
        }
        // clang-format on

        return *this;
    }

    Image& Image::addGaussianNoise(float mean, float dev) {
        auto gen = std::bind(std::normal_distribution<float>{mean, dev}, std::mt19937(std::random_device{}()));

        // clang-format off
        switch (m_pixelFormat) {
            case PF_GREY8:  std::for_each(m_d.g8,    m_d.g8    + m_pixelCount, [&gen] (GREY8&  p) { p.g = clamp_8b(p.g + gen());              }); break;
            case PF_GREYa8: std::for_each(m_d.ga8,   m_d.ga8   + m_pixelCount, [&gen] (GREYa8& p) { p.g = clamp_8b(p.g + gen());              }); break;
            case PF_RGB8:   std::for_each(m_d.rgb8,  m_d.rgb8  + m_pixelCount, [&gen] (RGB8&   p) { p  += arr3<float>{ gen(), gen(), gen() }; }); break;
            case PF_RGBa8:  std::for_each(m_d.rgba8, m_d.rgba8 + m_pixelCount, [&gen] (RGBa8&  p) { p  += arr3<float>{ gen(), gen(), gen() }; }); break;
            case PF_BGR8:   std::for_each(m_d.bgr8,  m_d.bgr8  + m_pixelCount, [&gen] (BGR8&   p) { p  += arr3<float>{ gen(), gen(), gen() }; }); break;
            case PF_BGRa8:  std::for_each(m_d.bgra8, m_d.bgra8 + m_pixelCount, [&gen] (BGRa8&  p) { p  += arr3<float>{ gen(), gen(), gen() }; }); break;
            default:        IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[m_pixelFormat].c_str());                             break;
        }
        // clang-format on

        return *this;
    }

    Image& Image::greyScaleAvg() {
        if (isGreyScale(m_pixelFormat)) {
            IMG_LOG_WARN("can't convert image to grey scale, pixel format %s.", PixelFormatMap[pixelFormat()].c_str());
            return *this;
        }

        union {
            u8* data_1B;

            RGB8*  rgb8;
            RGBa8* rgba8;
            BGR8*  bgr8;
            BGRa8* bgra8;
        } tmp;

        tmp.data_1B = m_d.data_1B;
        auto old_pf = m_pixelFormat;

        if (pixelFormat() & (PF_RGBa8 | PF_BGRa8)) {
            m_pixelFormat  = PF_GREYa8;
            m_channelCount = 2;
        } else {
            m_pixelFormat  = PF_GREY8;
            m_channelCount = 1;
        }

        m_d = {nullptr};
        initPixels();

        u32 i = 0;
        switch (old_pf) {
        case PF_RGB8:
            std::for_each(m_d.g8, m_d.g8 + m_pixelCount, [&tmp, &i](GREY8& p) {
                p.g = clamp_8b((tmp.rgb8[i].r + tmp.rgb8[i].g + tmp.rgb8[i].b) / 3.f);
                ++i;
            });
            break;
        case PF_RGBa8:
            std::for_each(m_d.ga8, m_d.ga8 + m_pixelCount, [&tmp, &i](GREYa8& p) {
                p.g = clamp_8b((tmp.rgba8[i].r + tmp.rgba8[i].g + tmp.rgba8[i].b) / 3.f);
                p.a = tmp.rgba8[i].a;
                ++i;
            });
            break;
        case PF_BGR8:
            std::for_each(m_d.g8, m_d.g8 + m_pixelCount, [&tmp, &i](GREY8& p) {
                p.g = clamp_8b((tmp.bgr8[i].r + tmp.bgr8[i].g + tmp.bgr8[i].b) / 3.f);
                ++i;
            });
            break;
        case PF_BGRa8:
            std::for_each(m_d.ga8, m_d.ga8 + m_pixelCount, [&tmp, &i](GREYa8& p) {
                p.g = clamp_8b((tmp.bgra8[i].r + tmp.bgra8[i].g + tmp.bgra8[i].b) / 3.f);
                p.a = tmp.bgra8[i].a;
                ++i;
            });
            break;
        default:
            IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[m_pixelFormat].c_str());
            break;
        }

        delete[] tmp.data_1B;

        return *this;
    }

    Image& Image::greyScaleLum() {
        if (isGreyScale(m_pixelFormat)) {
            IMG_LOG_WARN("can't convert image to grey scale, pixel format %s.", PixelFormatMap[pixelFormat()].c_str());
            return *this;
        }

        float rf = .2126f, gf = .7152f, bf = .0722f;

        union {
            u8* data_1B;

            struct RGB8*  rgb8;
            struct RGBa8* rgba8;
            struct BGR8*  bgr8;
            struct BGRa8* bgra8;
        } tmp;

        tmp.data_1B = m_d.data_1B;
        auto old_pf = m_pixelFormat;

        if (pixelFormat() & (PF_RGBa8 | PF_BGRa8)) {
            m_pixelFormat  = PF_GREYa8;
            m_channelCount = 2;
        } else {
            m_pixelFormat  = PF_GREY8;
            m_channelCount = 1;
        }

        m_d = {nullptr};
        initPixels();

        u32 i = 0;
        switch (old_pf) {
        case PF_RGB8:
            std::for_each(m_d.g8, m_d.g8 + m_pixelCount, [&tmp, &i, &rf, &gf, &bf](GREY8& p) {
                p.g = clamp_8b(rf * tmp.rgb8[i].r + gf * tmp.rgb8[i].g + bf * tmp.rgb8[i].b);
                ++i;
            });
            break;
        case PF_RGBa8:
            std::for_each(m_d.ga8, m_d.ga8 + m_pixelCount, [&tmp, &i, &rf, &gf, &bf](GREYa8& p) {
                p.g = clamp_8b(rf * tmp.rgba8[i].r + gf * tmp.rgba8[i].g + bf * tmp.rgba8[i].b);
                p.a = tmp.rgba8[i].a;
                ++i;
            });
            break;
        case PF_BGR8:
            std::for_each(m_d.g8, m_d.g8 + m_pixelCount, [&tmp, &i, &rf, &gf, &bf](GREY8& p) {
                p.g = clamp_8b(rf * tmp.bgr8[i].r + gf * tmp.bgr8[i].g + bf * tmp.bgr8[i].b);
                ++i;
            });
            break;
        case PF_BGRa8:
            std::for_each(m_d.ga8, m_d.ga8 + m_pixelCount, [&tmp, &i, &rf, &gf, &bf](GREYa8& p) {
                p.g = clamp_8b(rf * tmp.bgra8[i].r + gf * tmp.bgra8[i].g + bf * tmp.bgra8[i].b);
                p.a = tmp.bgra8[i].a;
                ++i;
            });
            break;
        default:
            IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[m_pixelFormat].c_str());
            break;
        }

        delete[] tmp.data_1B;

        return *this;
    }

    Image& Image::crop(u32 x1, u32 y1, u32 x2, u32 y2) {
        IMG_ASSERT((x2 > x1) && (y2 > y1) && (x1 * x2 * y1 * y2 != 0),
                   "`(x2 > x1 > 0) && (y2 > y1 > 0)` x_min = left = 1, x_max = right = width, y_min = top = 1, y_max = "
                   "bottom = height.");

        union {
            u8*     data_1B;
            GREY8*  g8;
            GREYa8* ga8;
            RGB8*   rgb8;
            RGBa8*  rgba8;
            BGR8*   bgr8;
            BGRa8*  bgra8;
        } tmp;

        tmp.data_1B = m_d.data_1B;
        m_d         = {nullptr};

        u32 old_w = m_width;
        m_width   = x2 - x1;
        m_height  = y2 - y1;

        m_pixelCount = m_width * m_height;

        initPixels();
#if 0
        u32 idx_new = 0;
        switch (pixelFormat()) {
        case PF_GREY8:
            std::ranges::for_each(std::views::iota(y1, y2), [this, &tmp, &x1, &x2, &old_w, &idx_new](const u32& y) {
                std::ranges::for_each(std::views::iota(x1, x2), [this, &y, &tmp, &idx_new, &old_w](const u32& x) {
                    const u32 idx_old = (old_w * (y - 1)) + (x - 1);
                    m_d.g8[idx_new]   = tmp.g8[idx_old];
                    ++idx_new;
                });
            });
            break;
        case PF_GREYa8:
            std::ranges::for_each(std::views::iota(y1, y2), [this, &tmp, &x1, &x2, &old_w, &idx_new](const u32& y) {
                std::ranges::for_each(std::views::iota(x1, x2), [this, y, &tmp, &idx_new, old_w](const u32& x) {
                    const u32 idx_old = (old_w * (y - 1)) + (x - 1);
                    m_d.ga8[idx_new]  = tmp.ga8[idx_old];
                    ++idx_new;
                });
            });
            break;
        case PF_RGB8:
            std::ranges::for_each(std::views::iota(y1, y2), [this, &tmp, &x1, &x2, &old_w, &idx_new](const u32& y) {
                std::ranges::for_each(std::views::iota(x1, x2), [this, &y, &tmp, &idx_new, &old_w](const u32& x) {
                    const u32 idx_old = (old_w * (y - 1)) + (x - 1);
                    m_d.rgb8[idx_new] = tmp.rgb8[idx_old];
                    ++idx_new;
                });
            });
            break;
        case PF_RGBa8:
            std::ranges::for_each(std::views::iota(y1, y2), [this, &tmp, &x1, &x2, &old_w, &idx_new](const u32& y) {
                std::ranges::for_each(std::views::iota(x1, x2), [this, &y, &tmp, &idx_new, &old_w](const u32& x) {
                    const u32 idx_old  = (old_w * (y - 1)) + (x - 1);
                    m_d.rgba8[idx_new] = tmp.rgba8[idx_old];
                    ++idx_new;
                });
            });
            break;
        case PF_BGR8:
            std::ranges::for_each(std::views::iota(y1, y2), [this, &tmp, &x1, &x2, &old_w, &idx_new](const u32& y) {
                std::ranges::for_each(std::views::iota(x1, x2), [this, y, &tmp, &idx_new, old_w](const u32& x) {
                    const u32 idx_old = (old_w * (y - 1)) + (x - 1);
                    m_d.bgr8[idx_new] = tmp.bgr8[idx_old];
                    ++idx_new;
                });
            });
            break;
        case PF_BGRa8:
            std::ranges::for_each(std::views::iota(y1, y2), [this, &tmp, &x1, &x2, &old_w, &idx_new](const u32& y) {
                std::ranges::for_each(std::views::iota(x1, x2), [this, y, &tmp, &idx_new, old_w](const u32& x) {
                    const u32 idx_old  = (old_w * (y - 1)) + (x - 1);
                    m_d.bgra8[idx_new] = tmp.bgra8[idx_old];
                    ++idx_new;
                });
            });
            break;
        default:
            IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[m_pixelFormat].c_str());
            break;
        }
#else
        for (u32 y = y1, ii = 0; y < y2; ++y) {
            for (u32 x = x1; x < x2; ++x, ++ii) {
                u32 idx_old = (old_w * (y - 1)) + (x - 1);
                // clang-format off
                switch (pixelFormat()) {
                    case PF_GREY8:  m_d.g8[ii]    = tmp.g8[idx_old];     break;
                    case PF_GREYa8: m_d.ga8[ii]   = tmp.ga8[idx_old];    break;
                    case PF_RGB8:   m_d.rgb8[ii]  = tmp.rgb8[idx_old];   break;
                    case PF_RGBa8:  m_d.rgba8[ii] = tmp.rgba8[idx_old];  break;
                    case PF_BGR8:   m_d.bgr8[ii]  = tmp.bgr8[idx_old];   break;
                    case PF_BGRa8:  m_d.bgra8[ii] = tmp.bgra8[idx_old];  break;
                    default: IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[m_pixelFormat].c_str()); break;
                }
                // clang-format on
            }
        }
#endif

        delete[] tmp.data_1B;

        return *this;
    }

    void Image::initPixels() {
        if (m_d.data_1B) {
            IMG_ABORT("image pixel data is already initialized");
        }

        // clang-format off
        switch (m_pixelFormat) {
            case PF_UNKOWN: m_d.data_1B = new u8[m_pixelCount * m_channelCount]; break;
            case PF_GREY8:  m_d.g8      = new GREY8[m_pixelCount];               break;
            case PF_GREYa8: m_d.ga8     = new GREYa8[m_pixelCount];              break;
            case PF_RGB8:   m_d.rgb8    = new RGB8[m_pixelCount];                break;
            case PF_RGBa8:  m_d.rgba8   = new RGBa8[m_pixelCount];               break;
            case PF_BGR8:   m_d.bgr8    = new BGR8[m_pixelCount];                break;
            case PF_BGRa8:  m_d.bgra8   = new BGRa8[m_pixelCount];               break;
            default: IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[m_pixelFormat].c_str()); break;
        }
        // clang-format on
    }

    void Image::copyPixelsFromOtherImage(const Image& other) {
        if (!m_d.data_1B) {
            IMG_ABORT("trying to copy pixels into nullptr");
        }

        // clang-format off
        switch (other.pixelFormat()) {//src begin     src end                             dist
            case PF_GREY8:  std::copy(other.m_d.g8,    other.m_d.g8    + other.pixelCount(), m_d.g8);          break;
            case PF_GREYa8: std::copy(other.m_d.ga8,   other.m_d.ga8   + other.pixelCount(), m_d.ga8);         break;
            case PF_RGB8:   std::copy(other.m_d.rgb8,  other.m_d.rgb8  + other.pixelCount(), m_d.rgb8);        break;
            case PF_RGBa8:  std::copy(other.m_d.rgba8, other.m_d.rgba8 + other.pixelCount(), m_d.rgba8);       break;
            case PF_BGR8:   std::copy(other.m_d.bgr8,  other.m_d.bgr8  + other.pixelCount(), m_d.bgr8);        break;
            case PF_BGRa8:  std::copy(other.m_d.bgra8, other.m_d.bgra8 + other.pixelCount(), m_d.bgra8);       break;
            default: IMG_ABORT("channel Type: %s is not implemented!", PixelFormatMap[m_pixelFormat].c_str()); break;
        }
        // clang-format on
    }
} // namespace img
