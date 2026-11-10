# Task 02: Use Tokens, Keywords, Identifiers and Constants Correctly

## Bloom’s Taxonomy Level: K5 (Evaluate) | Total Marks: 3

---

## 1. Five Kinds of Tokens in C++

A token is the smallest individual unit of a C++ program that the compiler recognizes during lexical analysis. 

| Token Kind | Definition | Examples from Metro Fare System | Role in System |
| :--- | :--- | :--- | :--- |
| **Keywords** | Reserved words defined by standard C++ that have a fixed syntactical meaning. | `constexpr`, `long`, `int`, `bool`, `enum`, `class`, `switch`, `case`, `break`, `if`, `return` | Specify language constructs, control structures, and compile-time evaluations. |
| **Identifiers** | User-defined names given to variables, constants, functions, classes, and types. | `Zone`, `BASE_PAISE`, `MIN_BALANCE_PAISE`, `calculateFare`, `SmartCard`, `canTravel` | Provide meaningful human-readable handles for code elements. |
| **Constants (Literals)** | Fixed values represented directly in source code. | `1000`, `2000`, `3000`, `5000`, `20`, `50`, `true`, `false`, `'.' `, `"Denied"` | Define base fare amounts, percentage multipliers, boolean flags, and UI text. |
| **Operators** | Symbols that instruct the compiler to perform mathematical, logical, or relational operations. | `=`, `+=`, `*`, `/`, `%`, `-=`, `<<`, `::`, `&&`, `\|\|`, `?:`, `<`, `>=` | Drive fare arithmetic, peak time checking, and stream formatting. |
| **Punctuators** | Separators and grouping characters that define syntax boundaries; they carry no value. | `{ }`, `( )`, `[ ]`, `;`, `,`, `:` | Enclose code blocks, parameter lists, array subscripts, and end statements. |

---

## 2. Constant Definition Mechanisms: `#define`, `const`, and `constexpr`

| Feature | Preprocessor `#define` | `const` Qualifier | `constexpr` Specifier (Recommended) |
| :--- | :--- | :--- | :--- |
| **Processing Stage** | Preprocessor (text replacement) | Compiler (type analysis) | Compiler (compile-time constant evaluation) |
| **Type Safety** | Untyped; no type checking | Strongly typed | Strongly typed |
| **Scope Rules** | Global from definition onward; ignores namespaces/classes | Obeys C++ lexical scoping | Obeys C++ lexical scoping |
| **Evaluation Time** | Text substitution before parsing | Runtime (unless initialized by a constant expression) | Guaranteed compile-time evaluation |
| **Use as Case Labels / Array Bounds** | Yes | Only if compile-time constant initialized | Guaranteed usable everywhere a constant expression is required |
| **Debugger Visibility** | Invisible (symbol replaced in token stream) | Visible in symbol table | Visible in symbol table |

### The Macro Precedence Trap Demonstrated
If a developer defines:
```cpp
#define MACRO_FARE_INNER 500 + 500
```
Then the expression `MACRO_FARE_INNER * 2` expands textually to:
$$500 + 500 \times 2 = 500 + 1000 = 1500 \text{ paise}$$
Multiplication (`*`) has higher precedence than addition (`+`), producing 1500 instead of the expected 2000 paise. 

With `constexpr`:
```cpp
constexpr long long CONSTEXPR_FARE_INNER = 500 + 500;
```
The expression `CONSTEXPR_FARE_INNER * 2` evaluates `(1000) * 2 = 2000` paise correctly because `CONSTEXPR_FARE_INNER` is fully reduced to 1000 at compile time.

---

## 3. `enum` vs. `enum class`

```cpp
enum class Zone { Inner, Middle, Outer };
```

* **Plain `enum` (Unsafe)**:
  * Unscoped: enumerator names pollute the enclosing scope.
  * Implicit Conversion: Silently converts to `int`. `OldOuter + 5` evaluates to `2 + 5 = 7`, allowing nonsensical arithmetic on zones.
* **`enum class` (Scoped & Strongly Typed)**:
  * Scoped: Must be referenced as `Zone::Inner`, `Zone::Outer`.
  * Strong Typing: Cannot be assigned to or compared with an `int` without an explicit `static_cast<int>()`. `Zone::Outer + 5` is rejected at compile time.

---

## 4. Identifier Naming Conventions

* **Constants**: `UPPER_SNAKE_CASE` with unit included (e.g., `MIN_BALANCE_PAISE`, `PEAK_SURCHARGE_PERCENT`).
* **Types / Classes / Enums**: `PascalCase` (e.g., `Zone`, `SmartCard`, `FareTable`).
* **Functions & Methods**: `camelCase` (e.g., `canTravel`, `calculateFare`, `isPeak`).
* **Private Members**: Trailing underscore (e.g., `balancePaise_`, `hour_`).

---

## 5. Alternative Concept & Complexity Analysis

* **`#define` Macros vs. `constexpr` & `enum class`**:
  * **Runtime Time Complexity**: $\mathcal{O}(1)$ for both. Both are resolved at compile time.
  * **Space Complexity**: A `#define` consumes 0 bytes of memory. A scalar `constexpr` is inlined by the compiler directly into machine instructions, consuming 0 bytes of static storage. Only the `constexpr BASE_PAISE[]` array occupies 24 bytes ($3 \times 8$ bytes) in read-only static memory ($\mathcal{O}(z)$ for $z$ zones).
  * **Verdict**: `constexpr` and `enum class` provide complete type safety and scope isolation at zero runtime performance penalty.
