#ifndef LIB_IMG_PIXEL_OPS_2_H
#define LIB_IMG_PIXEL_OPS_2_H

#include <immintrin.h>

#include "common.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace img {

    template<typename pixel_t>
        requires is_pixel_type<pixel_t>
    struct PIXEL_NOR_OP {
        friend pixel_t& operator~(pixel_t& _this) {
            if constexpr (is_1_channel_pixel<pixel_t> || is_2_channel_pixel<pixel_t>) {
                _this.g = ~(_this.g);
            }

            if constexpr (is_3_channel_pixel<pixel_t> || is_4_channel_pixel<pixel_t>) {
                _this.r = ~(_this.r);
                _this.g = ~(_this.g);
                _this.b = ~(_this.b);
            }

            // if constexpr (is_4_channel_pixel<pixel_t>) {
            //     _this.a = ~(_this.a);
            // }

            return _this;
        }
    };

    template<typename pixel_t>
        requires is_pixel_type<pixel_t>
    struct PIXEL_ADD_OP {
        [[nodiscard]] pixel_t friend operator+(const pixel_t& LHS, const pixel_t& RHS) {
            pixel_t tmp;

            if constexpr (std::is_same_v<pixel_t, GREY8>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g + RHS.g);
            }

            if constexpr (std::is_same_v<pixel_t, GREYa8>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g + RHS.g);
                tmp.a = clampColorChanel<pixel_t>(LHS.a + RHS.a);
            }

            if constexpr (std::is_same_v<pixel_t, RGB8> || std::is_same_v<pixel_t, BGR8>) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r + RHS.r);
                tmp.g = clampColorChanel<pixel_t>(LHS.g + RHS.g);
                tmp.b = clampColorChanel<pixel_t>(LHS.b + RHS.b);
            }

            if constexpr (std::is_same_v<pixel_t, RGBa8> || std::is_same_v<pixel_t, BGRa8>) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r + RHS.r);
                tmp.g = clampColorChanel<pixel_t>(LHS.g + RHS.g);
                tmp.b = clampColorChanel<pixel_t>(LHS.b + RHS.b);
                tmp.a = clampColorChanel<pixel_t>(LHS.a + RHS.a);
            }

            return tmp;
        }

        template<typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        [[nodiscard]] friend pixel_t operator+(const pixel_t& LHS, const array<U, sz>& arr) {
            // TODO: fix this mess!

            pixel_t tmp;

            if constexpr (is_1_channel_pixel<pixel_t>) {
                static_assert(sz == 1 || sz == 2);
                tmp.g = clampColorChanel<pixel_t>(LHS.g + arr[0]);
            }

            if constexpr (is_2_channel_pixel<pixel_t>) {
                static_assert(sz == 1 || sz == 2);
                tmp.g = clampColorChanel<pixel_t>(LHS.g + arr[0]);
                if constexpr (sz == 2) {
                    tmp.a = clampColorChanel<pixel_t>(LHS.a + arr[1]);
                }
            }

            if constexpr (is_3_channel_pixel<pixel_t>) {
                static_assert(sz == 3 || sz == 4);
                tmp.r = clampColorChanel<pixel_t>(LHS.r + arr[0]);
                tmp.g = clampColorChanel<pixel_t>(LHS.g + arr[1]);
                tmp.b = clampColorChanel<pixel_t>(LHS.b + arr[2]);
            }

            if constexpr (is_4_channel_pixel<pixel_t>) {
                static_assert(sz == 3 || sz == 4);
                tmp.r = clampColorChanel<pixel_t>(LHS.r + arr[0]);
                tmp.g = clampColorChanel<pixel_t>(LHS.g + arr[1]);
                tmp.b = clampColorChanel<pixel_t>(LHS.b + arr[2]);
                if constexpr (sz == 4) {
                    tmp.a = clampColorChanel<pixel_t>(LHS.a + arr[3]);
                }
            }

            return tmp;
        }

        template<typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        friend pixel_t& operator+=(pixel_t& LHS, const array<U, sz>& arr) {
            // TODO: fix this mess!

            if constexpr (is_1_channel_pixel<pixel_t>) {
                static_assert(sz == 1 || sz == 2);
                LHS.g = clampColorChanel<pixel_t>(LHS.g + arr[0]);
            }

            if constexpr (is_2_channel_pixel<pixel_t>) {
                static_assert(sz == 1 || sz == 2);
                LHS.g = clampColorChanel<pixel_t>(LHS.g + arr[0]);
                if constexpr (sz == 2) {
                    LHS.a = clampColorChanel<pixel_t>(LHS.a + arr[1]);
                }
            }

            if constexpr (is_3_channel_pixel<pixel_t>) {
                static_assert(sz == 3 || sz == 4);
                LHS.r = clampColorChanel<pixel_t>(LHS.r + arr[0]);
                LHS.g = clampColorChanel<pixel_t>(LHS.g + arr[1]);
                LHS.b = clampColorChanel<pixel_t>(LHS.b + arr[2]);
            }

            if constexpr (is_4_channel_pixel<pixel_t>) {
                static_assert(sz == 3 || sz == 4);
                LHS.r = clampColorChanel<pixel_t>(LHS.r + arr[0]);
                LHS.g = clampColorChanel<pixel_t>(LHS.g + arr[1]);
                LHS.b = clampColorChanel<pixel_t>(LHS.b + arr[2]);
                if constexpr (sz == 4) {
                    LHS.a = clampColorChanel<pixel_t>(LHS.a + arr[3]);
                }
            }

            return LHS;
        }
    };

    template<typename pixel_t>
        requires is_pixel_type<pixel_t>
    struct PIXEL_SUB_OP {
        [[nodiscard]] friend pixel_t operator-(const pixel_t& LHS, const pixel_t& RHS) {
            pixel_t tmp;

            if constexpr (std::is_same_v<pixel_t, GREY8>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g - RHS.g);
            }

            if constexpr (std::is_same_v<pixel_t, GREYa8>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g - RHS.g);
                tmp.a = clampColorChanel<pixel_t>(LHS.a - RHS.a);
            }

            if constexpr (std::is_same_v<pixel_t, RGB8> || std::is_same_v<pixel_t, BGR8>) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r - RHS.r);
                tmp.g = clampColorChanel<pixel_t>(LHS.g - RHS.g);
                tmp.b = clampColorChanel<pixel_t>(LHS.b - RHS.b);
            }

            if constexpr (std::is_same_v<pixel_t, RGBa8> || std::is_same_v<pixel_t, BGRa8>) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r - RHS.r);
                tmp.g = clampColorChanel<pixel_t>(LHS.g - RHS.g);
                tmp.b = clampColorChanel<pixel_t>(LHS.b - RHS.b);
                tmp.a = clampColorChanel<pixel_t>(LHS.a - RHS.a);
            }

            return tmp;
        }

        template<typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        [[nodiscard]] friend pixel_t operator-(const pixel_t& LHS, const array<U, sz>& arr) {
            // TODO: fix this mess!
            pixel_t tmp;

            if constexpr (sz == 1 && is_1_channel_pixel<pixel_t>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g - arr[0]);
            }

            if constexpr (sz == 2 && is_2_channel_pixel<pixel_t>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g - arr[0]);
                tmp.a = clampColorChanel<pixel_t>(LHS.a - arr[1]);
            }

            if constexpr (sz == 3 && (is_3_channel_pixel<pixel_t>)) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r - arr[0]);
                tmp.g = clampColorChanel<pixel_t>(LHS.g - arr[1]);
                tmp.b = clampColorChanel<pixel_t>(LHS.b - arr[2]);
            }

            if constexpr ((sz == 4 || sz == 3) && is_4_channel_pixel<pixel_t>) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r - arr[0]);
                tmp.g = clampColorChanel<pixel_t>(LHS.g - arr[1]);
                tmp.b = clampColorChanel<pixel_t>(LHS.b - arr[2]);
                if constexpr (sz == 4) {
                    tmp.a = clampColorChanel<pixel_t>(LHS.a - arr[3]);
                }
            }

            return tmp;
        }

        template<typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        friend pixel_t& operator-=(pixel_t& LHS, const array<U, sz>& arr) {
            // TODO: fix this mess!

            if constexpr (is_1_channel_pixel<pixel_t>) {
                static_assert(sz == 1 || sz == 2);
                LHS.g = clampColorChanel<pixel_t>(LHS.g - arr[0]);
            }

            if constexpr (is_2_channel_pixel<pixel_t>) {
                static_assert(sz == 1 || sz == 2);
                LHS.g = clampColorChanel<pixel_t>(LHS.g - arr[0]);
                if constexpr (sz == 2) {
                    LHS.a = clampColorChanel<pixel_t>(LHS.a - arr[1]);
                }
            }

            if constexpr (is_3_channel_pixel<pixel_t>) {
                static_assert(sz == 3 || sz == 4);
                LHS.r = clampColorChanel<pixel_t>(LHS.r - arr[0]);
                LHS.g = clampColorChanel<pixel_t>(LHS.g - arr[1]);
                LHS.b = clampColorChanel<pixel_t>(LHS.b - arr[2]);
            }

            if constexpr (is_3_channel_pixel<pixel_t> || is_3_channel_pixel<pixel_t>) {
                static_assert(sz == 3 || sz == 4);
                LHS.r = clampColorChanel<pixel_t>(LHS.r - arr[0]);
                LHS.g = clampColorChanel<pixel_t>(LHS.g - arr[1]);
                LHS.b = clampColorChanel<pixel_t>(LHS.b - arr[2]);
                if constexpr (sz == 4) {
                    LHS.a = clampColorChanel<pixel_t>(LHS.a - arr[3]);
                }
            }

            return LHS;
        }
    };

    template<typename pixel_t>
        requires is_pixel_type<pixel_t>
    struct PIXEL_MUL_OP {
        [[nodiscard]] friend pixel_t operator*(const pixel_t& LHS, const pixel_t& RHS) {
            pixel_t tmp;

            if constexpr (std::is_same_v<pixel_t, GREY8>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g * RHS.g);
            }

            if constexpr (std::is_same_v<pixel_t, GREYa8>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g * RHS.g);
                tmp.a = clampColorChanel<pixel_t>(LHS.a * RHS.a);
            }

            if constexpr (std::is_same_v<pixel_t, RGB8> || std::is_same_v<pixel_t, BGR8>) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r * RHS.r);
                tmp.g = clampColorChanel<pixel_t>(LHS.g * RHS.g);
                tmp.b = clampColorChanel<pixel_t>(LHS.b * RHS.b);
            }

            if constexpr (std::is_same_v<pixel_t, RGBa8> || std::is_same_v<pixel_t, BGRa8>) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r * RHS.r);
                tmp.g = clampColorChanel<pixel_t>(LHS.g * RHS.g);
                tmp.b = clampColorChanel<pixel_t>(LHS.b * RHS.b);
                tmp.a = clampColorChanel<pixel_t>(LHS.a * RHS.a);
            }

            return tmp;
        }

        template<typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        [[nodiscard]] friend pixel_t operator*(const pixel_t& LHS, const array<U, sz>& arr) {
            pixel_t tmp;

            if constexpr (sz == 1 && is_1_channel_pixel<pixel_t>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g * arr[0]);
            }

            if constexpr (sz == 2 && is_2_channel_pixel<pixel_t>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g * arr[0]);
                tmp.a = clampColorChanel<pixel_t>(LHS.a * arr[1]);
            }

            if constexpr (sz == 3 && (is_3_channel_pixel<pixel_t>)) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r * arr[0]);
                tmp.g = clampColorChanel<pixel_t>(LHS.g * arr[1]);
                tmp.b = clampColorChanel<pixel_t>(LHS.b * arr[2]);
            }

            if constexpr ((sz == 4 || sz == 3) && is_4_channel_pixel<pixel_t>) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r * arr[0]);
                tmp.g = clampColorChanel<pixel_t>(LHS.g * arr[1]);
                tmp.b = clampColorChanel<pixel_t>(LHS.b * arr[2]);
                if constexpr (sz == 4) {
                    tmp.a = clampColorChanel<pixel_t>(LHS.a * arr[3]);
                }
            }

            return tmp;
        }

        template<typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        friend pixel_t& operator*=(pixel_t& LHS, const array<U, sz>& arr) {
            // TODO: fix this mess!

            if constexpr (is_1_channel_pixel<pixel_t>) {
                static_assert(sz == 1 || sz == 2);
                LHS.g = clampColorChanel<pixel_t>(LHS.g * arr[0]);
            }

            if constexpr (is_2_channel_pixel<pixel_t>) {
                static_assert(sz == 1 || sz == 2);
                LHS.g = clampColorChanel<pixel_t>(LHS.g * arr[0]);
                if constexpr (sz == 2) {
                    LHS.a = clampColorChanel<pixel_t>(LHS.a * arr[1]);
                }
            }

            if constexpr (is_3_channel_pixel<pixel_t>) {
                static_assert(sz == 3 || sz == 4);
                LHS.r = clampColorChanel<pixel_t>(LHS.r * arr[0]);
                LHS.g = clampColorChanel<pixel_t>(LHS.g * arr[1]);
                LHS.b = clampColorChanel<pixel_t>(LHS.b * arr[2]);
            }

            if constexpr (is_3_channel_pixel<pixel_t> || is_3_channel_pixel<pixel_t>) {
                static_assert(sz == 3 || sz == 4);
                LHS.r = clampColorChanel<pixel_t>(LHS.r * arr[0]);
                LHS.g = clampColorChanel<pixel_t>(LHS.g * arr[1]);
                LHS.b = clampColorChanel<pixel_t>(LHS.b * arr[2]);
                if constexpr (sz == 4) {
                    LHS.a = clampColorChanel<pixel_t>(LHS.a * arr[3]);
                }
            }

            return LHS;
        }
    };

    template<typename pixel_t>
        requires is_pixel_type<pixel_t>
    struct PIXEL_DEV_OP {
        [[nodiscard]] friend pixel_t operator/(const pixel_t& LHS, const pixel_t& RHS) {
            pixel_t tmp;

            if constexpr (std::is_same_v<pixel_t, GREY8>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g / RHS.g);
            }

            if constexpr (std::is_same_v<pixel_t, GREYa8>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g / RHS.g);
                tmp.a = clampColorChanel<pixel_t>(LHS.a / RHS.a);
            }

            if constexpr (std::is_same_v<pixel_t, RGB8> || std::is_same_v<pixel_t, BGR8>) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r / RHS.r);
                tmp.g = clampColorChanel<pixel_t>(LHS.g / RHS.g);
                tmp.b = clampColorChanel<pixel_t>(LHS.b / RHS.b);
            }

            if constexpr (std::is_same_v<pixel_t, RGBa8> || std::is_same_v<pixel_t, BGRa8>) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r / RHS.r);
                tmp.g = clampColorChanel<pixel_t>(LHS.g / RHS.g);
                tmp.b = clampColorChanel<pixel_t>(LHS.b / RHS.b);
                tmp.a = clampColorChanel<pixel_t>(LHS.a / RHS.a);
            }

            return tmp;
        }

        template<typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        [[nodiscard]] friend pixel_t operator/(const pixel_t& LHS, const array<U, sz>& arr) {
            // TODO: fix this mess!
            pixel_t tmp;

            if constexpr (sz == 1 && is_1_channel_pixel<pixel_t>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g / arr[0]);
            }

            if constexpr (sz == 2 && is_2_channel_pixel<pixel_t>) {
                tmp.g = clampColorChanel<pixel_t>(LHS.g / arr[0]);
                tmp.a = clampColorChanel<pixel_t>(LHS.a / arr[1]);
            }

            if constexpr (sz == 3 && (is_3_channel_pixel<pixel_t>)) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r / arr[0]);
                tmp.g = clampColorChanel<pixel_t>(LHS.g / arr[1]);
                tmp.b = clampColorChanel<pixel_t>(LHS.b / arr[2]);
            }

            if constexpr ((sz == 4 || sz == 3) && is_4_channel_pixel<pixel_t>) {
                tmp.r = clampColorChanel<pixel_t>(LHS.r / arr[0]);
                tmp.g = clampColorChanel<pixel_t>(LHS.g / arr[1]);
                tmp.b = clampColorChanel<pixel_t>(LHS.b / arr[2]);
                if constexpr (sz == 4) {
                    tmp.a = clampColorChanel<pixel_t>(LHS.a / arr[3]);
                }
            }

            return tmp;
        }

        template<typename U, std::size_t sz, template<class, std::size_t> typename array>
            requires std::is_arithmetic_v<U> && is_allowed_arr_sz<U, sz>
        friend pixel_t& operator/=(pixel_t& LHS, const array<U, sz>& arr) {
            // TODO: fix this mess!

            if constexpr (is_1_channel_pixel<pixel_t>) {
                static_assert(sz == 1 || sz == 2);
                LHS.g = clampColorChanel<pixel_t>(LHS.g / arr[0]);
            }

            if constexpr (is_2_channel_pixel<pixel_t>) {
                static_assert(sz == 1 || sz == 2);
                LHS.g = clampColorChanel<pixel_t>(LHS.g / arr[0]);
                if constexpr (sz == 2) {
                    LHS.a = clampColorChanel<pixel_t>(LHS.a / arr[1]);
                }
            }

            if constexpr (is_3_channel_pixel<pixel_t>) {
                static_assert(sz == 3 || sz == 4);
                LHS.r = clampColorChanel<pixel_t>(LHS.r / arr[0]);
                LHS.g = clampColorChanel<pixel_t>(LHS.g / arr[1]);
                LHS.b = clampColorChanel<pixel_t>(LHS.b / arr[2]);
            }

            if constexpr (is_3_channel_pixel<pixel_t> || is_3_channel_pixel<pixel_t>) {
                static_assert(sz == 3 || sz == 4);
                LHS.r = clampColorChanel<pixel_t>(LHS.r / arr[0]);
                LHS.g = clampColorChanel<pixel_t>(LHS.g / arr[1]);
                LHS.b = clampColorChanel<pixel_t>(LHS.b / arr[2]);
                if constexpr (sz == 4) {
                    LHS.a = clampColorChanel<pixel_t>(LHS.a / arr[3]);
                }
            }

            return LHS;
        }
    };

} // namespace img

#endif // LIB_IMG_PIXEL_OPS_H