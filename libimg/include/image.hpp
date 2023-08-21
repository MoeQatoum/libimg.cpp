#ifndef LIB_IMG_IMAGE_H
#define LIB_IMG_IMAGE_H

#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>

#include "common.hpp"
#include "types.hpp"

namespace img {

    namespace fs = std::filesystem;

    class LIB_IMG_PUBLIC Image {
    public:
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

        Image(uint32_t width, uint32_t height, PixelFmt pf);
        Image();

    public:
        Image(const fs::path& fileName);
        Image(const Image& other);
        Image(Image&& other);
        ~Image();

        static Image    creatBlankImage(uint32_t width, uint32_t height, PixelFmt pf);
        static Image    GaussianRandomNoise(uint32_t width, uint32_t height, PixelFmt pf, float mean, float dev);
        static ImageFmt getImageFormat(const fs::path& filePath);
        static PixelFmt getPixelFormat(const fs::path& filePath);

        Image& operator=(const Image& other);
        Image& operator=(Image&& other);

        friend Image operator+(const Image& LHS, const Image& RHS);
        friend Image operator-(const Image& LHS, const Image& RHS);

        uint32_t height() const {
            return m_height;
        };

        uint32_t width() const {
            return m_width;
        };

        uint32_t chanelCount() const {
            return m_channelCount;
        };

        PixelFmt pixelFormat() const {
            return m_pixelFormat;
        }

        uint32_t pixelCount() const {
            return m_pixelCount;
        }

        bool isNull() {
            return !m_data_1B;
        }

        void forEachPixel(std::function<void(u32 /* x */, u32 /* y */)> function);

        bool save(fs::path filePath, bool png_for_unsupported_format = true) const;

        Image& colorMask(float r, float g, float b);
        Image& colorMaskAt(uint32_t x, uint32_t y, float r, float g, float b);

        Image& alphaMask(float a);
        Image& alphaMaskAt(uint32_t x, uint32_t y, float a);

        Image& addGaussianNoise(float mean, float dev);

        Image& greyScaleAvg();
        Image& greyScaleLum();

        Image& resize();
        Image& crop();
        Image& blur();

    private:
        void initPixels();
        void copyPixelsFromOtherImage(const Image& other);

    private:
        PixelFmt m_pixelFormat;

        u32 m_width, m_height, m_channelCount, m_pixelCount;

        union { /* image pixels */
            u8* m_data_1B;

            struct GREY8*  m_g8;
            struct GREYa8* m_ga8;
            struct RGB8*   m_rgb8;
            struct RGBa8*  m_rgba8;
            struct BGR8*   m_bgr8;
            struct BGRa8*  m_bgra8;
        };
    };

} // namespace img

#endif // LIB_IMG_IMAGE_H