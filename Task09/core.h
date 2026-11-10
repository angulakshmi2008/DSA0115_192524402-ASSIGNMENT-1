#ifndef CORE_H
#define CORE_H

enum class Zone { Inner, Middle, Outer };

constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int CONCESSION_PERCENT = 50;
constexpr long long MIN_BALANCE_PAISE = 5000;

constexpr int PEAK_AM_START = 8 * 60;
constexpr int PEAK_AM_END   = 10 * 60;
constexpr int PEAK_PM_START = 17 * 60;
constexpr int PEAK_PM_END   = 20 * 60;

inline bool isPeak(int hour, int minute)
{
    int t = hour * 60 + minute;
    return (t >= PEAK_AM_START && t < PEAK_AM_END) ||
           (t >= PEAK_PM_START && t < PEAK_PM_END);
}

inline long long fare(Zone z, bool peak, bool concession)
{
    long long f = BASE_PAISE[static_cast<int>(z)];
    if (concession) {
        f -= (f * CONCESSION_PERCENT + 50) / 100;
    }
    if (peak) {
        f += (f * PEAK_SURCHARGE_PERCENT + 50) / 100;
    }
    return f;
}

inline bool canTravel(long long balance, long long f)
{
    return f >= 0 && (balance - f >= MIN_BALANCE_PAISE);
}

#endif // CORE_H
