# Task 06: Design the Control Structures

## Bloom’s Taxonomy Level: K5 (Evaluate) | Total Marks: 4

---

## 1. Selection Criteria for Control Structures

| Control Structure | Recommended Context in Fare System | Engineering Justification |
| :--- | :--- | :--- |
| **`switch` on `Zone`** | Base fare lookup by zone (`Inner`, `Middle`, `Outer`). | Evaluates a single integral/enum expression against discrete case constants. The compiler generates efficient jump tables and emits `-Wswitch` warnings if an enum value is unhandled. |
| **`if-else`** | Peak hour intervals, concession checks, balance allowance. | Required for range checks ($8 \le h < 10$) and compound boolean conditions ($A \land B \lor C$), which cannot be expressed in standard C++ `switch` case labels. |
| **Conditional Operator (`?:`)** | Inline ternary value selection (e.g., UI display strings). | Compact two-way expression returning a value (e.g., `isPeak ? "PEAK" : "OFF"`). Should be avoided for complex statement execution. |
| **Range-based `for` Loop** | Looping through pre-loaded journey records (`for (const Trip& t : trips)`). | Cleanest, safest loop. Guarantees bounds safety, eliminates off-by-one indexing bugs, and avoids unnecessary object copying by using `const &`. |
| **`while` Loop** | Streaming interactive input from the gate (`while (cin >> zone >> h >> m)`). | Best suited when the stream length or journey count is determined dynamically by user input or end-of-file. |

### Role of `break` and `continue`
* **`break`**: Terminates a `switch` case branch, preventing unintended fallthrough to subsequent cases. Also exits loops on fatal gate errors.
* **`continue`**: Skips the remainder of the current loop iteration when an invalid record (e.g. unknown zone or malformed hour) is detected, allowing subsequent valid journeys to be processed without halting gate operations.

### Hazard of an Unhandled `switch` Case
If a `switch` lacks a matching `case` and has no `default:`, execution skips the entire switch body. If the base fare variable was initialized to `0`, an unhandled zone (e.g. newly introduced `Airport`) would result in a **free ride (Rs 0.00)**. Hence, defensive code must include a `default:` handler or validate inputs prior to switching.

---

## 2. SIMATS Engineering Algorithmic Pseudocode

```text
ALGORITHM ComputeFare
INPUT  : z (Zone: Inner | Middle | Outer), peak (Boolean)
OUTPUT : fare in integer paise
CONSTANTS: BASE_PAISE = [1000, 2000, 3000], PEAK_SURCHARGE_PERCENT = 20

BEGIN
    base <- 0
    SWITCH z DO
        CASE Zone::Inner  : base <- BASE_PAISE[0]
        CASE Zone::Middle : base <- BASE_PAISE[1]
        CASE Zone::Outer  : base <- BASE_PAISE[2]
        DEFAULT           : RETURN -1   // Invalid zone
    END SWITCH

    IF peak = TRUE THEN
        base <- base + (base * PEAK_SURCHARGE_PERCENT) / 100
    END IF

    RETURN base
END

ALGORITHM CanTravel
INPUT  : balance (integer paise), fare (integer paise)
OUTPUT : Boolean (TRUE / FALSE)
CONSTANTS: MIN_BALANCE_PAISE = 5000

BEGIN
    RETURN (balance - fare) >= MIN_BALANCE_PAISE
END

ALGORITHM ProcessDay
INPUT  : trips (Collection of Trip), startingBalance (integer paise)
OUTPUT : Gate admission decisions and remaining balances

BEGIN
    balance <- startingBalance
    FOR EACH trip IN trips DO
        f <- ComputeFare(trip.zone, trip.peak)
        IF CanTravel(balance, f) THEN
            balance <- balance - f
            PRINT "Fare Rs", f / 100, "balance Rs", balance / 100
        ELSE
            PRINT "Denied (low balance) balance Rs", balance / 100
        END IF
    END FOR
END
```

---

## 3. Complete the Code Verification

The supplied program was reproduced and completed by implementing strictly `TODO 1` and `TODO 2`:

### Comparison with Expected Output
```text
Expected Output:
Fare Rs 10.00 balance Rs 90
Fare Rs 36.00 balance Rs 54
Denied (low balance) balance Rs 54

Actual Executed Output:
Fare Rs 10.00 balance Rs 90
Fare Rs 36.00 balance Rs 54
Denied (low balance) balance Rs 54
```
**Result: Exact 100% Match (PASS)**.

### Boundary Cases Verification
All 8 peak-hour boundaries were empirically validated:
* `08:00 AM` $\rightarrow$ **PEAK** (Inclusive start)
* `09:59 AM` $\rightarrow$ **PEAK** (Inclusive end)
* `10:00 AM` $\rightarrow$ **OFF-PEAK** (Exclusive upper boundary)
* `17:00 PM` $\rightarrow$ **PEAK** (Inclusive start)
* `19:59 PM` $\rightarrow$ **PEAK** (Inclusive end)
* `20:00 PM` $\rightarrow$ **OFF-PEAK** (Exclusive upper boundary)

---

## 4. Alternative Concept: Array Lookup vs. `switch`

```cpp
// Array lookup alternative:
constexpr long long FARE_TABLE[] = {1000, 2000, 3000};
long long base = FARE_TABLE[static_cast<int>(z)];
```

### Analysis & Complexity
* **Time Complexity**: $\mathcal{O}(1)$ for both. The array lookup performs a direct memory offset ($O(1)$); modern compilers optimize the small 3-case `switch` into a jump table or direct comparison sequence ($O(1)$).
* **Space Complexity**: The array occupies 24 bytes in static memory ($3 \times 8$ bytes); the `switch` is encoded directly in machine instructions.
* **Scalability to a 4th Zone (Airport)**:
  * With `switch`: Adding `Airport` requires updating every switch statement in the codebase. If forgotten, `-Wswitch` warns, but the code still compiles and may silently produce a 0 fare.
  * With Array Lookup: Adding `Airport` requires adding one array element (`6000`) and one enum value (`Airport`). Paired with a `static_assert` on `sizeof(FARE_TABLE)`, it scales cleanly with zero code duplication (as adopted in Task 12).
