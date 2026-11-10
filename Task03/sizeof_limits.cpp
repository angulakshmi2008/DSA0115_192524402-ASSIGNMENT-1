/**
 * Task03 - Data Types, System Sizes, and Limits
 * Compiles under: g++ -std=c++17 -Wall -Wextra -pedantic
 */

#include <iostream>
#include <climits>
#include <cstdint>
#include <iomanip>

int main() {
    std::cout << "=== Task 03: System Data Types, Sizes, and Limits ===\n\n";

    // 1. System sizeof measurements
    std::cout << "--- System Data Type Sizes (bytes) ---\n";
    std::cout << "sizeof(bool)        : " << sizeof(bool) << " byte\n";
    std::cout << "sizeof(char)        : " << sizeof(char) << " byte\n";
    std::cout << "sizeof(int)         : " << sizeof(int) << " bytes\n";
    std::cout << "sizeof(unsigned int): " << sizeof(unsigned int) << " bytes\n";
    std::cout << "sizeof(long long)   : " << sizeof(long long) << " bytes\n";
    std::cout << "sizeof(float)       : " << sizeof(float) << " bytes\n";
    std::cout << "sizeof(double)      : " << sizeof(double) << " bytes\n\n";

    // 2. Numerical ranges from <climits>
    std::cout << "--- Integer Limits & Network Capacity ---\n";
    std::cout << "INT_MAX             : " << INT_MAX << " (" << 2147483647LL << ")\n";
    std::cout << "LLONG_MAX           : " << LLONG_MAX << "\n\n";

    // Whole network daily total analysis (3,000,000 journeys with average fare ~18.75 Rs = 1875 paise):
    // 3,000,000 * 1875 paise = 5,625,000,000 paise (~5.625 Billion paise)
    long long networkJourneys = 3000000LL;
    long long avgFarePaise = 1875LL;
    long long networkTotalPaise = networkJourneys * avgFarePaise;

    std::cout << "Network daily total (3,000,000 journeys @ avg 1875 paise):\n";
    std::cout << "  Projected Total   : " << networkTotalPaise << " paise\n";
    std::cout << "  Fits in 32-bit int? " << (networkTotalPaise <= INT_MAX ? "YES" : "NO (EXCEEDS INT_MAX by 3,477,516,353 paise!)") << "\n";
    std::cout << "  Fits in long long?  YES (LLONG_MAX is ~9.22 x 10^18)\n\n";

    // 3. Floating-point binary representation hazard (IEEE-754)
    std::cout << "--- IEEE-754 Floating-Point Precision Flaws ---\n";
    std::cout << std::setprecision(17);
    double d1 = 0.1;
    double d2 = 0.2;
    double sum = d1 + d2;
    std::cout << "0.1 + 0.2 = " << sum << '\n';
    std::cout << "Does 0.1 + 0.2 == 0.3? " << std::boolalpha << (sum == 0.3) << " (FALSE!)\n";

    double fareDouble = 10.80; // Rs 10.80
    std::cout << "10.80 stored as double: " << fareDouble << '\n';
    std::cout << "10 times 0.10 == 1.00? " << ((0.1 * 10) == 1.0) << '\n';

    return 0;
}
