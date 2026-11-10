/**
 * Task03 - Floating-Point Rounding Error Measurement
 * Compares double rupees accumulation with exact integer-paise accumulation
 * across scale: n = 10, 1,000, 100,000, 1,000,000, 3,000,000, 10,000,000
 */

#include <iostream>
#include <iomanip>
#include <cmath>

void testScale(long long n, double fareRupees, long long farePaise) {
    long long exactPaise = 0;
    double doubleRupees = 0.0;

    for (long long i = 0; i < n; ++i) {
        exactPaise += farePaise;
        doubleRupees += fareRupees;
    }

    double exactRupees = static_cast<double>(exactPaise) / 100.0;
    double errorRupees = doubleRupees - exactRupees;

    std::cout << std::setw(12) << n << " | "
              << std::setw(16) << std::fixed << std::setprecision(2) << exactRupees << " | "
              << std::setw(16) << doubleRupees << " | "
              << std::setw(18) << std::scientific << std::setprecision(6) << errorRupees << '\n';
}

int main() {
    std::cout << "=== Measurement of Floating-Point Drift Across n Additions ===\n";
    std::cout << "Testing fare: Rs 10.80 (1080 paise - e.g. discounted rate with surcharge)\n\n";
    std::cout << std::setw(12) << "n Journeys" << " | "
              << std::setw(16) << "Exact Total (Rs)" << " | "
              << std::setw(16) << "Double Total (Rs)" << " | "
              << std::setw(18) << "Absolute Drift (Rs)" << '\n';
    std::cout << "-------------+------------------+------------------+--------------------\n";

    long long counts[] = {10, 1000, 100000, 1000000, 3000000, 10000000};
    for (long long n : counts) {
        testScale(n, 10.80, 1080);
    }

    std::cout << "\nConclusion: In integer paise, the error is always identically ZERO.\n";
    std::cout << "In double rupees, rounding error accumulates continuously with n.\n";

    return 0;
}
