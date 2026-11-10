#ifndef FARE_CONSTANTS_H
#define FARE_CONSTANTS_H

// Scoped enum for metro zones - prevents accidental mixing with ints
enum class Zone {
    Inner,
    Middle,
    Outer
};

// Base fares in integer paise: Rs 10.00, Rs 20.00, Rs 30.00
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};

// Surcharge and concession percentages
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int CONCESSION_PERCENT = 50;

// Minimum balance required to remain on the card after fare deduction (Rs 50.00)
constexpr long long MIN_BALANCE_PAISE = 5000;

// Peak windows in minutes from midnight: [08:00, 10:00) and [17:00, 20:00)
constexpr int PEAK_AM_START = 8 * 60;   // 480 min (08:00)
constexpr int PEAK_AM_END   = 10 * 60;  // 600 min (10:00 - off-peak)
constexpr int PEAK_PM_START = 17 * 60;  // 1020 min (17:00)
constexpr int PEAK_PM_END   = 20 * 60;  // 1200 min (20:00 - off-peak)

#endif // FARE_CONSTANTS_H
