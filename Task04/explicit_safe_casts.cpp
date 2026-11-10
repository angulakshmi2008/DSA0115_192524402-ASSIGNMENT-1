/**
 * Task04 - Explicit Casts and Brace Initialization Protection
 * Compiles cleanly under: g++ -std=c++17 -Wall -Wextra -pedantic
 */

#include <iostream>

int main()
{
    // 1. Explicit floating-point division using static_cast
    int a = 7, b = 2;
    double exactDiv = static_cast<double>(a) / b;
    std::cout << "Explicit division: " << exactDiv << '\n';

    // 2. Safe character printing without unintended integer promotion
    char ch = 'A' + 2;
    std::cout << "Character preserved: " << ch << ' ' << static_cast<char>(ch + 1) << '\n';

    // 3. Explicit boolean initialization
    bool flag = (5 != 0);
    std::cout << "Explicit bool: " << std::boolalpha << flag << '\n';

    // 4. Safe signed and unsigned comparison
    // Promoting both to signed long long avoids hazardous modular conversion
    unsigned int u = 1;
    int s = -1;
    bool isLess = static_cast<long long>(s) < static_cast<long long>(u);
    std::cout << "Safe signed vs unsigned comparison (-1 < 1): "
              << (isLess ? "s < u (CORRECT)" : "s >= u (BUGGY)") << '\n';

    // 5. Narrowing conversion prevention with brace initialization:
    // C++17 brace initialization prevents narrowing at compile time.
    // Example:
    // int safeInt{3};       // OK
    // int dangerousInt{3.7}; // COMPILE ERROR: narrowing conversion of '3.7' from 'double' to 'int'
    // bool safeBool{true};   // OK
    // bool dangerousBool{5}; // COMPILE ERROR: narrowing conversion of '5' from 'int' to 'bool'
    int safeInt{3};
    std::cout << "Brace initialized integer: " << safeInt << '\n';

    return 0;
}
