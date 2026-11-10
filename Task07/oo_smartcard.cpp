// Object-oriented version: SmartCard keeps balance private
#include <iostream>

class FareTable
{
    static constexpr long long BASE[3] = {1000, 2000, 3000};

public:
    static long long fare(int zone, bool peak)
    {
        long long f = BASE[zone];
        if (peak) f += f * 20 / 100;
        return f;
    }
};

class SmartCard
{
    long long balance_; // private: cannot be accessed directly from outside
    static constexpr long long MIN_BAL = 5000;

public:
    explicit SmartCard(long long b) : balance_(b) {}
    long long balance() const { return balance_; }
    bool canTravel(long long f) const { return balance_ - f >= MIN_BAL; }
    
    // Encapsulated deduction enforces invariant inside the method
    bool deduct(long long f)
    {
        if (!canTravel(f)) return false;
        balance_ -= f;
        return true;
    }
};

int main()
{
    SmartCard card(10000);
    int zones[] = {0, 2, 1};
    bool peaks[] = {false, true, false};
    for (int i = 0; i < 3; ++i)
    {
        long long f = FareTable::fare(zones[i], peaks[i]);
        if (card.deduct(f))
            std::cout << "paid " << f << " balance " << card.balance() << '\n';
        else
            std::cout << "denied balance " << card.balance() << '\n';
    }

    // Careless deduct attempt is rejected by the class invariant!
    std::cout << "careless deduct accepted? " << std::boolalpha << card.deduct(9999)
              << ", balance " << card.balance() << '\n';

    return 0;
}
