# Task 08: Read and Print Data Safely

## Bloom’s Taxonomy Level: K5 (Evaluate) | Total Marks: 4

---

## 1. Defensive Input Stream Management with `std::cin`

When standard input expects an integer and the operator types non-numeric characters (e.g. `abc`), `std::cin` encounters a type failure:
1. The stream enters an error state (`cin.fail() == true`).
2. The extracted variable is assigned `0`.
3. The invalid characters remain in the input buffer.
4. **Cascading Failure**: Every subsequent extraction `std::cin >> x` fails immediately without waiting for user input, causing an infinite loop.

### The Three-Step Recovery Protocol
```cpp
// 1. Detect extraction failure or out-of-range domain violation
if (!(std::cin >> zone >> h >> m) || zone < 0 || zone > 2 || h < 0 || h > 23 || m < 0 || m > 59) {
    // 2. Clear fail/bad error flags to restore stream to healthy state
    std::cin.clear();
    // 3. Purge all corrupted characters up to the next newline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    continue; // Safely skip corrupted record and process next journey
}
```

---

## 2. Professional Tabular Formatting with `<iomanip>`

All financial arithmetic is kept in integer paise. Only during display are values converted to decimal rupees:
* `std::setw(w)`: Sets minimum field width for neat column alignment.
* `std::setfill('0')`: Zero-pads hours and minutes (e.g., `09:05` instead of `9: 5`).
* `std::fixed` and `std::setprecision(2)`: Enforces standard two-decimal financial formatting (`12.00`).

---

## 3. Empirical Test Results

### Test Case 1: Corrupted Mid-Stream Records (`t8.in`)
* Input includes journey 2 with alphabetic hour (`2 abc 10`) and journey 4 with an out-of-range zone (`3 10 0`).
* **Execution Result**: Both malformed records were identified, cleanly reported, and skipped. Journey 3 (`18:05` peak, Rs 24.00) and Journey 5 (`20:00` off-peak, Rs 30.00 - denied due to Rs 50 minimum rule) were processed with 100% accuracy.

### Test Case 2: Interactive Error Recovery on Count (`t8b.in`)
* User input enters non-numeric count (`abc`), then negative count (`-3`), before providing valid count (`2`).
* **Execution Result**: Both invalid counts triggered error recovery prompts, and the gate successfully continued to process the remaining trips.

---

## 4. Alternative Concept: `std::getline()` and `std::stoi()` with `try/catch`

In `getline_stoi_alt.cpp`, each record is ingested as a complete string line:
```cpp
std::string line;
while (std::getline(std::cin, line)) {
    try {
        size_t pos;
        int v = std::stoi(line, &pos);
        if (pos != line.size()) throw std::invalid_argument("trailing characters");
        std::cout << "ok " << v << '\n';
    } catch (const std::exception& e) {
        std::cout << "bad line: '" << line << "'\n";
    }
}
```

### Complexity & Recovery Trade-off
* **Time Complexity**: $\mathcal{O}(L)$ for a line of length $L$ in both approaches (examining each character once).
* **Space Complexity**: Direct `cin >>` consumes $\mathcal{O}(1)$ extra storage; `getline()` requires an $\mathcal{O}(L)$ string buffer.
* **Error Isolation**: `getline()` isolates errors to individual lines automatically. If one line contains garbage, `getline()` discards it naturally upon reading the next line without requiring manual buffer purging.
