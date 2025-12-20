#include "CalculatorETA.hpp"

int CalculatorETA::_targetValue = 0;
int CalculatorETA::_prevValue = 0;
double CalculatorETA::_avgSpeed = 0;
bool CalculatorETA::_isFirstRun = true;
std::chrono::time_point<std::chrono::steady_clock> CalculatorETA::_prevTime;

std::string CalculatorETA::formatSeconds(long long totalSeconds)
{
    if (totalSeconds < 0) return "--:--:--";
    
    int h = totalSeconds / 3600;
    int m = (totalSeconds % 3600) / 60;
    int s = totalSeconds % 60;

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", h, m, s);
    return std::string(buffer);
}

void CalculatorETA::reset(int targetValue)
{
    _targetValue = targetValue;
    _prevValue = 0;
    _avgSpeed = 0.0;
    _isFirstRun = true;
    _prevTime = Clock::now();
}

std::string CalculatorETA::update(int currentValue) 
{
    auto now = Clock::now();
    
    // Obliczamy różnicę czasu w milisekundach
    long long deltaTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - _prevTime).count();
    
    // Zabezpieczenie przed dzieleniem przez zero lub zbyt częstym wywoływaniem
    // Jeśli minęło 0ms, zwracamy poprzedni wynik (lub puste, jeśli start)
    if (deltaTimeMs <= 0) {
        return _isFirstRun ? "--:--:--" : formatSeconds((long long)((_targetValue - currentValue) / (_avgSpeed > 0 ? _avgSpeed : 1)));
    }

    double deltaValue = (double)currentValue - _prevValue;
    
    // Obliczamy prędkość chwilową (jednostki na milisekundę)
    double currentSpeed = deltaValue / deltaTimeMs;

    // Logika wygładzania (EMA) - eliminuje skakanie czasu
    if (_isFirstRun) {
        _avgSpeed = currentSpeed;
        _isFirstRun = false;
    } else {
        _avgSpeed = (currentSpeed * SMOOTHING_FACTOR) + (_avgSpeed * (1.0 - SMOOTHING_FACTOR));
    }

    // Aktualizacja stanu na następny cykl
    _prevTime = now;
    _prevValue = currentValue;

    // Obliczanie czasu do końca
    double remainingValue = (double)_targetValue - currentValue;
    
    if (remainingValue <= 0) return "00:00:00";
    if (_avgSpeed <= 0.000001) return "--:--:--"; // Zatrzymano lub brak postępu

    // Czas w sekundach = (pozostała wartość / prędkość(ms)) / 1000
    long long secondsLeft = static_cast<long long>((remainingValue / _avgSpeed) / 1000.0);

    return formatSeconds(secondsLeft);
}