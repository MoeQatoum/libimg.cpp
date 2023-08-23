#ifndef LIB_IMG_PIXEL_H
#define LIB_IMG_PIXEL_H

#include <array>
#include <cstdint>

#include "common.hpp"
#include "ops.hpp"
#include "types.hpp"

namespace img {

    // template<typename T1, typename T2>
    // T1 clamp(T1 d, T2 min, T2 max) {
    //     const T1 t = d < min ? min : d;
    //     return t > max ? max : t;
    // }

    // template<typename T>
    // u8 clamp_8b(T n) {
    //     return clamp(n, 0, 255);
    // }

    constexpr u16 operator""_US(unsigned long long x) {
        return static_cast<u16>(x);
    }

    constexpr u8 operator""_UB(unsigned long long x) {
        return static_cast<u8>(x);
    }

    struct LIB_IMG_PUBLIC GREY8 {
        uint8_t g;

        operator u8() const {
            return g;
        }

        template<typename T>
        GREY8& operator+(const T& other) {
            Add<GREY8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        GREY8& operator-(const T& other) {
            Subtract<GREY8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        GREY8& operator*(const T& other) {
            Multiply<GREY8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        GREY8& operator+=(const T& other) {
            return (*this + other);
        }

        template<typename T>
        GREY8& operator-=(const T& other) {
            return (*this - other);
        }

        template<typename T>
        GREY8& operator*=(const T& other) {
            return (*this * other);
        }
    };

    struct LIB_IMG_PUBLIC GREYa8 {
        uint8_t g, a;

        operator std::array<u8, 2>() const {
            return {g, a};
        }

        template<typename T>
        GREYa8& operator+(const T& other) {
            Add<GREYa8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        GREYa8& operator-(const T& other) {
            Subtract<GREYa8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        GREYa8& operator*(const T& other) {
            Multiply<GREYa8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        GREYa8& operator+=(const T& other) {
            return (*this + other);
        }

        template<typename T>
        GREYa8& operator-=(const T& other) {
            return (*this - other);
        }

        template<typename T>
        GREYa8& operator*=(const T& other) {
            return (*this * other);
        }
    };

    //////////////RGB//////////////

    struct LIB_IMG_PUBLIC RGB8 {
        uint8_t r, g, b;

        operator std::array<u8, 3>() const {
            return {r, g, b};
        }

        template<typename T>
        RGB8& operator+(const T& other) {
            Add<RGB8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        RGB8& operator-(const T& other) {
            Subtract<RGB8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        RGB8& operator*(const T& other) {
            Multiply<RGB8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        RGB8& operator+=(const T& other) {
            return (*this + other);
        }

        template<typename T>
        RGB8& operator-=(const T& other) {
            return (*this - other);
        }

        template<typename T>
        RGB8& operator*=(const T& other) {
            return (*this * other);
        }
    };

    //////////////RGBA//////////////

    struct LIB_IMG_PUBLIC RGBa8 {
        uint8_t r, g, b, a;

        operator std::array<u8, 4>() const {
            return {r, g, b, a};
        }

        template<typename T>
        RGBa8& operator+(const T& other) {
            Add<RGBa8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        RGBa8& operator-(const T& other) {
            Subtract<RGBa8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        RGBa8& operator*(const T& other) {
            Multiply<RGBa8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        RGBa8& operator+=(const T& other) {
            return (*this + other);
        }

        template<typename T>
        RGBa8& operator-=(const T& other) {
            return (*this - other);
        }

        template<typename T>
        RGBa8& operator*=(const T& other) {
            return (*this * other);
        }
    };

    //////////////BGR//////////////

    struct LIB_IMG_PUBLIC BGR8 {
        uint8_t b, g, r;

        operator std::array<u8, 3>() const {
            return {r, g, b};
        }

        template<typename T>
        BGR8& operator+(const T& other) {
            Add<BGR8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        BGR8& operator-(const T& other) {
            Subtract<BGR8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        BGR8& operator*(const T& other) {
            Multiply<BGR8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        BGR8& operator+=(const T& other) {
            return (*this + other);
        }

        template<typename T>
        BGR8& operator-=(const T& other) {
            return (*this - other);
        }

        template<typename T>
        BGR8& operator*=(const T& other) {
            return (*this * other);
        }
    };

    //////////////BGRA//////////////

    struct LIB_IMG_PUBLIC BGRa8 {
        uint8_t b, g, r, a;

        operator std::array<u8, 4>() const {
            return {r, g, b, a};
        }

        template<typename T>
        BGRa8& operator+(const T& other) {
            Add<BGRa8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        BGRa8& operator-(const T& other) {
            Subtract<BGRa8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        BGRa8& operator*(const T& other) {
            Multiply<BGRa8>::apply(*this, other);
            return *this;
        }

        template<typename T>
        BGRa8& operator+=(const T& other) {
            return (*this + other);
        }

        template<typename T>
        BGRa8& operator-=(const T& other) {
            return (*this - other);
        }

        template<typename T>
        BGRa8& operator*=(const T& other) {
            return (*this * other);
        }
    };

    struct LIB_IMG_PUBLIC HDR32 {};

} // namespace img

#endif // LIB_IMG_PIXEL_H