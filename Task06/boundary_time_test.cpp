/**
 * Task06 - Peak Time Boundary Verification
 * Validates half-open intervals: [08:00, 10:00) and [17:00, 20:00)
 * Compiles under: g++ -std=c++17 -Wall -Wextra -pedantic
 */

#include <iostream>
#include <iomanip>

inline bool isPeak(int hour, int minute) {
    int totalMinutes = hour * 60 + minute;
    return (totalMinutes >= 8 * 60 && totalMinutes < 10 * 60) ||
           (totalMinutes >= 17 * 60 && totalMinutes < 20 * 60);
}

struct BoundaryCase {
    int hour;
    int minute;
    bool expectedPeak;
    const char* description;
};

int main() {
    BoundaryCase cases[] = {
        {7, 59, false, "07:59 AM (Pre-peak window)"},
        {8,  0, true,  "08:00 AM (AM Peak START - inclusive)"},
        {9, 59, true,  "09:59 AM (AM Peak END boundary - inclusive)"},
        {10, 0, false, "10:00 AM (AM Peak END - exclusive, OFF-PEAK)"},
        {16, 59, false, "04:59 PM (Pre-peak window)"},
        {17,  0, true,  "05:00 PM (PM Peak START - inclusive)"},
        {19, 59, true,  "07:59 PM (PM Peak END boundary - inclusive)"},
        {20,  0, false, "08:00 PM (PM Peak END - exclusive, OFF-PEAK)"}
    };

    std::cout << "=== Task 06: Boundary Time Verification ===\n\n";
    std::cout << "Time  | Description                           | Expected | Actual | Status\n";
    std::cout << "------+---------------------------------------+----------+--------+-------\n";

    bool allPassed = true;
    for (const auto& c : cases) {
        bool actual = isPeak(c.hour, c.minute);
        bool match = (actual == c.expectedPeak);
        if (!match) allPassed = false;

        std::cout << std::setfill('0') << std::setw(2) << c.hour << ":"
                  << std::setw(2) << c.minute << " | "
                  << std::setfill(' ')
                  << std::left << std::setw(37) << c.description << " | "
                  << std::setw(8) << (c.expectedPeak ? "PEAK" : "OFF") << " | "
                  << std::setw(6) << (actual ? "PEAK" : "OFF") << " | "
                  << (match ? "PASS" : "FAIL") << '\n';
    }

    std::cout << "\nOverall Boundary Test Status: " << (allPassed ? "ALL TESTS PASSED" : "FAILURES DETECTED") << '\n';
    return 0;
}
