#pragma once
#include <sys/time.h>

typedef void(*on_elapsed_callback)(float msec_difference, void* state);
class timer final {
    static_assert(sizeof(long long)>=8,"This platform is not supported because it cannot operate on 64-bit integers.");
    on_elapsed_callback m_on_elapsed;
    void* m_on_elapsed_state;
    float m_interval;
    timeval m_prev;
    timer(const timer& rhs)=delete;
    timer& operator=(const timer& rhs)=delete;
    void do_move(timer& rhs);
public:
    timer();
    timer(timer&& rhs);
    timer& operator=(timer&& rhs);
    void on_elapsed(on_elapsed_callback callback, void* callback_state = nullptr);
    void interval(float msecs, bool repeat = true);
    void reset();
    void update();
};