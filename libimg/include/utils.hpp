#ifndef LIB_IMG_UTILS_H
#define LIB_IMG_UTILS_H

#include "common.hpp"
#include "img_assert.hpp"
#include "types.hpp"

namespace img {
    constexpr u16 operator""_US(unsigned long long x) {
        return static_cast<u16>(x);
    }

    constexpr u8 operator""_UB(unsigned long long x) {
        return static_cast<u8>(x);
    }

    template<typename T, typename U>
    U clamp(const T d, const U min, const U max) {
        const T t = d < min ? min : d;
        return t > max ? max : t;
    }

    template<typename T>
    u8 clamp_U8B(const T n) {
        return clamp<T, u8>(n, 0, 255);
    }

    template<typename Pixel, typename T>
    auto clampColorChanel(const T value) {
        if constexpr (is_color_8_bit_depth<Pixel>) {
            return clamp<T, u8>(value, 0, 255);
        }

        UNREACHABLE();
    }

} // namespace img

#endif // LIB_IMG_UTILS_H