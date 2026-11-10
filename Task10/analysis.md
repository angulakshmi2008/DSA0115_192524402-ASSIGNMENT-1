# Task 10: Compare Expectations with Execution

## Bloom’s Taxonomy Level: K5 (Evaluate) | Total Marks: 4

---

## 1. Multi-Scale Workload Benchmark

A deterministic, reproducible workload simulation was executed across three operational scales:
1. **Station Scale**: $20,000$ journeys/day.
2. **Line Scale**: $500,000$ journeys/day.
3. **Whole Network Scale**: $3,000,000$ journeys/day.

### Measured Execution Results

| Journeys ($N$) | Exact Total (`long long` paise) | `double` (Rupees $\times 100$) | Deviation | `float` (Rupees) | Float Deviation (Rs) | 32-bit `int` Capacity Status |
| :---: | :---: | :---: | :---: | :---: | :---: | :--- |
| **20,000** | $37,624,700$ | $37,624,700$ | $0.00 \text{ p}$ | Rs $376,247.00$ | Rs $0.00$ | **FITS SAFELY** ($3.76 \times 10^7 \le \text{INT\_MAX}$) |
| **500,000** | $937,116,000$ | $937,116,000$ | $0.00 \text{ p}$ | Rs $9,371,160.00$ | Rs $0.00$ | **FITS SAFELY** ($9.37 \times 10^8 \le \text{INT\_MAX}$) |
| **3,000,000** | $\mathbf{5,624,397,900}$ | $\mathbf{5,624,397,900}$ | $0.00 \text{ p}$ | Rs $56,192,716.00$ | $\mathbf{-51,263.00 \text{ Rs}}$ | $\mathbf{OVERFLOW!}$ ($5.62 \times 10^9 > \text{INT\_MAX}$) |

---

## 2. Technical Evaluation of Accumulator Discrepancies

### A. Why `double` Agreed at 3,000,000 Journeys
* IEEE-754 double precision uses a **53-bit significand** (including the implicit leading bit). It can represent integers up to $2^{53} = 9,007,199,254,740,992$ ($\approx 9.0 \times 10^{15}$) with absolute mathematical exactness.
* Because the network sum ($\approx 5.62 \times 10^9$ paise, or Rs $5.62 \times 10^7$) is well below $2^{53}$, and each fare was an exact whole-cent rupee multiple ($0.50$ or $0.20$), no intermediate rounding occurred for this specific workload. 
* *Caution*: As proved in Task 03, non-dyadic fractional rates (like Rs 10.80) quickly cause `double` to drift.

### B. Why `float` Lost Rs 51,263.00 at 3,000,000 Journeys
* IEEE-754 single-precision `float` provides only a **24-bit significand** ($\approx 7$ decimal digits of precision).
* Exact integers are supported only up to $2^{24} = 16,777,216$ ($\approx \text{Rs } 1.67 \times 10^7$).
* Once the running total passes Rs 16.7 million, the spacing between consecutive representable floating-point numbers ($\text{ULP}$) becomes $2.0$ or $4.0$. Adding small increments (such as Rs $12.00$ or Rs $18.00$) results in truncation of lower-order bits. Over millions of journeys, this truncation caused a cumulative shortfall of **Rs 51,263.00**!

### C. Safe Analysis of 32-bit `int` Overflow
* **Standard C++ Rule**: In ISO C++, **signed integer overflow is Undefined Behaviour (UB)** ([expr.pre] / [basic.fundamental]).
* Unlike unsigned integers (which are guaranteed to wrap modulo $2^{32}$), the C++ compiler's optimizer assumes signed integer overflow never occurs. An optimizing compiler can legally eliminate safety checks or miscompile loops that rely on signed wrapping.
* Mathematically:
  $$5,624,397,900 > 2,147,483,647 \ (\text{INT\_MAX})$$
  The network daily revenue exceeds `INT_MAX` after approximately $1.14$ million journeys. If truncated to a 32-bit two's-complement bitmask, it would wrap to $1,329,430,604$, corrupting the audit trail.
* **Resolution**: Accumulating money in `long long` paise guarantees at least 64 bits of storage, supporting totals up to $9.22 \times 10^{18}$ paise ($\approx 4.9 \times 10^{15}$ journeys) with standard-compliant, portable safety.

---

## 3. Alternative Concept: Time and Space Comparison

| Type | Size | Time per Addition | Network Capacity (3M Journeys) | Failure Mode |
| :--- | :--- | :--- | :--- | :--- |
| **`int` (paise)** | 4 bytes | $\mathcal{O}(1)$ | Fails after $\approx 1.14 \text{ M}$ journeys | **Undefined Behaviour / Overflow** |
| **`long long` (paise)** | 8 bytes | $\mathcal{O}(1)$ | **Safe** (capacity $\approx 4.9 \times 10^{15}$ trips) | None (Standard C++ guarantee) |
| **`double` (rupees)** | 8 bytes | $\mathcal{O}(1)$ | Exact up to $2^{53}$ for whole units | **Representation drift for decimal fractions** |
| **`float` (rupees)** | 4 bytes | $\mathcal{O}(1)$ | **Fails** above Rs $1.67 \times 10^7$ | **Precision Loss (Lost Rs 51,263.00)** |
