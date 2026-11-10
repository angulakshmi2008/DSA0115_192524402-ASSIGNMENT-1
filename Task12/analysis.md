# Task 12: Day-4 Design Challenge – Re-evaluate and Adapt the Design

## Bloom’s Taxonomy Level: K5 (Evaluate) | Total Marks: 8

---

## 1. Changed Requirements & Stated Assumptions

The metro authority introduced three major feature requirements:
1. **Fourth Zone (Airport)**: Farthest zone added with base fare Rs 60.00 ($6000$ paise).
2. **Weekend Discount (10%)**: A $10\%$ discount applies on Saturdays and Sundays. The discount **does not combine** with the peak surcharge; on weekends, the peak surcharge is inactive and the weekend discount applies.
3. **Monthly Pass**: Allows unlimited journeys in two chosen zones for a calendar month. Covered trips cost Rs 0.00 and do not require the card to hold the Rs 50 minimum balance.

### Documented Assumptions
* **A6 (Weekend Definition)**: Saturday (weekday 5) and Sunday (weekday 6).
* **A7 (Discount Application Order)**: Concession (50%) is applied first to base fare; then either the weekend discount (on weekends) or the peak surcharge (on peak weekdays) is applied to the concession-adjusted base fare, rounded half-up to integer paise.
* **A8 (Pass Scope & Pricing)**: Covers unlimited travel across the two specified zones for the active month. Because the question does not specify a pass purchase tariff, pass purchase pricing is outside scope.

---

## 2. Subsection A: Impact Analysis (2 Marks)

The new requirements impact the following components of the original design:

| Original Component | Affected? | Exact Impact & Required Modification |
| :--- | :---: | :--- |
| `BASE_PAISE[]` | **YES** | Expanded from 3 to 4 elements: `{1000, 2000, 3000, 6000}`. |
| `enum class Zone` | **YES** | Added `Airport` enumerator and `Count` for array boundary validation. |
| `switch` in `fare()` | **YES** | Broken. The original switch has no case for `Airport`, causing silent failure. |
| `fare(Zone, bool peak)` signature | **YES** | Inadequate. Only accepts zone and peak flag; cannot accept weekday, date, or concession. |
| Surcharge Expression | **YES** | Must become conditional on `!isWeekend(weekday)`. |
| Weekend Discount | **NEW** | Added `WEEKEND_DISCOUNT_PERCENT = 10` constant and evaluator. |
| `SmartCard` Class | **YES** | Must support pass association (`attachPass()`) and allow pass-covered trips without minimum-balance denial. |
| `Journey` Entity | **YES** | Expanded to encapsulate `zone`, `hour`, `minute`, `weekday`, `concession`, `year`, `month`. |
| `Pass` Class | **NEW** | Introduced to encapsulate covered zone bitmask, validity year, and month. |

---

## 3. Subsection B: Evaluation of the Original Design (2 Marks)

We empirically tested the ability of the original design to absorb the changes without refactoring:

### Example 1: Fourth Zone (Airport) in Old Switch
* When `Zone::Airport` is passed to the original `switch(z)` (demonstrated in `task12_flaw_demo.cpp`):
  ```
  warning: enumeration value 'Airport' not handled in switch [-Wswitch]
  Calculated Fare: 0 paise (Rs 0.00 - SILENT FREE RIDE REVENUE LOSS!)
  ```
* Because `base` was initialized to `0` and no case matched, Airport passengers travel for **free**, causing catastrophic revenue leakage.

### Example 2: Weekend Discount Incompatibility
* Airport journey on Saturday at 09:00 for a student passenger:
  * Expected: Base Rs 60 $\rightarrow$ Concession Rs 30 $\rightarrow$ Weekend $-10\%$ = **Rs 27.00** (2700 paise).
  * Original Design: Function signature `fare(Zone, bool peak)` has no weekday parameter. Passing `peak = true` adds $20\%$ surcharge, charging Rs 36.00 and violating the non-combination rule.

### Example 3: Monthly Pass Denial
* A commuter holding an Inner + Middle monthly pass has Rs 10.00 on their smart card. A Middle zone journey must be free.
* Original `SmartCard::deduct(2000)` checks `1000 - 2000 >= 5000` $\rightarrow$ **DENIED**. The card rejects a valid pass holder because it lacks pass awareness.

---

## 4. Subsection C: Adaptation & Implementation (2 Marks)

We selected the **MODIFY** design strategy: preserving the proven integer-paise arithmetic, constants centralization, and balance encapsulation, while adapting the architecture:
1. **Array Lookup Table**: Replaced the fragile `switch` with `BASE_PAISE[ZONE_COUNT]`.
2. **Expanded `Journey` Structure**: Encapsulates `zone`, `hour`, `minute`, `weekday`, `concession`, `year`, and `month`.
3. **Dedicated `Pass` Class**: Uses an efficient bitmask `(1u << zoneA) | (1u << zoneB)` and checks validity against journey calendar month.
4. **Enhanced `SmartCard::travel()`**: Evaluates pass coverage first (charging 0 paise with zero deduction and bypassing the minimum balance rule). If uncovered, it deducts the computed fare normally.

---

## 5. Subsection D: Evidence-Based Defence (2 Marks)

### Design Trade-offs Evaluated
* **Retain**: Unviable. Proved to cause silent zero-fare revenue loss and impossible parameter passing.
* **Redesign from Scratch (Full Polymorphism)**: Building dynamic inheritance hierarchies with virtual functions (`PassPolicy`, `DiscountPipeline`) introduces pointer indirections, heap allocations, and virtual dispatch overhead unnecessary for a compact transit gate.
* **Modify (Adopted)**: Balances clean extensibility with runtime efficiency. Preserves all earlier verified invariants while achieving complete feature compliance.

### Empirical Test Suite Results (13/13 Cases Passed)
```text
Airport weekday 12:00 (base fare Rs 60)       | Expected:  6000 | Actual:  6000 | [PASS]
Airport weekday 09:00 (peak +20% = Rs 72)     | Expected:  7200 | Actual:  7200 | [PASS]
Inner Saturday 12:00 (weekend -10% = Rs 9)    | Expected:   900 | Actual:   900 | [PASS]
Inner Saturday 09:00 (peak window, weekend discount replaces peak) | Expected:   900 | Actual:   900 | [PASS]
Outer Sunday 18:00 (weekend -10% = Rs 27)     | Expected:  2700 | Actual:  2700 | [PASS]
Airport Sunday student (concession 3000 then -10%) | Expected:  2700 | Actual:  2700 | [PASS]
Middle Friday 17:00 senior peak (concession 1000 + 20%) | Expected:  1200 | Actual:  1200 | [PASS]
Pass: Middle journey covered (fare 0)         | Expected:     0 | Actual:     0 | [PASS]
Pass: Card balance remains unchanged          | Expected: 10000 | Actual: 10000 | [PASS]
Pass: Outer journey NOT covered (deducts 3600 peak) | Expected:  3600 | Actual:  3600 | [PASS]
Pass: Balance after Outer journey             | Expected:  6400 | Actual:  6400 | [PASS]
Pass: Expired month (Dec 2026) charges normal fare | Expected:  2000 | Actual:  2000 | [PASS]
No Pass: Airport peak denied due to Rs 50 rule | Expected:    -1 | Actual:    -1 | [PASS]
```

### Complexity Performance
* **Time Complexity**: $\mathcal{O}(1)$ time per journey (1 array lookup, at most 2 percentage calculations, 1 bitwise pass test).
* **Space Complexity**: $\mathcal{O}(1)$ memory per card. Adding a pointer `const Pass* pass_` increases `sizeof(SmartCard)` from 8 to 16 bytes on 64-bit systems.
