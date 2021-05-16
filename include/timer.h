#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <sstream>

std::string time_remaining(std::chrono::milliseconds step_time, int steps_remaining) {
    using namespace std::chrono;

    typedef duration<int, std::ratio<86400>> days;

    auto input_seconds = std::chrono::duration<double, std::milli>(step_time.count() * steps_remaining);

    auto d = duration_cast<days>(input_seconds);
    input_seconds -= d;
    
    auto h = duration_cast<hours>(input_seconds);
    input_seconds -= h;
    
    auto m = duration_cast<minutes>(input_seconds);
    input_seconds -= m;
    
    auto s = duration_cast<seconds>(input_seconds);

    auto dc = d.count();
    auto hc = h.count();
    auto mc = m.count();
    auto sc = s.count();

    std::stringstream ss;
    ss.fill('0');
    if (dc) {
        ss << d.count() << "d : ";
    }
    if (dc || hc) {
        if (dc) { ss << std::setw(2); } //pad if second set of numbers
        ss << h.count() << "h : ";
    }
    if (dc || hc || mc) {
        if (dc || hc) { ss << std::setw(2); }
        ss << m.count() << "m : ";
    }
    if (dc || hc || mc || sc) {
        if (dc || hc || mc) { ss << std::setw(2); }
        ss << s.count() << 's';
    }

    return ss.str();
}

#endif // TIMER_H