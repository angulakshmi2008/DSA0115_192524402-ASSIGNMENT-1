/**
 * Task12 - Demonstration of Original Design Flaws Under Changed Requirements
 * Shows:
 * 1. Adding Airport to enum causes silent free ride (fare 0) in switch
 * 2. Original fare() signature cannot accept weekday or concession
 * 3. SmartCard denies pass-covered journeys if balance < Rs 50
 */

#include <iostream>

enum class Zone { Inner, Middle, Outer, Airport };
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};

// Original switch-based fare logic from Task 06
long long fareOld(Zone z, bool peak) {
    long long base = 0;
    switch (z) {
        case Zone::Inner:  base = BASE_PAISE[0]; break;
        case Zone::Middle: base = BASE_PAISE[1]; break;
        case Zone::Outer:  base = BASE_PAISE[2]; break;
        // Airport is NOT handled in switch!
    }
    if (peak) base += (base * 20) / 100;
    return base;
}

int main() {
    std::cout << "=== Task 12 Flaw Demonstration (Original Design) ===\n\n";

    // Example 1: Airport journey evaluated with old switch
    long long airportFare = fareOld(Zone::Airport, false);
    std::cout << "Evaluating Zone::Airport off-peak fare using old switch:\n";
    std::cout << "  Calculated Fare: " << airportFare << " paise (Rs 0.00 - SILENT FREE RIDE REVENUE LOSS!)\n\n";

    std::cout << "Example 2: Original signature fare(Zone, bool peak) cannot receive weekday, date, or concession.\n";
    std::cout << "Example 3: SmartCard::deduct() requires balance >= 5000 paise; cannot allow valid pass holders with low balance.\n";

    return 0;
}
