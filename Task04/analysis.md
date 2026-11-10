# Task 04: Control Implicit Conversions

## Bloom’s Taxonomy Level: K5 (Evaluate) | Total Marks: 4

---

## 1. Prediction vs. Actual Execution Comparison

Before running the supplied program, output predictions were derived and saved in `prediction.txt`.

| Output Line | Predicted Output | Actual Output | Compiler Warning | Verification Status |
| :--- | :--- | :--- | :--- | :--- |
| **Line 1** (`c` and `d`) | `3 3.5` | `3 3.5` | None | **MATCH (PASS)** |
| **Line 2** (`ch` and `ch + 1`) | `C 68` | `C 68` | None | **MATCH (PASS)** |
| **Line 3** (`flag`) | `1 true` | `1 true` | None | **MATCH (PASS)** |
| **Line 4** (`s < u`) | `s >= u` | `s >= u` | `-Wsign-compare` (Line 18) | **MATCH (PASS)** |

---

## 2. In-Depth Technical Analysis of Implicit Conversions

### A. Integer Division and Integer-to-Double Conversion
```cpp
int a = 7, b = 2;
double c = a / b;
double d = static_cast<double>(a) / b;
```
* In `a / b`, both operands are `int`. C++ evaluates division using integer arithmetic, truncating toward zero to produce `3`. Only *after* the division is completed is `3` converted to `double` (`3.0`) for assignment to `c`. The fractional part is permanently lost.
* In `static_cast<double>(a) / b`, operand `a` is explicitly cast to `double` ($7.0$). In floating-point division, `b` is promoted to `double` ($2.0$), yielding the exact result `3.5`.

### B. Character Integral Promotion
```cpp
char ch = 'A' + 2;
std::cout << ch << ' ' << ch + 1 << '\n';
```
* `'A'` has ASCII value 65. Adding 2 yields 67, representing character `'C'`.
* In `ch + 1`, the operand `ch` undergoes C++ integral promotion to `int`. The addition produces integer `68`. Because `68` is of type `int`, `std::cout` outputs the decimal integer `68` rather than the character `'D'`.

### C. Integer to Boolean Conversion
```cpp
bool flag = 5;
std::cout << flag << ' ' << std::boolalpha << flag << '\n';
```
* In C++, any non-zero integer implicitly converts to `true` when assigned to a `bool`.
* By default, `std::cout` streams boolean values as integers (`1` for `true`, `0` for `false`). Activating the `std::boolalpha` stream manipulator causes `std::cout` to print the textual literal `"true"`.

### D. Signed vs. Unsigned Comparison Hazard
```cpp
unsigned int u = 1;
int s = -1;
std::cout << (s < u ? "s < u" : "s >= u") << '\n';
```
* **Compiler Warning**: `warning: comparison of integer expressions of different signedness: 'int' and 'unsigned int' [-Wsign-compare]`.
* **C++ Standard Rule**: When an `int` and `unsigned int` are compared, the signed operand is implicitly converted to `unsigned int`. 
* On a 32-bit system, `-1` converted to `unsigned int` becomes $2^{32} - 1 = 4,294,967,295$. 
* The comparison evaluated is $4,294,967,295 < 1$, which is **false**, resulting in the counter-intuitive output `s >= u`. In a smart-card system, this bug could cause a negative card balance to appear as a huge positive balance!

---

## 3. Defensive Mitigation: `static_cast` and Brace Initialization

1. **Brace Initialization (`{}`) Prevents Narrowing**:
   ```cpp
   int x{3.7};   // COMPILE-TIME ERROR: narrowing conversion of '3.7' to 'int'
   bool b{5};    // COMPILE-TIME ERROR: narrowing conversion of '5' to 'bool'
   ```
2. **Explicit Casting**:
   ```cpp
   // Safe signed/unsigned comparison by promoting both to signed long long:
   bool safeComparison = static_cast<long long>(s) < static_cast<long long>(u); // Evaluates TRUE
   ```

---

## 4. Alternative Concept & Complexity Analysis

* **Explicit Conversions Everywhere vs. Implicit Conversions**:
  * **Runtime Time Complexity**: $\mathcal{O}(1)$ for both. Built-in `static_cast` instructions compile into the exact same machine instructions as implicit conversions.
  * **Space Complexity**: $\mathcal{O}(1)$ memory (identical).
  * **Safety Benefit**: Errors are caught at **compile time** rather than manifesting as silent calculation bugs or incorrect gate admissions at runtime.
