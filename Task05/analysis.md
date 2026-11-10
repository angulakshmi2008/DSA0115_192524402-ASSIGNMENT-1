# Task 05: Write Correct Expressions with the Right Precedence

## Bloom’s Taxonomy Level: K5 (Evaluate) | Total Marks: 4

---

## 1. Prediction vs. Actual Output Comparison

Output predictions were documented in `prediction.txt` prior to execution.

| Code Expression Evaluated | Predicted Output | Actual Output | Status |
| :--- | :--- | :--- | :--- |
| `x + y * 2` and `(x + y) * 2` | `11 16` | `11 16` | **MATCH (PASS)** |
| `int z = x++ + ++y;` then `z, x, y` | `9 6 4` | `9 6 4` | **MATCH (PASS)** |
| `(x > y ? x : y)` and `x % 4 * 2` | `6 4` | `6 4` | **MATCH (PASS)** |
| `10 / 3`, `10 % 3`, `-10 / 3`, `-10 % 3` | `3 1 -3 -1` | `3 1 -3 -1` | **MATCH (PASS)** |
| `for (i = 0; i < 10; i += 3) k += i;` | `18` | `18` | **MATCH (PASS)** |

---

## 2. Line-by-Line Operator Precedence & Evaluation Analysis

| Expression | Operators Involved | Precedence & Associativity Rules Applied | Evaluation Steps | Result |
| :--- | :--- | :--- | :--- | :--- |
| `x + y * 2` | `+`, `*` | `*` has higher precedence than `+`. Evaluated left-to-right. | $5 + (3 \times 2) = 5 + 6$ | **11** |
| `(x + y) * 2` | `( )`, `+`, `*` | Grouping parentheses `( )` have highest precedence. | $(5 + 3) \times 2 = 8 \times 2$ | **16** |
| `z = x++ + ++y;` | `++` (post), `++` (pre), `+`, `=` | 1. Postfix `x++` yields old value 5; then `x` becomes 6.<br>2. Prefix `++y` increments `y` from 3 to 4, yielding 4.<br>3. Binary `+` evaluates $5 + 4 = 9$.<br>4. Assignment `=` stores 9 into `z`. | $z = 5 + 4 = 9$; $x = 6$; $y = 4$ | **9 6 4** |
| `(x > y ? x : y)` | `?:`, `>` | Relational `>` precedes conditional ternary `?:`. | $6 > 4$ is true $\rightarrow$ yields $x$ ($6$). | **6** |
| `x % 4 * 2` | `%`, `*` | Remainder `%` and multiplication `*` share equal precedence and associate **left-to-right**. | $(6 \% 4) \times 2 = 2 \times 2 = 4$ | **4** |
| `10 / 3`, `10 % 3` | `/`, `%` | Integer division truncates toward zero. Remainder takes dividend sign. | $10 / 3 = 3$; $10 \% 3 = 1$ | **3 1** |
| `-10 / 3`, `-10 % 3` | Unary `-`, `/`, `%` | In C++11 and C++17, division truncates toward zero. Remainder sign matches dividend ($-10$). | $-10 / 3 = -3$; $-10 \% 3 = -1$ | **-3 -1** |
| Loop accumulation | `+=`, `<`, `++` | Compound assignment `+=` associates right-to-left. Loops for $i \in \{0, 3, 6, 9\}$. | $k = 0 + 3 + 6 + 9 = 18$ | **18** |

---

## 3. Important C++17 Sequencing & Precedence Rules

### A. Pre-increment vs. Post-increment & The `i = i++` Hazard
* `x++` produces the current value as an rvalue, with the side effect of incrementing the stored value.
* `++x` performs the increment and produces the updated value as an lvalue.
* **Why `i = i++` is a dangerous mistake**: In C++ (prior to C++17), modifying a scalar object more than once between sequence points without intervening read was **Undefined Behaviour**. Even in C++17, writing `i = i++` produces unsequenced side-effects because the assignment and the post-increment side-effect both attempt to write to `i`. The clean, idiomatic statement is simply `++i;` or `i++;`.

### B. Logical Operator Precedence & Readability in Peak Checks
```cpp
// Form A (Explicit Parentheses - Strongly Recommended):
(hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)

// Form B (Unparenthesized - Legally Equivalent):
hour >= 8 && hour < 10 || hour >= 17 && hour < 20
```
In C++, relational operators (`>=`, `<`) have higher precedence than logical AND (`&&`), and `&&` has higher precedence than logical OR (`||`). Therefore, Form B parses identically to Form A. However, Form A is strongly preferred because:
1. It visually delineates the morning and evening half-open intervals.
2. It satisfies compiler warning checks (`-Wparentheses`), which advise explicit grouping for mixed logical operators.

---

## 4. Alternative Concept: Lookup Table vs. Boolean Expression

We implemented and measured the alternative hourly lookup table:
```cpp
bool peakTable[24]; // Precomputed lookup table
```

### Analysis & Trade-offs
* **Time Complexity**: $\mathcal{O}(1)$ for both approaches. The expression evaluates 2 to 4 comparisons; the table performs 1 array index dereference.
* **Space Complexity**: The boolean expression requires 0 bytes of storage. The lookup table occupies 24 bytes ($24 \times 1$ byte for `bool`).
* **Critical Design Limitation of Hourly Tables**: An hourly table cannot evaluate minute-level thresholds (such as 08:30 or 17:15) without expanding to a 1,440-element table ($24 \times 60$). The boolean expression on scaled minutes `t = hour * 60 + minute` provides arbitrary minute resolution with zero memory overhead.
