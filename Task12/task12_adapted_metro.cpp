/**
 * Task12 - Adapted Metro System (Day-4 Design Challenge)
 * Implements:
 * 1. Fourth Zone: Airport (base Rs 60.00 = 6000 paise)
 * 2. 10% Weekend Discount (replaces peak surcharge on Saturday/Sunday)
 * 3. Monthly Pass (two chosen zones, calendar month, free journeys)
 * 
 * Compiles under: g++ -std=c++17 -Wall -Wextra -pedantic
 */

#include <iostream>
#include <iomanip>
#include <string>

enum class Zone {
    Inner,
    Middle,
    Outer,
    Airport,
    Count
};

constexpr int ZONE_COUNT = static_cast<int>(Zone::Count);
constexpr long long BASE_PAISE[ZONE_COUNT] = {1000, 2000, 3000, 6000}; // Rs 10, 20, 30, 60

constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int WEEKEND_DISCOUNT_PERCENT = 10;
constexpr int CONCESSION_PERCENT = 50;
constexpr long long MIN_BALANCE_PAISE = 5000;

struct Journey {
    Zone zone;
    int hour;
    int minute;
    int weekday; // 0=Mon, 1=Tue, 2=Wed, 3=Thu, 4=Fri, 5=Sat, 6=Sun
    bool concession;
    int year;
    int month;
};

constexpr bool isWeekend(int weekday) {
    return weekday == 5 || weekday == 6; // Saturday or Sunday
}

constexpr bool isPeak(int h, int m) {
    int t = h * 60 + m;
    return (t >= 8 * 60 && t < 10 * 60) || (t >= 17 * 60 && t < 20 * 60);
}

constexpr long long pct(long long amount, int rate) {
    return (amount * rate + 50) / 100; // Half-up integer percentage rounding
}

// Fare calculation engine:
// 1. Base fare from zone array
// 2. Concession (50%) deducted first
// 3. On weekends: 10% weekend discount applies (replaces peak surcharge)
// 4. On weekdays: 20% peak surcharge applies during peak hours
long long fare(const Journey& j) {
    int idx = static_cast<int>(j.zone);
    if (idx < 0 || idx >= ZONE_COUNT) return -1;

    long long f = BASE_PAISE[idx];

    // Concession applies first
    if (j.concession) {
        f -= pct(f, CONCESSION_PERCENT);
    }

    // Weekend discount replaces peak surcharge
    if (isWeekend(j.weekday)) {
        f -= pct(f, WEEKEND_DISCOUNT_PERCENT);
    } else if (isPeak(j.hour, j.minute)) {
        f += pct(f, PEAK_SURCHARGE_PERCENT);
    }

    return f;
}

// Monthly Pass for two selected zones
class Pass {
private:
    unsigned int mask_; // Bitmask for covered zones
    int year_;
    int month_;

public:
    Pass(Zone a, Zone b, int y, int m)
        : mask_((1u << static_cast<int>(a)) | (1u << static_cast<int>(b))),
          year_(y), month_(m) {}

    bool covers(const Journey& j) const {
        if (j.year != year_ || j.month != month_) return false;
        int idx = static_cast<int>(j.zone);
        if (idx < 0 || idx >= ZONE_COUNT) return false;
        return (mask_ >> idx) & 1u;
    }
};

// SmartCard with pass integration
class SmartCard {
private:
    long long balancePaise_;
    const Pass* pass_ = nullptr;

public:
    explicit SmartCard(long long initialPaise)
        : balancePaise_(initialPaise) {}

    void attachPass(const Pass* p) { pass_ = p; }
    long long getBalancePaise() const { return balancePaise_; }

    // Evaluates travel entitlement:
    // If covered by pass: fare is 0, balance is untouched, Rs 50 rule does not apply.
    // If not covered: calculates fare, checks balance - fare >= 5000, and deducts.
    // Returns fare charged, or -1 if denied.
    long long travel(const Journey& j) {
        if (pass_ != nullptr && pass_->covers(j)) {
            return 0; // Unlimited travel in pass zones!
        }

        long long f = fare(j);
        if (f < 0 || (balancePaise_ - f < MIN_BALANCE_PAISE)) {
            return -1; // Denied (low balance)
        }

        balancePaise_ -= f;
        return f;
    }
};

static int failures = 0;
void checkCase(const char* name, long long expected, long long actual) {
    bool ok = (expected == actual);
    if (!ok) ++failures;
    std::cout << std::left << std::setw(45) << name
              << " | Expected: " << std::right << std::setw(5) << expected
              << " | Actual: " << std::setw(5) << actual
              << " | [" << (ok ? "PASS" : "FAIL") << "]\n";
}

int main() {
    std::cout << "=== Task 12: Day-4 Design Challenge Verification ===\n\n";

    // 1. Fare calculation tests across all new dimensions
    checkCase("Airport weekday 12:00 (base fare Rs 60)", 6000,
              fare({Zone::Airport, 12, 0, 1, false, 2026, 11}));

    checkCase("Airport weekday 09:00 (peak +20% = Rs 72)", 7200,
              fare({Zone::Airport, 9, 0, 1, false, 2026, 11}));

    checkCase("Inner Saturday 12:00 (weekend -10% = Rs 9)", 900,
              fare({Zone::Inner, 12, 0, 5, false, 2026, 11}));

    checkCase("Inner Saturday 09:00 (peak window, weekend discount replaces peak)", 900,
              fare({Zone::Inner, 9, 0, 5, false, 2026, 11}));

    checkCase("Outer Sunday 18:00 (weekend -10% = Rs 27)", 2700,
              fare({Zone::Outer, 18, 0, 6, false, 2026, 11}));

    checkCase("Airport Sunday student (concession 3000 then -10%)", 2700,
              fare({Zone::Airport, 12, 0, 6, true, 2026, 11}));

    checkCase("Middle Friday 17:00 senior peak (concession 1000 + 20%)", 1200,
              fare({Zone::Middle, 17, 0, 4, true, 2026, 11}));

    // 2. SmartCard & Monthly Pass integration tests
    std::cout << "\n--- Monthly Pass and SmartCard Integration Tests ---\n";
    Pass passInnerMiddle(Zone::Inner, Zone::Middle, 2026, 11);
    SmartCard cardA(10000); // Rs 100.00
    cardA.attachPass(&passInnerMiddle);

    checkCase("Pass: Middle journey covered (fare 0)", 0,
              cardA.travel({Zone::Middle, 9, 0, 1, false, 2026, 11}));
    checkCase("Pass: Card balance remains unchanged", 10000, cardA.getBalancePaise());

    checkCase("Pass: Outer journey NOT covered (deducts 3600 peak)", 3600,
              cardA.travel({Zone::Outer, 9, 0, 1, false, 2026, 11}));
    checkCase("Pass: Balance after Outer journey", 6400, cardA.getBalancePaise());

    SmartCard cardExpired(10000);
    cardExpired.attachPass(&passInnerMiddle);
    checkCase("Pass: Expired month (Dec 2026) charges normal fare", 2000,
              cardExpired.travel({Zone::Middle, 12, 0, 1, false, 2026, 12}));

    SmartCard cardB(10000); // No pass
    // Airport peak is 7200 paise. 10000 - 7200 = 2800 < 5000 -> Denied (-1)
    checkCase("No Pass: Airport peak denied due to Rs 50 rule", -1,
              cardB.travel({Zone::Airport, 9, 0, 1, false, 2026, 11}));

    std::cout << "\nOverall Task 12 Suite Status: "
              << (failures == 0 ? "ALL 13 TESTS PASSED" : "TEST FAILURES DETECTED") << '\n';

    return 0;
}
