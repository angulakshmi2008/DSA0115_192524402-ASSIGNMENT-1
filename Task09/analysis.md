# Task 09: Implement and Test the Fare Logic

## Bloom’s Taxonomy Level: K5 (Evaluate) | Total Marks: 4

---

## 1. Unified Core Engine Architecture (`core.h`)

The core fare calculation engine was centralized into `core.h`, implementing:
1. `Zone` enum class: `Inner`, `Middle`, `Outer`.
2. `isPeak(h, m)`: Half-open interval evaluator $[08:00, 10:00) \cup [17:00, 20:00)$ using scaled minutes ($h \times 60 + m$).
3. `fare(z, peak, concession)`: Integer percentage fare calculation with half-up rounding.
4. `canTravel(balance, f)`: Strict invariant enforcement ($\text{balance} - f \ge 5000$).

---

## 2. Test Verification Matrix (14 Table-Driven Cases)

| Test ID | Case Description | Zone | Time | Concession | Hand-Calculated Expected (paise) | Program Result (paise) | Status |
| :---: | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **1** | Inner off-peak | Inner | 12:00 | No | 1,000 (Rs 10.00) | 1,000 | **PASS** |
| **2** | Middle off-peak | Middle | 12:00 | No | 2,000 (Rs 20.00) | 2,000 | **PASS** |
| **3** | Outer off-peak | Outer | 12:00 | No | 3,000 (Rs 30.00) | 3,000 | **PASS** |
| **4** | Inner peak AM | Inner | 09:00 | No | $1000 + 200 = 1,200$ | 1,200 | **PASS** |
| **5** | Middle peak PM | Middle | 18:00 | No | $2000 + 400 = 2,400$ | 2,400 | **PASS** |
| **6** | Outer peak AM | Outer | 08:30 | No | $3000 + 600 = 3,600$ | 3,600 | **PASS** |
| **7** | Boundary 08:00 | Inner | 08:00 | No | $1000 + 200 = 1,200$ (Peak) | 1,200 | **PASS** |
| **8** | Boundary 09:59 | Inner | 09:59 | No | $1000 + 200 = 1,200$ (Peak) | 1,200 | **PASS** |
| **9** | Boundary 10:00 | Inner | 10:00 | No | $1,000$ (Off-peak) | 1,000 | **PASS** |
| **10** | Boundary 17:00 | Inner | 17:00 | No | $1000 + 200 = 1,200$ (Peak) | 1,200 | **PASS** |
| **11** | Boundary 19:59 | Inner | 19:59 | No | $1000 + 200 = 1,200$ (Peak) | 1,200 | **PASS** |
| **12** | Boundary 20:00 | Inner | 20:00 | No | $1,000$ (Off-peak) | 1,000 | **PASS** |
| **13** | Concession off-peak | Outer | 12:00 | Yes (50%) | $3000 - 1500 = 1,500$ | 1,500 | **PASS** |
| **14** | Concession peak AM | Outer | 09:00 | Yes (50%) | $1500 + 300 = 1,800$ | 1,800 | **PASS** |

---

## 3. Boundary & Invariant Validation

### A. Balance Leaving Exactly Rs 50 (5,000 paise)
* **Balance 6,000 paise, Fare 1,000 paise**: Leaves exactly $5,000$ paise $\rightarrow$ **ALLOWED (PASS)**.
* **Balance 5,999 paise, Fare 1,000 paise**: Leaves $4,999$ paise ($< 5,000$) $\rightarrow$ **DENIED (PASS)**.
* **Balance 6,000 paise, Fare 1,001 paise**: Leaves $4,999$ paise ($< 5,000$) $\rightarrow$ **DENIED (PASS)**.

### B. Invalid Input Handling
* Out-of-range Zone `7` $\rightarrow$ Rejected (`PASS`).
* Negative recharge amount `$-200$` $\rightarrow$ Rejected (`PASS`).
* Alphabetic hour string `"ab"` $\rightarrow$ Rejected by stream parser (`PASS`).
* Out-of-range hour `"25"` $\rightarrow$ Rejected by validation (`PASS`).

---

## 4. Five-Journey Hand-Calculated Verification

| Journey | Zone Travelled | Entry Time | Window Type | Calculation Breakdown | Fare (paise) |
| :---: | :---: | :---: | :---: | :--- | :---: |
| **1** | Inner | 12:00 | Off-peak | Base fare | 1,000 |
| **2** | Middle | 09:00 | AM Peak | $2000 + 20\% (400)$ | 2,400 |
| **3** | Outer | 18:30 | PM Peak | $3000 + 20\% (600)$ | 3,600 |
| **4** | Inner | 10:00 | Off-peak boundary | Base fare (10:00 is off-peak) | 1,000 |
| **5** | Outer | 20:00 | Off-peak boundary | Base fare (20:00 is off-peak) | 3,000 |
| **TOTAL** | — | — | — | $1000 + 2400 + 3600 + 1000 + 3000$ | **11,000 paise (Rs 110.00)** |

* **Program Computed Total**: `11000` paise.
* **Verification Status**: **100% Match (PASS)**.

---

## 5. Alternative Concept: Table-Driven Test Suite vs. Hardcoded `if` Blocks

* **Time Complexity**: For $t$ test cases, execution takes $\mathcal{O}(t)$ time in both styles. In our test suite, all 14 cases executed in under $1 \text{ ms}$.
* **Space Complexity**: The `Case cases[]` array consumes $\mathcal{O}(t)$ memory ($14 \times 32 \text{ bytes} \approx 448 \text{ bytes}$).
* **Maintainability & Extensibility**: In hardcoded `if` statements, adding a new boundary test requires writing new procedural branches and assertion blocks. In table-driven testing, adding a test is a **single declarative row** added to the `Case` array, decoupling test data from test execution logic.
