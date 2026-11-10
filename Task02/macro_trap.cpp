/**
 * Task02 - Macro Precedence Trap & enum class Demonstration
 * Compiles under: g++ -std=c++17 -Wall -Wextra -pedantic
 */

#include <iostream>
#include "fare_constants.h"

// Macro definition without parentheses - classic preprocessor hazard
#define MACRO_FARE_INNER 500 + 500

// Safe constexpr equivalent
constexpr long long CONSTEXPR_FARE_INNER = 500 + 500;

// Plain unscoped enum for comparison
enum OldZone {
    OldInner,
    OldMiddle,
    OldOuter
};

int main() {
    std::cout << "=== Task 02: Macro Trap & Constants Evaluation ===\n\n";

    // 1. Operator precedence hazard with #define
    // MACRO_FARE_INNER * 2 expands textually to: 500 + 500 * 2 = 500 + 1000 = 1500 (WRONG!)
    long long resultMacro = MACRO_FARE_INNER * 2;
    // CONSTEXPR_FARE_INNER * 2 evaluates as (1000) * 2 = 2000 (CORRECT)
    long long resultConstexpr = CONSTEXPR_FARE_INNER * 2;

    std::cout << "Macro expansion (500 + 500 * 2)       : " << resultMacro << " paise (EXPECTED 2000)\n";
    std::cout << "Constexpr evaluation ((500 + 500) * 2) : " << resultConstexpr << " paise (CORRECT)\n\n";

    // 2. Type safety and scoping: Plain enum vs enum class
    std::cout << "sizeof(Zone::Inner) [enum class]      : " << sizeof(Zone::Inner) << " bytes\n";
    std::cout << "sizeof(OldZone::OldInner) [plain enum]: " << sizeof(OldZone::OldInner) << " bytes\n\n";

    // Plain enum silently promotes to integer arithmetic (dangerous!)
    int plainArithmetic = OldOuter + 5;
    std::cout << "Plain enum allows silent int arithmetic: OldOuter + 5 = " << plainArithmetic << '\n';

    // enum class prevents implicit conversions:
    // int x = Zone::Outer; // COMPILER ERROR: cannot convert 'Zone' to 'int' in initialization
    std::cout << "enum class Zone::Inner requires explicit scope and static_cast to convert to int.\n";
    std::cout << "static_cast<int>(Zone::Outer): " << static_cast<int>(Zone::Outer) << '\n';

    return 0;
}
