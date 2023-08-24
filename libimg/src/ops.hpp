#ifndef LIB_IMG_PIXEL_OPS_H
#define LIB_IMG_PIXEL_OPS_H

#include <common.hpp>
#include <types.hpp>

#include "simple_containers.hpp"

namespace img {

    template<typename T1, typename T2>
    T1 clamp(T1 d, T2 min, T2 max) {
        const T1 t = d < min ? min : d;
        return t > max ? max : t;
    }

    template<typename T>
    u8 clamp_8b(T n) {
        return clamp(n, 0, 255);
    }

    template<typename pixel_t, typename Binary_OP_t>
        requires isPixel<pixel_t>
    struct Binary_OP {

        template<typename T>
        static void apply(pixel_t& _this, const T& other) {
            Binary_OP_t::apply(_this, other);
        }
    };

    struct Add_OP {
        template<typename pixel_t>
            requires isPixel<pixel_t>
        static void apply(pixel_t& _this, const pixel_t& other) {
            if constexpr (std::is_same_v<pixel_t, GREY8>) {
                _this.g = clamp_8b(_this.g + other.g);
            }

            if constexpr (std::is_same_v<pixel_t, GREYa8>) {
                _this.g = clamp_8b(_this.g + other.g);
                _this.a = clamp_8b(_this.a + other.a);
            }

            if constexpr (std::is_same_v<pixel_t, RGB8> || std::is_same_v<pixel_t, BGR8>) {
                _this.r = clamp_8b(_this.r + other.r);
                _this.g = clamp_8b(_this.g + other.g);
                _this.b = clamp_8b(_this.b + other.b);
            }

            if constexpr (std::is_same_v<pixel_t, RGBa8> || std::is_same_v<pixel_t, BGRa8>) {
                _this.r = clamp_8b(_this.r + other.r);
                _this.g = clamp_8b(_this.g + other.g);
                _this.b = clamp_8b(_this.b + other.b);
                _this.a = clamp_8b(_this.a + other.a);
            }
        }

        template<typename pixel_t, typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires isPixel<pixel_t> && std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        static void apply(pixel_t& _this, const array<U, sz>& arr) {
            if constexpr (1 == sz || sz == 2) {
                _this.g = clamp_8b(_this.g + arr[0]);
                if constexpr (sz == 2) {
                    _this.a = clamp_8b(_this.a + arr[1]);
                }
            }

            if constexpr (3 == sz || sz == 4) {
                _this.r = clamp_8b(_this.r + arr[0]);
                _this.g = clamp_8b(_this.g + arr[1]);
                _this.b = clamp_8b(_this.b + arr[2]);
                if constexpr (sz == 4) {
                    _this.a = clamp_8b(_this.a + arr[3]);
                }
            }
        }
    };

    struct subtract_OP {
        template<typename pixel_t>
            requires isPixel<pixel_t>
        static void apply(pixel_t& _this, const pixel_t& other) {
            if constexpr (std::is_same_v<pixel_t, GREY8>) {
                _this.g = clamp_8b(_this.g - other.g);
            }

            if constexpr (std::is_same_v<pixel_t, GREYa8>) {
                _this.g = clamp_8b(_this.g - other.g);
                _this.a = clamp_8b(_this.a - other.a);
            }

            if constexpr (std::is_same_v<pixel_t, RGB8> || std::is_same_v<pixel_t, BGR8>) {
                _this.r = clamp_8b(_this.r - other.r);
                _this.g = clamp_8b(_this.g - other.g);
                _this.b = clamp_8b(_this.b - other.b);
            }

            if constexpr (std::is_same_v<pixel_t, RGBa8> || std::is_same_v<pixel_t, BGRa8>) {
                _this.r = clamp_8b(_this.r - other.r);
                _this.g = clamp_8b(_this.g - other.g);
                _this.b = clamp_8b(_this.b - other.b);
                _this.a = clamp_8b(_this.a - other.a);
            }
        }

        template<typename pixel_t, typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires isPixel<pixel_t> && std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        static void apply(pixel_t& _this, const array<U, sz>& arr) {
            if constexpr (1 == sz || sz == 2) {
                _this.g = clamp_8b(_this.g - arr[0]);
                if constexpr (sz == 2) {
                    _this.a = clamp_8b(_this.a - arr[1]);
                }
            }

            if constexpr (3 == sz || sz == 4) {
                _this.r = clamp_8b(_this.r - arr[0]);
                _this.g = clamp_8b(_this.g - arr[1]);
                _this.b = clamp_8b(_this.b - arr[2]);
                if constexpr (sz == 4) {
                    _this.a = clamp_8b(_this.a - arr[3]);
                }
            }
        }
    };

    struct multiply_OP {
        template<typename pixel_t>
            requires isPixel<pixel_t>
        static void apply(pixel_t& _this, const pixel_t& other) {
            if constexpr (std::is_same_v<pixel_t, GREY8>) {
                _this.g = clamp_8b(_this.g * other.g);
            }

            if constexpr (std::is_same_v<pixel_t, GREYa8>) {
                _this.g = clamp_8b(_this.g * other.g);
                _this.a = clamp_8b(_this.a * other.a);
            }

            if constexpr (std::is_same_v<pixel_t, RGB8> || std::is_same_v<pixel_t, BGR8>) {
                _this.r = clamp_8b(_this.r * other.r);
                _this.g = clamp_8b(_this.g * other.g);
                _this.b = clamp_8b(_this.b * other.b);
            }

            if constexpr (std::is_same_v<pixel_t, RGBa8> || std::is_same_v<pixel_t, BGRa8>) {
                _this.r = clamp_8b(_this.r * other.r);
                _this.g = clamp_8b(_this.g * other.g);
                _this.b = clamp_8b(_this.b * other.b);
                _this.a = clamp_8b(_this.a * other.a);
            }
        }

        template<typename pixel_t, typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires isPixel<pixel_t> && std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        static void apply(pixel_t& _this, const array<U, sz>& arr) {
            if constexpr (1 == sz || sz == 2) {
                _this.g = clamp_8b(_this.g * arr[0]);
                if constexpr (sz == 2) {
                    _this.a = clamp_8b(_this.a * arr[1]);
                }
            }

            if constexpr (3 == sz || sz == 4) {
                _this.r = clamp_8b(_this.r * arr[0]);
                _this.g = clamp_8b(_this.g * arr[1]);
                _this.b = clamp_8b(_this.b * arr[2]);
                if constexpr (sz == 4) {
                    _this.a = clamp_8b(_this.a * arr[3]);
                }
            }
        }
    };

    struct divide_OP {
        template<typename pixel_t>
            requires isPixel<pixel_t>
        static void apply(pixel_t& _this, const pixel_t& other) {
            if constexpr (std::is_same_v<pixel_t, GREY8>) {
                _this.g = clamp_8b(_this.g * other.g);
            }

            if constexpr (std::is_same_v<pixel_t, GREYa8>) {
                _this.g = clamp_8b(_this.g / other.g);
                _this.a = clamp_8b(_this.a / other.a);
            }

            if constexpr (std::is_same_v<pixel_t, RGB8> || std::is_same_v<pixel_t, BGR8>) {
                _this.r = clamp_8b(_this.r / other.r);
                _this.g = clamp_8b(_this.g / other.g);
                _this.b = clamp_8b(_this.b / other.b);
            }

            if constexpr (std::is_same_v<pixel_t, RGBa8> || std::is_same_v<pixel_t, BGRa8>) {
                _this.r = clamp_8b(_this.r / other.r);
                _this.g = clamp_8b(_this.g / other.g);
                _this.b = clamp_8b(_this.b / other.b);
                _this.a = clamp_8b(_this.a / other.a);
            }
        }

        template<typename pixel_t, typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires isPixel<pixel_t> && std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        static void apply(pixel_t& _this, const array<U, sz>& arr) {
            if constexpr (1 == sz || sz == 2) {
                _this.g = clamp_8b(_this.g / arr[0]);
                if constexpr (sz == 2) {
                    _this.a = clamp_8b(_this.a / arr[1]);
                }
            }

            if constexpr (3 == sz || sz == 4) {
                _this.r = clamp_8b(_this.r / arr[0]);
                _this.g = clamp_8b(_this.g / arr[1]);
                _this.b = clamp_8b(_this.b / arr[2]);
                if constexpr (sz == 4) {
                    _this.a = clamp_8b(_this.a / arr[3]);
                }
            }
        }
    };

    template<typename Pixel_t>
    using Add = Binary_OP<Pixel_t, Add_OP>;

    template<typename Pixel_t>
    using Subtract = Binary_OP<Pixel_t, subtract_OP>;

    template<typename Pixel_t>
    using Multiply = Binary_OP<Pixel_t, multiply_OP>;

    template<typename Pixel_t>
    using Divide = Binary_OP<Pixel_t, divide_OP>;

} // namespace img

#endif // LIB_IMG_PIXEL_OPS_H