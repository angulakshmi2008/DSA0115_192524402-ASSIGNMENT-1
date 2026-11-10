#include <iomanip>
#include <iostream>
#include <limits>

enum class Zone { Inner, Middle, Outer };
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

inline bool isPeak(int h, int m)
{
    int t = h * 60 + m;
    return (t >= 8 * 60 && t < 10 * 60) || (t >= 17 * 60 && t < 20 * 60);
}

long long fare(Zone z, bool peak, bool concession)
{
    long long f = BASE_PAISE[static_cast<int>(z)];
    if (concession) {
        f -= (f * 50 + 50) / 100;
    }
    if (peak) {
        f += (f * PEAK_SURCHARGE_PERCENT + 50) / 100;
    }
    return f;
}

bool canTravel(long long balance, long long f)
{
    return balance - f >= MIN_BALANCE_PAISE;
}

int main()
{
    long long balance = 10000; // Rs 100.00 in paise
    int n = 0;
    std::cout << "Journeys: ";

    // Robust loop to read journey count
    while (!(std::cin >> n) || n < 0)
    {
        std::cout << "Invalid count, try again\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (std::cin.eof()) return 1;
    }

    std::cout << '\n'
              << std::left << std::setw(4) << "No" << std::setw(6) << "Zone"
              << std::setw(7) << "Time" << std::right << std::setw(10) << "Fare(Rs)"
              << std::setw(13) << "Balance(Rs)" << '\n';

    for (int i = 1; i <= n; ++i)
    {
        int zone, h, m;
        // Validation: verify stream health and domain ranges
        if (!(std::cin >> zone >> h >> m) || zone < 0 || zone > 2 || h < 0 || h > 23 || m < 0 || m > 59)
        {
            std::cout << "Journey " << i << ": invalid input, skipped\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        long long f = fare(static_cast<Zone>(zone), isPeak(h, m), false);

        std::cout << std::left << std::setw(4) << i << std::setw(6) << zone
                  << std::setfill('0') << std::right << std::setw(2) << h << ':'
                  << std::setw(2) << m << std::setfill(' ') << "  ";

        if (canTravel(balance, f))
        {
            balance -= f;
            std::cout << std::setw(10) << std::fixed << std::setprecision(2)
                      << f / 100.0 << std::setw(13) << balance / 100.0 << '\n';
        }
        else
        {
            std::cout << std::setw(10) << "DENIED" << std::setw(13) << balance / 100.0
                      << '\n';
        }
    }

    return 0;
}
