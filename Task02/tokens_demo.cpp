/**
 * Task02 - Token Classification & Demonstration
 * Demonstrates the 5 kinds of C++ tokens:
 * 1. Keywords
 * 2. Identifiers
 * 3. Constants (Literals)
 * 4. Operators
 * 5. Punctuators
 */

#include <iostream>
#include "fare_constants.h"

int main() {
    std::cout << "=== Task 02: Classification of C++ Tokens ===\n\n";

    // Example code statement from Metro fare calculation:
    // constexpr long long MIN_BALANCE_PAISE = 5000;
    // if (peak) base += (base * PEAK_SURCHARGE_PERCENT) / 100;

    std::cout << "1. Keywords    : constexpr, long, if, return, int, static_cast, bool\n";
    std::cout << "2. Identifiers : MIN_BALANCE_PAISE, BASE_PAISE, Zone, main, result\n";
    std::cout << "3. Literals    : 5000, 1000, 20, 100, true, false, 'A', \"Rs \"\n";
    std::cout << "4. Operators   : =, +=, *, /, %, ==, >=, &&, ||, ::, <<\n";
    std::cout << "5. Punctuators : { } ( ) [ ] ; , :\n\n";

    std::cout << "Constant values defined in fare_constants.h:\n";
    std::cout << "  Base Fares: Inner = " << BASE_PAISE[0] << " p, Middle = "
              << BASE_PAISE[1] << " p, Outer = " << BASE_PAISE[2] << " p\n";
    std::cout << "  Peak Surcharge: " << PEAK_SURCHARGE_PERCENT << "%\n";
    std::cout << "  Concession    : " << CONCESSION_PERCENT << "%\n";
    std::cout << "  Min Balance   : " << MIN_BALANCE_PAISE << " p (Rs 50)\n";

    return 0;
}
