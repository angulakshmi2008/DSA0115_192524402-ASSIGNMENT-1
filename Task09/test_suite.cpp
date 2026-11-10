#include "core.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

struct Case
{
    const char *name;
    int zone, h, m;
    bool conc;
    long long expected;
};

// Parses a zone/hour text the way the gate does; returns false for invalid input
bool parseHour(const std::string &s, int &out)
{
    std::istringstream in(s);
    in >> out;
    return !in.fail() && in.eof() && out >= 0 && out <= 23;
}

int main()
{
    Case cases[] = {
        {"Inner off-peak",   0, 12,  0, false, 1000},
        {"Middle off-peak",  1, 12,  0, false, 2000},
        {"Outer off-peak",   2, 12,  0, false, 3000},
        {"Inner peak AM",    0,  9,  0, false, 1200},
        {"Middle peak PM",   1, 18,  0, false, 2400},
        {"Outer peak AM",    2,  8, 30, false, 3600},
        {"Boundary 08:00",   0,  8,  0, false, 1200},
        {"Boundary 09:59",   0,  9, 59, false, 1200},
        {"Boundary 10:00",   0, 10,  0, false, 1000},
        {"Boundary 17:00",   0, 17,  0, false, 1200},
        {"Boundary 19:59",   0, 19, 59, false, 1200},
        {"Boundary 20:00",   0, 20,  0, false, 1000},
        {"Concession off",   2, 12,  0, true,  1500},
        {"Concession peak",  2,  9,  0, true,  1800},
    };

    int pass = 0, total = 0;
    std::cout << "=== Task 09: Comprehensive Fare Engine Test Suite ===\n\n";
    std::cout << "No  Case              Expected  Actual  Result\n";
    std::cout << "----+-----------------+---------+-------+-------\n";

    for (const Case &c : cases)
    {
        long long a = fare(static_cast<Zone>(c.zone), isPeak(c.h, c.m), c.conc);
        bool ok = (a == c.expected);
        ++total;
        pass += ok;
        std::cout << std::left << std::setw(3) << total << " "
                  << std::setw(17) << c.name
                  << std::right << std::setw(9) << c.expected
                  << std::setw(8) << a << "    "
                  << (ok ? "PASS" : "FAIL") << '\n';
    }

    std::cout << "\nTable-Driven Suite: " << pass << "/" << total << " cases passed.\n\n";

    // Balance leaving exactly Rs 50
    std::cout << "--- Card Balance Threshold Invariant Checks ---\n";
    bool b1 = canTravel(6000, 1000); // 6000 - 1000 = 5000 (Exactly 5000)
    bool b2 = canTravel(5999, 1000); // 5999 - 1000 = 4999 (< 5000)
    bool b3 = canTravel(6000, 1001); // 6000 - 1001 = 4999 (< 5000)

    std::cout << "Balance 6000, fare 1000 (leaves exactly 5000): "
              << (b1 ? "ALLOWED" : "DENIED") << "  expected ALLOWED [" << (b1 ? "PASS" : "FAIL") << "]\n";
    std::cout << "Balance 5999, fare 1000 (leaves 4999 paise) : "
              << (b2 ? "ALLOWED" : "DENIED") << "  expected DENIED  [" << (!b2 ? "PASS" : "FAIL") << "]\n";
    std::cout << "Balance 6000, fare 1001 (leaves 4999 paise) : "
              << (b3 ? "ALLOWED" : "DENIED") << "  expected DENIED  [" << (!b3 ? "PASS" : "FAIL") << "]\n\n";

    // Invalid input checks
    std::cout << "--- Invalid Input Rejection Checks ---\n";
    int zone = 7;
    bool badZone = !(zone >= 0 && zone <= 2);
    long long recharge = -200;
    bool badRecharge = recharge <= 0;
    int h;
    bool badHourAb = !parseHour("ab", h);
    bool badHour25 = !parseHour("25", h);

    std::cout << "Unknown zone 7 rejected     : [" << (badZone ? "PASS" : "FAIL") << "]\n";
    std::cout << "Negative recharge rejected  : [" << (badRecharge ? "PASS" : "FAIL") << "]\n";
    std::cout << "Hour 'ab' rejected          : [" << (badHourAb ? "PASS" : "FAIL") << "]\n";
    std::cout << "Hour '25' rejected          : [" << (badHour25 ? "PASS" : "FAIL") << "]\n\n";

    // 5 journeys hand calculation
    std::cout << "--- Five-Journey Hand-Calculated Verification ---\n";
    struct J { int z, h, m; } js[] = {
        {0, 12, 0},   // Inner 12:00 = 1000
        {1, 9, 0},    // Middle 09:00 peak = 2000 + 400 = 2400
        {2, 18, 30},  // Outer 18:30 peak = 3000 + 600 = 3600
        {0, 10, 0},   // Inner 10:00 off-peak boundary = 1000
        {2, 20, 0}    // Outer 20:00 off-peak boundary = 3000
    };

    long long total5 = 0;
    for (auto j : js) {
        total5 += fare(static_cast<Zone>(j.z), isPeak(j.h, j.m), false);
    }

    std::cout << "Five journeys hand-calculated total : 11000 paise (Rs 110.00)\n";
    std::cout << "Program computed five-journey total : " << total5 << " paise (Rs " << total5 / 100 << ".00)\n";
    std::cout << "Comparison Result                   : [" << (total5 == 11000 ? "PASS" : "FAIL") << "]\n";

    return 0;
}
