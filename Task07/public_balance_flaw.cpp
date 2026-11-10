// Alternative: balance as a PUBLIC data member
#include <iostream>

struct ExposedSmartCard {
    long long balance;
};

int main()
{
    ExposedSmartCard card{10000};

    // Public member allows arbitrary external corruption
    card.balance -= 9999;      // compiles; bypasses Rs 50 rule!
    card.balance = -500;       // compiles; introduces illegal negative balance!

    std::cout << "Corrupted public balance: " << card.balance << " paise\n";

    return 0;
}
