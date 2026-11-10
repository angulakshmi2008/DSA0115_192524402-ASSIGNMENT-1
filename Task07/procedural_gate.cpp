// Procedural version: global variables + free functions
#include <iostream>

long long balance = 10000; // global: any function can change it
const long long BASE[] = {1000, 2000, 3000};
const long long MIN_BAL = 5000;

long long fareOf(int zone, bool peak)
{
    long long f = BASE[zone];
    if (peak) f += f * 20 / 100;
    return f;
}

bool canTravel(long long f) { return balance - f >= MIN_BAL; }

void deduct(long long f)
{
    balance -= f; // nothing stops a caller skipping canTravel()
}

int main()
{
    int zones[] = {0, 2, 1};
    bool peaks[] = {false, true, false};
    for (int i = 0; i < 3; ++i)
    {
        long long f = fareOf(zones[i], peaks[i]);
        if (canTravel(f))
        {
            deduct(f);
            std::cout << "paid " << f << " balance " << balance << '\n';
        }
        else
            std::cout << "denied balance " << balance << '\n';
    }

    // Careless call: caller forgets canTravel()
    deduct(9999); // legal call -> negative balance
    std::cout << "after careless deduct: " << balance << '\n';

    return 0;
}
