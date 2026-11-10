# DSA01-192524402-Assignment1
## Design, Implement and Validate the Fare Logic of a City Metro Smart-Card System

### Academic & Student Information
* **Course**: DSA01 – Object-Oriented Programming with C++ (CO1)
* **Bloom’s Level Assessed**: K5 – Evaluate (every task in this assignment is assessed at K5 level)
* **Nature of Assignment**: Individual
* **Student Name**: ANGULAKSHMI S.
* **Register Number**: 192524402
* **Department / Section**: Department of Information Technology / Section IT
* **Institution**: SIMATS Engineering
* **Academic Year / Semester**: 2026–2027 / Semester IV
* **Faculty Name**: Course Faculty
* **Repository Name**: `DSA01-192524402-Assignment1`
* **GitHub Remote URL**: https://github.com/angulakshmi2008/DSA0115_192524402-ASSIGNMENT-1.git

---

### Compiler Specification & Build Standards
* **Compiler**: MinGW-w64 GCC 16.2.0 (x86_64-ucrt-posix-seh, 64-bit architecture)
* **C++ Standard**: C++17 (`-std=c++17`)
* **Mandatory Flags**: `-std=c++17 -Wall -Wextra -pedantic`
* **Local Windows Workaround**:
  * On this Windows machine, the user home path (`C:\Users\ANGULAKSHMI SUBBURAJ\...`) contains a space.
  * In the WinLibs GCC toolchain, the built-in specs include an unquoted manifest directive (`default-manifest.o%s`), causing `ld.exe` to treat the space as an argument delimiter.
  * To resolve this cleanly without modifying compiler binaries, a clean `specs` file is included in the project root that omits this optional Windows manifest line.
  * A lightweight PowerShell helper `compile.ps1` invokes `g++` with this spec automatically:
    ```powershell
    .\compile.ps1 -std=c++17 -Wall -Wextra -pedantic <filename>.cpp -o <executable_name>.exe
    ```
  * **Portability Guarantee**: The C++ source code is 100% standard C++17. On any standard Linux, macOS, or Windows installation located in a directory without spaces, the standard command `g++ -std=c++17 -Wall -Wextra -pedantic <filename>.cpp -o <executable_name>.exe` compiles the code natively without any helper or custom spec.

---

### Task Directory Structure & Execution Summary

| Folder | Task Title | Marks | Compilation & Run Commands | Summary of Genuinely Executed Results |
| :--- | :--- | :---: | :--- | :--- |
| **`Task01`** | Object-Oriented Analysis | 4 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task01/metro_model_preview.cpp -o Task01/task01.exe`<br>`.\Task01\task01.exe` | Domain model designed with 5 core entities (`SmartCard`, `Passenger`, `Journey`, `Zone`, `FareTable`), UML diagram, and procedural comparison. Verified Trip 1 (Outer peak student = Rs 18), Trip 2 (Outer off-peak = Rs 30), and Trip 3 (denied due to Rs 50 minimum reserve). |
| **`Task02`** | Tokens, Keywords, Identifiers & Constants | 3 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task02/macro_trap.cpp -o Task02/macro_trap.exe`<br>`.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task02/tokens_demo.cpp -o Task02/tokens_demo.exe`<br>`.\Task02\macro_trap.exe`<br>`.\Task02\tokens_demo.exe` | Centralized `fare_constants.h`. Demonstrated preprocessor macro precedence trap (`500 + 500 * 2 = 1500` vs `constexpr = 2000`). Demonstrated `enum class` type-safety preventing accidental integer arithmetic. |
| **`Task03`** | Data Types & Type Compatibility | 4 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task03/sizeof_limits.cpp -o Task03/sizeof_limits.exe`<br>`.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task03/floating_point_error.cpp -o Task03/floating_point_error.exe`<br>`.\Task03\sizeof_limits.exe`<br>`.\Task03\floating_point_error.exe` | System `sizeof` measurements recorded. Proven why money must be stored as integer paise (IEEE-754 binary drift: `0.1 + 0.2 != 0.3`). Proved 32-bit `int` cannot hold network total (5.62B paise > `INT_MAX`). Measured double drift across $n = 10$ to $10^7$ additions. |
| **`Task04`** | Control Implicit Conversions | 4 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task04/predict_conversions.cpp -o Task04/predict_conversions.exe`<br>`.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task04/explicit_safe_casts.cpp -o Task04/explicit_safe_casts.exe`<br>`.\Task04\predict_conversions.exe`<br>`.\Task04\explicit_safe_casts.exe` | Output predicted and saved to `prediction.txt` BEFORE running. Recorded `-Wsign-compare` compiler warning. Explained integer division truncation, char promotion to 68, bool conversion, and signed/unsigned comparison hazard (`-1 < 1u` yields false). Demonstrated brace initialization narrowing protection. |
| **`Task05`** | Expressions & Operator Precedence | 4 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task05/predict_precedence.cpp -o Task05/predict_precedence.exe`<br>`.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task05/lookup_table_comparison.cpp -o Task05/lookup_table_comparison.exe`<br>`.\Task05\predict_precedence.exe`<br>`.\Task05\lookup_table_comparison.exe` | Output predicted and saved to `prediction.txt` BEFORE running. Evaluated operator precedence table, postfix vs prefix increment, C++17 integer truncation toward zero for negative operands (`-10 / 3 = -3`, `-10 % 3 = -1`), and compared boolean range check with 24-byte lookup table. |
| **`Task06`** | Design Control Structures | 4 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task06/complete_the_code.cpp -o Task06/complete_the_code.exe`<br>`.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task06/boundary_time_test.cpp -o Task06/boundary_time_test.exe`<br>`.\Task06\complete_the_code.exe`<br>`.\Task06\boundary_time_test.exe` | Completed `TODO 1` (`switch` fare logic) and `TODO 2` (`canTravel` check) leaving original code unchanged. Actual output matched `expected_output.txt` 100%. Formulated SIMATS algorithmic pseudocode. Empirically validated all 8 peak boundary times (08:00, 10:00, 17:00, 20:00). |
| **`Task07`** | Procedural to Object-Oriented | 4 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task07/procedural_gate.cpp -o Task07/procedural_gate.exe`<br>`.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task07/oo_smartcard.cpp -o Task07/oo_smartcard.exe`<br>`.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task07/public_balance_flaw.cpp -o Task07/public_balance_flaw.exe`<br>`.\Task07\procedural_gate.exe`<br>`.\Task07\oo_smartcard.exe`<br>`.\Task07\public_balance_flaw.exe` | Demonstrated that careless procedural deduction `deduct(9999)` succeeds and produces negative balance (`-4599`), whereas `SmartCard` encapsulating `balance_` as private rejects careless deduction and keeps balance at 5400 paise. Analyzed public member flaw. |
| **`Task08`** | Read and Print Data Safely | 4 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task08/safe_io_gate.cpp -o Task08/safe_io_gate.exe`<br>`.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task08/getline_stoi_alt.cpp -o Task08/getline_stoi_alt.exe`<br>`Get-Content Task08/t8.in \| .\Task08\safe_io_gate.exe`<br>`Get-Content Task08/t8b.in \| .\Task08\safe_io_gate.exe`<br>`Get-Content Task08/t8g.in \| .\Task08\getline_stoi_alt.exe` | Stream validation implemented using `cin.fail()`, `cin.clear()`, and `cin.ignore()`. Successfully caught and skipped corrupted records (`2 abc 10` and `3 10 0`) and bad journey counts. Formatted financial reports with `<iomanip>`. Evaluated `getline` + `stoi` alternative. |
| **`Task09`** | Implement & Test Fare Logic | 4 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task09/test_suite.cpp -o Task09/test_suite.exe`<br>`.\Task09\test_suite.exe` | Built unified engine in `core.h`. Automated 14-case table-driven test suite (14/14 PASS). Validated exact Rs 50 card balance boundary (6000-1000 allowed; 5999-1000 denied; 6000-1001 denied). Hand-calculated 5 journeys total matched 11,000 paise (Rs 110.00) exactly. |
| **`Task10`** | Compare Expectations with Execution | 4 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task10/scale_simulation.cpp -o Task10/scale_simulation.exe`<br>`.\Task10\scale_simulation.exe` | Deterministic simulation across 20k, 500k, and 3M journeys. Measured exact `long long` total (5,624,397,900 paise = Rs 56,243,979.00). Proved 32-bit `int` overflows. Measured single-precision `float` precision loss (-Rs 51,263.00). Documented compiler warnings log. |
| **`Task11`** | Recommend & Defend Final Design | 4 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task11/config_loader_alt.cpp -o Task11/config_loader_alt.exe`<br>`.\Task11\config_loader_alt.exe` | Comprehensive architectural defense of types, constants, and classes. Formulated Standard Operating Procedure (SOP) for fare revisions. Documented system limitations. Implemented and benchmarked dynamic configuration file loader (`fares.cfg`). |
| **`Task12`** | Day-4 Design Challenge | 8 | `.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task12/task12_flaw_demo.cpp -o Task12/task12_flaw_demo.exe`<br>`.\compile.ps1 -std=c++17 -Wall -Wextra -pedantic Task12/task12_adapted_metro.cpp -o Task12/task12_adapted_metro.exe`<br>`.\Task12\task12_flaw_demo.exe`<br>`.\Task12\task12_adapted_metro.exe` | Evaluated 4 subsections (12A–12D). Demonstrated old switch failure on Airport (silent Rs 0 free ride). Adapted architecture with `Zone::Airport` (Rs 60), 10% weekend discount (replaces peak surcharge), and 2-zone `Pass` class with bitmask. All 13 test cases passed. |

---

### Verification and Academic Integrity Checklist
* [x] All 12 tasks implemented in separate folders (`Task01` through `Task12`).
* [x] No fabricated outputs, execution logs, or benchmark figures.
* [x] Predictions written and recorded before execution in Tasks 04 and 05.
* [x] Complete the Code tasks preserve the original supplied code and only replace `TODO` blocks.
* [x] Progressive commits maintained task-by-task.
* [x] Verified zero warnings under `-std=c++17 -Wall -Wextra -pedantic`.
