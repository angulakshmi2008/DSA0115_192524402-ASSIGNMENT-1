/**
 * Task01 - Object-Oriented Analysis & Model Demonstration
 * Course: DSA01 - Object-Oriented Programming with C++ (CO1)
 * 
 * Demonstrates the core object model for the Namma Metro Smart-Card System:
 * - Zone (enum class)
 * - FareTable (encapsulating fare matrices and constants)
 * - Journey (capturing travel context: zone, entry time)
 * - Passenger (category and concession status)
 * - SmartCard (encapsulating stored balance in paise and enforcing invariants)
 *
 * Compiles under: g++ -std=c++17 -Wall -Wextra -pedantic
 */

#include <iostream>
#include <iomanip>
#include <string>

// Enum class restricts zone values to valid domain set
enum class Zone {
    Inner,
    Middle,
    Outer
};

// Passenger categories
enum class Category {
    Regular,
    Student,
    Senior
};

// Journey encapsulates the destination zone and entry time
class Journey {
private:
    Zone farthestZone_;
    int hour_;
    int minute_;

public:
    Journey(Zone zone, int hour, int minute)
        : farthestZone_(zone), hour_(hour), minute_(minute) {}

    Zone getFarthestZone() const { return farthestZone_; }
    int getHour() const { return hour_; }
    int getMinute() const { return minute_; }

    // Evaluates whether travel occurs within peak hours:
    // [08:00, 10:00) or [17:00, 20:00)
    bool isPeak() const {
        int totalMinutes = hour_ * 60 + minute_;
        return (totalMinutes >= 8 * 60 && totalMinutes < 10 * 60) ||
               (totalMinutes >= 17 * 60 && totalMinutes < 20 * 60);
    }
};

// FareTable encapsulates fare rules, constants, and computation
class FareTable {
public:
    static constexpr long long BASE_PAISE[] = {1000, 2000, 3000}; // Rs 10, Rs 20, Rs 30
    static constexpr int PEAK_SURCHARGE_PERCENT = 20;
    static constexpr int CONCESSION_PERCENT = 50;
    static constexpr long long MIN_BALANCE_PAISE = 5000;          // Rs 50.00

    static long long calculateFare(const Journey& journey, bool hasConcession) {
        int zoneIndex = static_cast<int>(journey.getFarthestZone());
        long long fare = BASE_PAISE[zoneIndex];

        // Apply concession if passenger is eligible (Assumption A1: 50% of base fare)
        if (hasConcession) {
            fare -= (fare * CONCESSION_PERCENT + 50) / 100;
        }

        // Apply peak surcharge if journey starts during peak hours
        if (journey.isPeak()) {
            fare += (fare * PEAK_SURCHARGE_PERCENT + 50) / 100;
        }

        return fare;
    }
};

// Passenger represents user identity and concession eligibility
class Passenger {
private:
    std::string name_;
    Category category_;

public:
    Passenger(std::string name, Category cat)
        : name_(std::move(name)), category_(cat) {}

    const std::string& getName() const { return name_; }
    Category getCategory() const { return category_; }
    bool isConcession() const {
        return category_ == Category::Student || category_ == Category::Senior;
    }
};

// SmartCard owns and encapsulates balance, enforcing the Rs 50 minimum rule
class SmartCard {
private:
    long long balancePaise_; // All monetary values stored as integer paise

public:
    explicit SmartCard(long long initialPaise)
        : balancePaise_(initialPaise) {}

    long long getBalancePaise() const { return balancePaise_; }

    // R1 Invariant: Balance remaining after paying fare must be >= 5000 paise
    bool canTravel(long long farePaise) const {
        return farePaise >= 0 && (balancePaise_ - farePaise >= FareTable::MIN_BALANCE_PAISE);
    }

    // Deducts fare safely; returns false and leaves balance unchanged if rule fails
    bool deduct(long long farePaise) {
        if (!canTravel(farePaise)) {
            return false;
        }
        balancePaise_ -= farePaise;
        return true;
    }

    // Accepts only strictly positive recharges
    bool recharge(long long amountPaise) {
        if (amountPaise <= 0) {
            return false;
        }
        balancePaise_ += amountPaise;
        return true;
    }
};

// Helper function to format paise into Rupees for presentation
void printMoney(long long paise) {
    std::cout << "Rs " << (paise / 100) << '.'
              << std::setw(2) << std::setfill('0') << (paise % 100)
              << std::setfill(' ');
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "  Task 01: Namma Metro Smart-Card Object Model Preview  \n";
    std::cout << "========================================================\n\n";

    // Instantiate Passenger and SmartCard
    Passenger student("Aarav", Category::Student);
    SmartCard card(10000); // 10,000 paise = Rs 100.00

    std::cout << "Passenger: " << student.getName()
              << " | Concession: " << (student.isConcession() ? "Eligible (50%)" : "None") << '\n';
    std::cout << "Initial Card Balance: ";
    printMoney(card.getBalancePaise());
    std::cout << "\n\n";

    // Test Trip 1: Outer zone during peak hour (09:00 AM)
    Journey trip1(Zone::Outer, 9, 0);
    long long fare1 = FareTable::calculateFare(trip1, student.isConcession());

    std::cout << "Trip 1: Outer Zone at 09:00 (Peak AM, Concession Applied)\n";
    std::cout << "  Calculated Fare: ";
    printMoney(fare1);
    std::cout << '\n';

    if (card.deduct(fare1)) {
        std::cout << "  Gate Decision: ALLOWED | Remaining Balance: ";
        printMoney(card.getBalancePaise());
        std::cout << '\n';
    } else {
        std::cout << "  Gate Decision: DENIED (Low Balance)\n";
    }
    std::cout << '\n';

    // Test Trip 2: Outer zone off-peak (12:00 PM)
    Journey trip2(Zone::Outer, 12, 0);
    Passenger regularUser("Priya", Category::Regular);
    long long fare2 = FareTable::calculateFare(trip2, regularUser.isConcession());

    std::cout << "Trip 2: Outer Zone at 12:00 (Off-Peak, Regular Passenger)\n";
    std::cout << "  Calculated Fare: ";
    printMoney(fare2);
    std::cout << '\n';

    if (card.deduct(fare2)) {
        std::cout << "  Gate Decision: ALLOWED | Remaining Balance: ";
        printMoney(card.getBalancePaise());
        std::cout << '\n';
    } else {
        std::cout << "  Gate Decision: DENIED (Insufficient balance)\n";
    }
    std::cout << '\n';

    // Test Trip 3: Middle zone off-peak (14:00 PM) - Demonstrates R1 invariant protection
    // Current balance is Rs 52.00 (5200 paise). Middle fare is Rs 20.00 (2000 paise).
    // Remaining balance would be Rs 32.00 (3200 paise), which is < Rs 50.00 (5000 paise).
    Journey trip3(Zone::Middle, 14, 0);
    long long fare3 = FareTable::calculateFare(trip3, regularUser.isConcession());

    std::cout << "Trip 3: Middle Zone at 14:00 (Off-Peak, Regular Passenger)\n";
    std::cout << "  Calculated Fare: ";
    printMoney(fare3);
    std::cout << '\n';

    if (card.deduct(fare3)) {
        std::cout << "  Gate Decision: ALLOWED | Remaining Balance: ";
        printMoney(card.getBalancePaise());
        std::cout << '\n';
    } else {
        std::cout << "  Gate Decision: DENIED (Insufficient balance to maintain Rs 50 reserve)\n";
        std::cout << "  Balance remains safely protected at: ";
        printMoney(card.getBalancePaise());
        std::cout << '\n';
    }

    return 0;
}
