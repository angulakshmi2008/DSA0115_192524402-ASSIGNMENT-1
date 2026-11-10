/**
 * Task10 - Workload Scale Simulation: Expectations vs Execution
 * Compares accumulation in:
 * 1. long long (exact paise)
 * 2. double (rupees converted to paise)
 * 3. float (rupees)
 * 4. 32-bit int capacity analysis (safe evaluation avoiding UB)
 *
 * Compiles under: g++ -std=c++17 -Wall -Wextra -pedantic
 */

#include <iostream>
#include <iomanip>
#include <climits>
#include <cstdint>

// Deterministic generator to produce reproducible journey distributions
struct WorkloadGenerator {
    uint32_t state = 123456789;

    uint32_t next() {
        state = state * 1664525u + 1013904223u;
        return state;
    }

    // Generates a representative journey fare in paise
    long long nextFarePaise() {
        uint32_t r = next();
        int zone = r % 3;             // 0=Inner, 1=Middle, 2=Outer
        int hour = (r >> 8) % 24;     // Hour 0..23
        bool concession = ((r >> 16) % 5) == 0; // 20% of passengers have concession

        long long base = (zone == 0 ? 1000 : (zone == 1 ? 2000 : 3000));
        if (concession) {
            base -= (base * 50 + 50) / 100;
        }

        bool peak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
        if (peak) {
            base += (base * 20 + 50) / 100;
        }
        return base;
    }
};

void runSimulation(long long journeyCount) {
    WorkloadGenerator gen;

    long long exactPaiseTotal = 0;
    double doubleRupeesTotal = 0.0;
    float floatRupeesTotal = 0.0f;

    for (long long i = 0; i < journeyCount; ++i) {
        long long farePaise = gen.nextFarePaise();

        exactPaiseTotal += farePaise;
        doubleRupeesTotal += (static_cast<double>(farePaise) / 100.0);
        floatRupeesTotal += (static_cast<float>(farePaise) / 100.0f);
    }

    double exactRupees = static_cast<double>(exactPaiseTotal) / 100.0;
    double doubleConvertedPaise = doubleRupeesTotal * 100.0;
    double doubleDiff = doubleConvertedPaise - static_cast<double>(exactPaiseTotal);
    double floatDiffRupees = static_cast<double>(floatRupeesTotal) - exactRupees;

    std::cout << "Journeys: " << std::setw(10) << journeyCount << '\n';
    std::cout << "  Exact total (long long paise) : " << exactPaiseTotal << " paise (Rs "
              << std::fixed << std::setprecision(2) << exactRupees << ")\n";
    std::cout << "  Double accumulator total      : Rs " << doubleRupeesTotal
              << " | Difference: " << doubleDiff << " paise\n";
    std::cout << "  Float accumulator total       : Rs " << floatRupeesTotal
              << " | Difference: Rs " << floatDiffRupees << '\n';

    // Safe 32-bit integer overflow demonstration without invoking undefined behaviour:
    std::cout << "  32-bit int capacity check     : ";
    if (exactPaiseTotal <= INT_MAX) {
        std::cout << "FITS SAFELY (" << exactPaiseTotal << " <= " << INT_MAX << ")\n";
    } else {
        std::cout << "OVERFLOW! (" << exactPaiseTotal << " > INT_MAX " << INT_MAX << ")\n";
        std::cout << "    [Note: In ISO C++, signed overflow is Undefined Behaviour.\n";
        std::cout << "     Two's complement wrap would yield: "
                  << static_cast<int32_t>(static_cast<uint32_t>(exactPaiseTotal)) << "]\n";
    }
    std::cout << '\n';
}

int main() {
    std::cout << "=== Task 10: Scale Simulation - Expectations vs Execution ===\n\n";

    runSimulation(20000);   // 1 station
    runSimulation(500000);  // 1 line
    runSimulation(3000000); // Whole network

    return 0;
}
