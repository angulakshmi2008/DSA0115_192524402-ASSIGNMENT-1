# Task 07: From Procedural to Object-Oriented

## Bloom’s Taxonomy Level: K5 (Evaluate) | Total Marks: 4

---

## 1. Architectural Comparison of Both Styles

| Dimension | Purely Procedural Implementation | Object-Oriented Implementation |
| :--- | :--- | :--- |
| **Balance Location** | Global variable `long long balance` accessible to every function. | `private` member variable `balance_` encapsulated inside `SmartCard`. |
| **Rule Enforcement** | **Caller-dependent**: The caller must explicitly call `canTravel()` before `deduct()`. | **Self-enforcing**: `SmartCard::deduct()` executes `canTravel()` internally before deducting. |
| **Careless Call Hazard (`deduct(9999)`)** | Deducts unchecked $\rightarrow$ **Balance becomes $-4599$** (Illegal negative balance bug reproduced!). | Rejected $\rightarrow$ **Returns `false`**, balance remains safely protected at $5400$ paise. |
| **Changing the Minimum Balance** | Modifies the constant, but **every function** touching `balance` across the application must be re-audited. | Modifies `MIN_BAL` in `SmartCard`. Only the class member functions have access. |
| **Code Size & Complexity** | Marginally fewer lines of setup. | Structured class with constructor and guarded accessors. |
| **Runtime Performance** | $\mathcal{O}(1)$ time per operation. | $\mathcal{O}(1)$ time per operation. |
| **Memory Footprint** | $\mathcal{O}(1)$ (8 bytes). | $\mathcal{O}(1)$ (8 bytes for `SmartCard`). |

---

## 2. How the `SmartCard` Class Prevents Negative Balances

In `oo_smartcard.cpp`, `balance_` is declared under the `private:` access specifier:
```cpp
class SmartCard {
    long long balance_; // Private state
public:
    bool deduct(long long f) {
        if (!canTravel(f)) return false; // Self-enforced invariant
        balance_ -= f;
        return true;
    }
};
```
1. No external code (such as gate loops, third-party station plugins, or operator consoles) can directly assign or subtract from `balance_`.
2. The only mutation path is through `deduct(f)`, which tests `balance_ - f >= MIN_BAL` ($5000$ paise).
3. If an attempted deduction leaves less than Rs 50.00, `deduct()` aborts immediately and returns `false`, guaranteeing that balance never drops below Rs 50.00, and therefore can **never be negative**.

---

## 3. Alternative Concept: Public Data Member Flaw

In `public_balance_flaw.cpp`, the balance was made a public data member:
```cpp
struct ExposedSmartCard { long long balance; };
```
* Even though the runtime performance and memory footprint remain $\mathcal{O}(1)$ (8 bytes), any statement anywhere in the code can write:
  ```cpp
  card.balance -= 9999; // Bypasses Rs 50 minimum reserve check
  card.balance = -500;  // Directly forces negative balance!
  ```
* This proves that encapsulation is not an academic formality—it is a critical structural defense against financial loss and data corruption.
