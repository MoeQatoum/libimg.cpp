#ifndef LIB_ING_SIMPLE_CONTAINERS_H
#define LIB_ING_SIMPLE_CONTAINERS_H

#include "types.hpp"
#include <common.hpp>

namespace img {

    template<typename T, std::size_t sz>
    struct container {
        using value_type = T;

        container() = default;

        container(const std::initializer_list<T> list) {
            std::copy(list.begin(), list.end(), m_data);
        }

        container(const container<T, sz>& other) {
            std::copy(other.begin(), other.end(), m_data);
        }

        T& operator[](std::size_t idx) {
            static_assert(0 <= idx < sz, "out of bounds error!");
            return m_data[idx];
        }

        const T& at(std::size_t idx) const {
            static_assert(0 <= idx < sz, "out of bounds error!");
            return m_data[idx];
        }

        constexpr std::size_t size() const {
            return sz;
        }

        const T* begin() const {
            return m_data;
        }

        const T* end() const {
            return m_data + sz;
        }

    private:
        T m_data[sz] = {0};
    };

    template<typename T>
        requires std::is_arithmetic_v<T>
    using con2 = container<T, 2>;

    template<typename T>
        requires std::is_arithmetic_v<T>
    using con3 = container<T, 3>;

    template<typename T>
        requires std::is_arithmetic_v<T>
    using con4 = container<T, 4>;

} // namespace img

#endif // LIB_ING_SIMPLE_CONTAINERS_H