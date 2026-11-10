/**
 * Task05 - Alternative Concept: Lookup Table vs. Boolean Expression
 * Compiles under: g++ -std=c++17 -Wall -Wextra -pedantic
 */

#include <iostream>
#include <iomanip>
#include <chrono>

// Approach A: Logical range expression
inline bool isPeakExpression(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

// Approach B: 24-entry lookup table
class PeakTable {
private:
    bool table_[24];

public:
    PeakTable() {
        for (int h = 0; h < 24; ++h) {
            table_[h] = (h >= 8 && h < 10) || (h >= 17 && h < 20);
        }
    }

    bool isPeak(int hour) const {
        return (hour >= 0 && hour < 24) ? table_[hour] : false;
    }
};

int main() {
    std::cout << "=== Task 05: Lookup Table vs. Boolean Expression ===\n\n";

    PeakTable peakTable;

    std::cout << "Hour | Expression Result | Table Result\n";
    std::cout << "-----+-------------------+-------------\n";
    int testHours[] = {7, 8, 9, 10, 16, 17, 19, 20};
    for (int h : testHours) {
        std::cout << std::setw(4) << h << " | "
                  << std::setw(17) << (isPeakExpression(h) ? "PEAK" : "OFF-PEAK") << " | "
                  << std::setw(11) << (peakTable.isPeak(h) ? "PEAK" : "OFF-PEAK") << '\n';
    }

    std::cout << "\nComparison Analysis:\n";
    std::cout << "- Time Complexity: O(1) for both.\n";
    std::cout << "- Memory: Expression uses 0 extra bytes; Table uses 24 bytes (sizeof(bool[24]) = "
              << sizeof(bool[24]) << " bytes).\n";
    std::cout << "- Granularity: Expression naturally extends to minutes (hour*60 + minute), while hourly table cannot represent 08:30 without 1440 entries.\n";

    return 0;
}
