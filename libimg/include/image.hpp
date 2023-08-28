#ifndef LIB_IMG_IMAGE_H
#define LIB_IMG_IMAGE_H

#include <filesystem>
#include <functional>
#include <random>
#include <unordered_map>
#include <utility>

#include "common.hpp"
#include "img_assert.hpp"
#include "pixel.hpp"
#include "types.hpp"
#include "utils.hpp"

#include "stb_image.h"
#include "stb_image_write.h"

namespace img {

    namespace fs = std::filesystem;

    template<typename>
    class Image;

    template<typename T, typename A>
    constexpr T make_pixel_from_array(arr4<A>&& a) {
        return std::apply([](auto&&... args) { return T{decltype(args)(args)...}; }, std::forward<arr4<A>>(a));
    }

    template<typename T>
        requires(is_pixel_type<T>)
    inline u32 channelCountFromPixelType() {
        if constexpr (std::is_same_v<T, GREY8>) {
            return 1;
        }

        if constexpr (std::is_same_v<T, GREYa8>) {
            return 2;
        }

        if constexpr (std::is_same_v<T, RGB8> || std::is_same_v<T, BGR8>) {
            return 3;
        }

        if constexpr (std::is_same_v<T, RGBa8> || std::is_same_v<T, BGRa8>) {
            return 4;
        }
    }

    template<typename Pixel>
    class LIB_IMG_PUBLIC Image {

        template<class U>
        friend class Image;

        enum ImageFmt : u16 {
            IF_UNKOWN = 0X000,
            IF_JPEG   = 0x001,
            IF_JPG    = 0x002,
            IF_PNG    = 0x004,
            IF_BMP    = 0x008,
            IF_PSD    = 0x010,
            IF_TGA    = 0x020,
            IF_GIF    = 0x040,
            IF_HDR    = 0x080,
            IF_PIC    = 0x100,
            IF_PNM    = 0x200,
        };

        enum PixelFmt : u16 {
            PF_UNKOWN = 0X0000,
            PF_GREY8  = 0x0001,
            PF_GREYa8 = 0x0002,
            PF_RGB8   = 0x0004,
            PF_RGBa8  = 0x0020,
            PF_BGR8   = 0x0100,
            PF_BGRa8  = 0x0800,
        };

        enum ChannelCountMask : u16 {
            M_1C = PF_GREY8,
            M_2C = PF_GREY8 | PF_GREYa8,
            M_3C = PF_RGB8 | PF_BGR8,
            M_4C = PF_RGBa8 | PF_BGRa8,
        };

    public:
        using Pixel_t         = Pixel;
        using Iterator_t      = Pixel*;
        using ConstIterator_t = const Pixel*;

        Image() : m_d(nullptr), m_width(0), m_height(0), m_pixelCount(0) {
        }

    public:
        Image(uint32_t width, uint32_t height)
            : m_d(nullptr),
              m_width(width),
              m_height(height),
              m_pixelCount(width * height) {
            m_d = new Pixel_t[m_pixelCount];
        }

        Image(uint32_t width, uint32_t height, Pixel_t fillColor)
            : m_d(nullptr),
              m_width(width),
              m_height(height),
              m_pixelCount(width * height) {
            m_d = new Pixel_t[m_pixelCount];
            fill(fillColor);
        }

        Image(const fs::path& filePath) : m_d(nullptr) {
            if (!fs::exists(filePath)) {
                IMG_ABORT("file does not exist: %s", filePath.c_str());
            }

            u8* d;
            int w, h, c;
            if ((d = stbi_load(filePath.c_str(), &w, &h, &c, 0))) {
                m_width  = static_cast<u32>(w);
                m_height = static_cast<u32>(h);

                IMG_DEBUG_ASSERT((m_width * m_height) <= LIB_IMG_MAX_SIZE,
                                 "Image %s pixel count exceeded `LIB_IMG_MAX_SIZE`: %u, image pixel count: %d"
                                 "are you sure this file is valid?",
                                 filePath.c_str(),
                                 LIB_IMG_MAX_SIZE,
                                 m_width * m_height);

                m_pixelCount = m_width * m_height;

                m_d = new Pixel_t[m_pixelCount];

                std::copy(d, d + (m_pixelCount * c), reinterpret_cast<u8*>(m_d));
                stbi_image_free(d);
            } else {
                IMG_ABORT("stb couldn't load image file: %s", filePath.c_str());
            }
        }

        Image(const Image& other) : m_width(other.m_width), m_height(other.m_height), m_pixelCount(m_width * m_height) {
            m_d = new Pixel_t[m_pixelCount];
            std::copy(other.m_d, other.m_d + other.pixelCount(), m_d);
        }

        Image(Image&& other)
            : m_d(std::move(other.m_d)),
              m_width(std::move(other.m_width)),
              m_height(std::move(other.m_height)),
              m_pixelCount(std::move(other.m_pixelCount)) {
            other.m_d = nullptr;
        }

        ~Image() {
            if (m_d) {
                delete[] m_d;
            }
        }

        static Image creatBlankImage(uint32_t width, uint32_t height, PixelFmt pf) {
            IMG_ABORT("Unimplemented");
        }

        template<typename T>
        static Image<T> gaussianRandomNoise(uint32_t width, uint32_t height, float mean, float dev) {
            Image<T> img(width, height);

            auto gen     = std::bind(std::normal_distribution<float>{mean, dev}, std::mt19937(std::random_device{}()));
            auto rand_u8 = [&gen]() { return static_cast<u8>(gen()); };

            std::for_each(img.m_d, img.m_d + img.pixelCount(), [&rand_u8](Pixel_t& p) { p = rand_u8(); });

            return img;
        }

        Image& operator=(const Image& other) {
            if (this == &other) {
                return *this;
            }

            m_width      = other.m_width;
            m_height     = other.m_height;
            m_pixelCount = other.m_pixelCount;

            if (m_d) {
                delete[] m_d;
            }

            m_d = new Pixel_t[m_pixelCount];
            std::copy(other.m_d, other.m_d + other.pixelCount(), m_d);

            return *this;
        }

        Image& operator=(Image&& other) {
            if (this == &other) {
                return *this;
            }

            m_width      = std::move(other.m_width);
            m_height     = std::move(other.m_height);
            m_pixelCount = std::move(other.m_pixelCount);

            if (m_d) {
                delete[] m_d;
                m_d = nullptr;
            }

            m_d       = std::move(other.m_d);
            other.m_d = nullptr;

            return *this;
        }

        Image& operator~() {
            std::for_each(m_d, m_d + m_pixelCount, [](Pixel_t& p) { ~p; });
            return *this;
        }

        Pixel_t& operator[](u32 idx) {
            return m_d[idx];
        }

        Pixel_t& operator[](u32 x, u32 y) {
            return m_d[(m_width * y) + x];
        }

        [[nodiscard]] Image friend operator+(const Image& LHS, const Image& RHS) {
            uint32_t w_max = LHS.width() > RHS.width() ? LHS.width() : RHS.width();
            uint32_t h_max = LHS.height() > RHS.height() ? LHS.height() : RHS.height();
            Image    ret{w_max, h_max};

            for (u32 y = 0, idx = 0; y < ret.height(); ++y) {
                for (u32 x = 0; x < ret.width(); ++x, ++idx) {
                    u32 rhsIdx   = RHS.width() * (y % RHS.height()) + (x % RHS.width());
                    u32 lhsIdx   = LHS.width() * (y % LHS.height()) + (x % LHS.width());
                    ret.m_d[idx] = LHS.m_d[lhsIdx] + RHS.m_d[rhsIdx];
                }
            }

            return ret;
        }

        [[nodiscard]] Image friend operator-(const Image& LHS, const Image& RHS) {
            u32   w_max = LHS.width() > RHS.width() ? LHS.width() : RHS.width();
            u32   h_max = LHS.height() > RHS.height() ? LHS.height() : RHS.height();
            Image ret{w_max, h_max};

            for (u32 y = 0, idx = 0; y < ret.height(); ++y) {
                for (u32 x = 0; x < ret.width(); ++x, ++idx) {
                    u32 rhsIdx   = RHS.width() * (y % RHS.height()) + (x % RHS.width());
                    u32 lhsIdx   = LHS.width() * (y % LHS.height()) + (x % LHS.width());
                    ret.m_d[idx] = LHS.m_d[lhsIdx] - RHS.m_d[rhsIdx];
                }
            }

            return ret;
        }

        const Pixel_t& pixelAt(u32 idx) const {
            return m_d[idx];
        }

        const Pixel_t& pixelAt(u32 x, u32 y) const {
            return m_d[(m_width * y) + x];
        }

        u32 height() const {
            return m_height;
        };

        u32 width() const {
            return m_width;
        };

        u32 chanelCount() const {
            return channelCountFromPixelType<Pixel_t>();
        };

        u32 pixelCount() const {
            return m_pixelCount;
        }

        bool isNull() const {
            return !m_d;
        }

        Iterator_t begin() {
            return m_d;
        }

        ConstIterator_t begin() const {
            return m_d;
        }

        Iterator_t end() {
            return m_d;
        }

        ConstIterator_t end() const {
            return m_d;
        }

        bool save(fs::path filePath, bool png_for_unsupported_format = true) const {
            if (!filePath.has_filename() || !filePath.has_extension()) {
                IMG_ABORT("Invalid image path: %s", filePath.c_str());
            }

            int ret;
            int w = static_cast<int>(m_width);
            int h = static_cast<int>(m_height);
            int c = static_cast<int>(channelCountFromPixelType<Pixel_t>());

            // clang-format off
            switch (getImageFormat(filePath)) {
                case IF_JPG:
                case IF_JPEG: ret = stbi_write_jpg(filePath.c_str(), w, h, c, reinterpret_cast<u8*>(m_d), 100);   break;
                case IF_PNG:  ret = stbi_write_png(filePath.c_str(), w, h, c, reinterpret_cast<u8*>(m_d), w * c); break;
                // case IF_HDR: break;
                default:
                    if (png_for_unsupported_format /* allow implicit conversion on unsupported format ??*/) {
                        IMG_LOG_WARN("saving \"%s\" extension is not supported, defaulting to \".png\"", filePath.extension().c_str());
                        filePath.replace_extension(".png");
                        ret = stbi_write_png(filePath.c_str(), w, h, c, reinterpret_cast<u8*>(m_d), w * c);
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

        Image& colorMask(float r, float g, float b) {
            std::for_each(m_d, m_d + m_pixelCount, [&r, &g, &b](Pixel_t& p) { p *= arr3<float>{r, g, b}; });
            return *this;
        }

        Image& fill(const Pixel_t fillColor) {
            std::for_each(m_d, m_d + m_pixelCount, [&fillColor](Pixel_t& p) { p = fillColor; });
            return *this;
        }

        Image& flipX() {
            for (u32 y = 0; y < m_height; ++y) {
                for (u32 x = 0; x < (m_width / 2); ++x) {
                    u32 idx    = (m_width * y) + x;
                    u32 op_idx = (m_width * y) + (m_width - 1 - x);
                    std::swap(m_d[idx], m_d[op_idx]);
                }
            }
            return *this;
        }

        Image& flipY() {
            for (u32 y = 0; y < (m_height / 2); ++y) {
                for (u32 x = 0; x < m_width; ++x) {
                    u32 idx    = (m_width * y) + x;
                    u32 op_idx = (m_width * (m_height - 1 - y)) + x;
                    std::swap(m_d[idx], m_d[op_idx]);
                }
            }
            return *this;
        }

        Image& rotateRight() {
            Pixel_t* oldPixels = m_d;
            m_d                = new Pixel_t[m_pixelCount];

            for (u32 x = 0, idx = 0; x < m_width; ++x) {
                for (u32 y = 0; y < m_height; ++y, ++idx) {
                    u32 oldIdx = (m_width * y) + x;
                    m_d[idx]   = oldPixels[oldIdx];
                }
            }

            std::swap(m_width, m_height);
            delete[] oldPixels;

            flipX();

            return *this;
        }

        Image& rotateLeft() {
            Pixel_t* oldPixels = m_d;
            m_d                = new Pixel_t[m_pixelCount];

            for (u32 x = 0, idx = 0; x < m_width; ++x) {
                for (u32 y = 0; y < m_height; ++y, ++idx) {
                    u32 oldIdx = (m_width * (m_height - y - 1) + (m_width - 1 - x));
                    m_d[idx]   = oldPixels[oldIdx];
                }
            }

            std::swap(m_width, m_height);
            delete[] oldPixels;

            flipX();

            return *this;
        }

        Image& addGaussianNoise(float mean, float dev) {
            auto gen = std::bind(std::normal_distribution<float>{mean, dev}, std::mt19937(std::random_device{}()));
            if constexpr (is_1_channel_pixel<Pixel_t> || is_1_channel_pixel<Pixel_t>) {
                std::for_each(
                    m_d, m_d + m_pixelCount, [&gen](Pixel_t& p) { p.g = clampColorChanel<Pixel_t>(p.g + gen()); });
            }

            if constexpr (is_3_channel_pixel<Pixel_t> || is_4_channel_pixel<Pixel_t>) {
                std::for_each(m_d, m_d + m_pixelCount, [&gen](Pixel_t& p) { p += arr3<float>{gen(), gen(), gen()}; });
            }

            return *this;
        }

        [[nodiscard]] auto addSaltAndPepperNoise(float prob, float randBotLimit, float randTopLimit) {

            auto gen = std::bind(std::uniform_real_distribution(randBotLimit, randTopLimit),
                                 std::mt19937(std::random_device{}()));

            if constexpr (is_3_channel_pixel<Pixel_t>) {
                auto img = greyScaleLum();
                std::for_each(img.m_d, img.m_d + img.pixelCount(), [&gen, &prob](GREY8& p) {
                    float rand = gen();
                    if (rand < (prob / 2)) {
                        p.g = 255;
                    } else if (rand > (1 - (prob / 2))) {
                        p.g = 0;
                    }
                });
                return img;
            }

            if constexpr (is_4_channel_pixel<Pixel_t>) {
                auto img = greyScaleLum();
                std::for_each(img.m_d, img.m_d + img.pixelCount(), [&gen, &prob](GREYa8& p) {
                    float rand = gen();
                    if (rand < (prob / 2)) {
                        p.g = 255;
                    } else if (rand > (1 - (prob / 2))) {
                        p.g = 0;
                    }
                });
                return img;
            }

            UNREACHABLE();
        }

        [[nodiscard]] auto greyScaleAvg()
            requires(!is_grey_scale_pixel<Pixel_t>)
        {
            const float rf = .2126f, gf = .7152f, bf = .0722f;
            if constexpr (is_4_channel_pixel<Pixel_t>) {
                Image<GREYa8> ret{m_width, m_height};
                u32           i = 0;
                std::for_each(ret.m_d, ret.m_d + ret.pixelCount(), [this, &i, &rf, &gf, &bf](GREYa8& p) {
                    p = GREYa8{.g = clampColorChanel<Pixel_t>((m_d[i].r + m_d[i].g + m_d[i].b) / 3.), .a = m_d[i].a};
                    ++i;
                });
                return ret;
            }

            if constexpr (is_3_channel_pixel<Pixel_t>) {
                Image<GREY8> ret{m_width, m_height};
                u32          i = 0;
                std::for_each(ret.m_d, ret.m_d + ret.pixelCount(), [this, &i, &rf, &gf, &bf](GREY8& p) {
                    p = GREY8{.g = clampColorChanel<Pixel_t>((m_d[i].r + m_d[i].g + m_d[i].b) / 3.f)};
                    ++i;
                });
                return ret;
            }

            UNREACHABLE();
        }

        [[nodiscard]] auto greyScaleLum()
            requires(!is_grey_scale_pixel<Pixel_t>)

        {
            const float rf = .2126f, gf = .7152f, bf = .0722f;

            if constexpr (is_4_channel_pixel<Pixel_t>) {
                Image<GREYa8> ret{m_width, m_height};
                u32           i = 0;
                std::for_each(ret.m_d, ret.m_d + ret.pixelCount(), [this, &i, &rf, &gf, &bf](GREYa8& p) {
                    p = GREYa8{.g = clamp_U8B(rf * m_d[i].r + gf * m_d[i].g + bf * m_d[i].b), .a = m_d[i].a};
                    ++i;
                });
                return ret;
            }

            if constexpr (is_3_channel_pixel<Pixel_t>) {
                Image<GREY8> ret{m_width, m_height};
                u32          i = 0;
                std::for_each(ret.m_d, ret.m_d + ret.pixelCount(), [this, &i, &rf, &gf, &bf](GREY8& p) {
                    p = GREY8{.g = clamp_U8B(rf * m_d[i].r + gf * m_d[i].g + bf * m_d[i].b)};
                    ++i;
                });
                return ret;
            }

            UNREACHABLE();
        }

        Image& pad(u32 topPad, u32 bottomPad, u32 leftPad, u32 rightPad, Pixel_t padColor) {
            u32 idx_x_1 = rightPad;
            u32 idx_y_1 = topPad;
            u32 idx_x_2 = m_width + rightPad - 1;
            u32 idx_y_2 = m_height + topPad - 1;

            u32 oldWidth = m_width;

            m_height += topPad + bottomPad;
            m_width += rightPad + leftPad;
            m_pixelCount = m_width * m_height;

            Pixel_t* oldPixels = m_d;
            m_d                = new Pixel_t[m_pixelCount];

            for (u32 y = 0, idx = 0; y < m_height; ++y) {
                for (u32 x = 0; x < m_width; ++x, ++idx) {
                    if ((x >= idx_x_1) && (x <= idx_x_2) && (y >= idx_y_1) && (y <= idx_y_2)) {
                        u32 oldIdx = oldWidth * (y - topPad) + (x - rightPad);
                        m_d[idx]   = oldPixels[oldIdx];
                    } else {
                        m_d[idx] = padColor;
                    }
                }
            }

            delete[] oldPixels;

            return *this;
        }

        Image& padBorderEqual(u32 padSize, Pixel_t padColor) {
            pad(padSize, padSize, padSize, padSize, padColor);
            return *this;
        }

        Image& padTop(u32 padSize, Pixel_t padColor) {
            pad(padSize, 0, 0, 0, padColor);
            return *this;
        }

        Image& padBottom(u32 padSize, Pixel_t padColor) {
            pad(0, padSize, 0, 0, padColor);
            return *this;
        }

        Image& padLeft(u32 padSize, Pixel_t padColor) {
            pad(0, 0, padSize, 0, padColor);
            return *this;
        }

        Image& padRight(u32 padSize, Pixel_t padColor) {
            pad(0, 0, 0, padSize, padColor);
            return *this;
        }

        Image& rescale(u32 width, u32 height) {
            IMG_ABORT("Unimplemented");
        }

        Image& crop(u32 x1, u32 y1, u32 x2, u32 y2) {
            IMG_ASSERT((x2 > x1) && (y2 > y1) && (x1 * x2 * y1 * y2 != 0),
                       "`(x2 > x1 > 0) && (y2 > y1 > 0)` x_min = left = 1, x_max = right = width, y_min = top = 1, "
                       "y_max = "
                       "bottom = height.");

            u32 old_w = m_width;
            m_width   = x2 - x1;
            m_height  = y2 - y1;

            m_pixelCount = m_width * m_height;

            Pixel_t* oldPixels = m_d;

            m_d = new Pixel_t[m_pixelCount];

            for (u32 y = y1, idx = 0; y < y2; ++y) {
                u32 offset = (old_w * (y - 1));
                for (u32 x = x1; x < x2; ++x, ++idx) {
                    u32 oldIdx = offset + (x - 1);
                    m_d[idx]   = oldPixels[oldIdx];
                }
            }

            delete[] oldPixels;

            return *this;
        }

        Image& blur() {
            IMG_ABORT("Unimplemented");
        }

    private:
        ImageFmt getImageFormat(const fs::path& filePath) const {
            if (!filePath.has_extension()) {
                IMG_ABORT("Image extention is missing, the path is invalid: `%s`", filePath.c_str());
            }

            std::string ext = filePath.extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), [](const unsigned char& c) { return std::toupper(c); });
            return imageTypeMap.at(ext);
        }

    private:
        Pixel* m_d;

        u32 m_width, m_height, m_pixelCount;

        const std::unordered_map<std::string, ImageFmt> imageTypeMap{
            {".JPEG", IF_JPEG},
            { ".JPG",  IF_JPG},
            { ".PNG",  IF_PNG},
            { ".BMP",  IF_BMP},
            { ".HDR",  IF_HDR}
        };

        // const std::unordered_map<PixelFmt, std::string> PixelFormatMap{
        //     { PF_GREY8,  "GREY8"},
        //     {PF_GREYa8, "GREYa8"},
        //     {  PF_RGB8,   "RGB8"},
        //     { PF_RGBa8,  "RGBa8"},
        //     {  PF_BGR8,   "BGR8"},
        //     { PF_BGRa8,  "BGRa8"},
        // };
    };

} // namespace img

#endif // LIB_IMG_IMAGE_H