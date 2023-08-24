#ifndef LIB_IMG_TIMER_H
#define LIB_IMG_TIMER_H

#include <chrono>
#include <iostream>

namespace img {

    namespace chr = std::chrono;

    class Timer {
        using Clock      = chr::high_resolution_clock;
        using time_point = typename Clock::time_point;

    public:
        using nsec = chr::nanoseconds;
        using usec = chr::microseconds;
        using msec = chr::milliseconds;

        Timer& start() {
            m_start = Clock::now();
            return *this;
        }

        Timer& restart() {
            m_start = Clock::now();
            return *this;
        }

        chr::nanoseconds nsElapsed() {
            return Clock::now() - m_start;
        }

        template<typename TimeUnit>
        TimeUnit elapsed() const {
            return chr::duration_cast<TimeUnit>(Clock::now() - m_start);
        }

        void log_elapsed(const std::string& bench_name) const {
            auto now = Clock::now();
            std::cout << "[TIME] " << bench_name << ": " << chr::duration_cast<usec>(now - m_start) << ", "
                      << chr::duration_cast<msec>(now - m_start) << "\n";
        }

        template<typename TimeUnit>
        void log_elapsed(const std::string& bench_name) const {
            std::cout << "[TIME] " << bench_name << ": " << chr::duration_cast<TimeUnit>(Clock::now() - m_start)
                      << "\n";
        }

    private:
        time_point m_start = Clock::now();
    };
} // namespace img

#endif // LIB_IMG_TIMER_H
