#include <timer.hpp>
void timer::do_move(timer& rhs) {
    m_on_elapsed = rhs.m_on_elapsed;
    rhs.m_on_elapsed = nullptr;
    m_on_elapsed_state = rhs.m_on_elapsed_state;
    m_interval = rhs.m_interval;
    rhs.m_interval = 0;
    m_prev = rhs.m_prev;
}
timer::timer() : m_on_elapsed(nullptr), m_interval(0) {
}
timer::timer(timer&& rhs) {
    do_move(rhs);
}
timer& timer::operator=(timer&& rhs) {
    do_move(rhs);
    return *this;
}
void timer::on_elapsed(on_elapsed_callback callback, void* callback_state) {
    m_on_elapsed = callback;
    m_on_elapsed_state = callback_state;
}
void timer::interval(float msecs, bool repeat) {
    if(msecs!=msecs) {return;}
    if(0>msecs) {return;}
    if(!repeat) {
        msecs = -msecs;
    }
    m_interval = msecs;
    if(m_interval!=0) {
        reset();
    }
}
void timer::reset() {
    gettimeofday(&m_prev,nullptr);
}
void timer::update() {
    if(0==m_interval || m_on_elapsed==nullptr) {
        return;
    }
    timeval t;
    if(0!=gettimeofday(&t,nullptr)) {
        return;
    }
    float msecs = ((float)t.tv_sec*1000.0f)+((float)t.tv_usec/1000.0f);
    float prev_msecs = ((float)m_prev.tv_sec*1000.0f)+((float)m_prev.tv_usec/1000.0f);
    if(0>m_interval) {
        float i = -m_interval;
        if(msecs-prev_msecs>=i) {
            m_on_elapsed((msecs-prev_msecs)-i,m_on_elapsed_state);
            m_interval = 0;
            reset();
        }
    } else {
        if(msecs-prev_msecs>=m_interval) {
            m_on_elapsed((msecs-prev_msecs)-m_interval,m_on_elapsed_state);
            reset();
        }
        
    }
}