#ifndef LIB_IMG_TYPES_H
#define LIB_IMG_TYPES_H

#include <array>
#include <cstdint>
#include <type_traits>

namespace img {

    using u8  = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    using i8  = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    // clang-format off
    template<typename T>
    requires std::is_arithmetic_v<T>
    using arr2 = std::array<T, 2>;

    template<typename T>
    requires std::is_arithmetic_v<T>
    using arr3 = std::array<T, 3>;

    template<typename T>
    requires std::is_arithmetic_v<T>
    using arr4 = std::array<T, 4>;

    template<typename T, std::size_t sz, template<typename, std::size_t> class arr = std::array>
    concept is_allowed_arr_sz =  std::is_same_v<arr<T, sz>, arr2<T>>
                              || std::is_same_v<arr<T, sz>, arr3<T>>
                              || std::is_same_v<arr<T, sz>, arr4<T>>;

    // clang-format on

    template<typename T>
    concept isPixel
        = std::is_same_v<T, struct GREY8> || std::is_same_v<T, struct GREYa8> || std::is_same_v<T, struct RGB8>
          || std::is_same_v<T, struct RGBa8> || std::is_same_v<T, struct BGR8> || std::is_same_v<T, struct BGRa8>
          || std::is_same_v<T, struct HDR32>;

    template<typename T>
    concept is_grey_scale_pixel = std::is_same_v<T, struct GREY8> || std::is_same_v<T, struct GREYa8>;

    template<typename T>
    concept is_1_channel_pixel = std::is_same_v<T, struct GREY8>;

    template<typename T>
    concept is_2_channel_pixel = std::is_same_v<T, struct GREYa8>;

    template<typename T>
    concept is_3_channel_pixel = std::is_same_v<T, struct RGB8> || std::is_same_v<T, struct BGR8>;

    template<typename T>
    concept is_4_channel_pixel = std::is_same_v<T, struct RGBa8> || std::is_same_v<T, struct BGRa8>;

} // namespace img

#define IMG_8BIT_MASK  0x00'00'00'FF // 0000'0000'0000'0000'0000'0000'1111'1111
#define IMG_10BIT_MASK 0x00'00'03'FF // 0000'0000'0000'0000'0000'0011'1111'1111
#define IMG_12BIT_MASK 0x00'00'0F'FF // 0000'0000'0000'0000'0000'1111'1111'1111

#endif // LIB_IMG_TYPES_H
