#ifndef CALCULATOR_ETA_HPP
#define CALCULATOR_ETA_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <cstdio>
#include <cmath>

class CalculatorETA {
private:
    static int _targetValue;        // total frames
    static int _prevValue;          // previous frames value
    static double _avgSpeed;        // smoothed speed
    
    // last mesurement time
    using Clock = std::chrono::steady_clock;
    static std::chrono::time_point<Clock> _prevTime;
    
    static bool _isFirstRun; // flag
    static constexpr double SMOOTHING_FACTOR = 0.05; // (0.0 - 1.0)

    CalculatorETA() = delete; 

    static std::string formatSeconds(long long totalSeconds);

public:
    static void reset(int targetValue);

    static std::string update(int currentValue);
};

#endif // CALCULATOR_ETA_HPP