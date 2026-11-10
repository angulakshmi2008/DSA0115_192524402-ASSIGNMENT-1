/**
 * Task11 - Alternative Concept: Dynamic Configuration Loader
 * Reads fare table from external configuration file at gate startup
 * Compiles under: g++ -std=c++17 -Wall -Wextra -pedantic
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

std::vector<long long> loadFares(const std::string& path)
{
    std::ifstream input(path);
    if (!input.is_open()) {
        throw std::runtime_error("Cannot open fare configuration file: " + path);
    }

    std::vector<long long> fares;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip comments and blank lines
        try {
            long long val = std::stoll(line);
            if (val < 0) throw std::runtime_error("Negative fare disallowed");
            fares.push_back(val);
        } catch (const std::exception& e) {
            throw std::runtime_error("Corrupted fare entry: " + line);
        }
    }

    if (fares.empty()) {
        throw std::runtime_error("Empty fare configuration file");
    }

    return fares;
}

int main()
{
    std::cout << "=== Task 11: Dynamic Configuration Loader Alternative ===\n\n";

    try {
        std::vector<long long> dynamicFares = loadFares("Task11/fares.cfg");
        std::cout << "Successfully loaded " << dynamicFares.size() << " zone fares from Task11/fares.cfg:\n";
        for (size_t i = 0; i < dynamicFares.size(); ++i) {
            std::cout << "  Zone " << i << " Base Fare: " << dynamicFares[i]
                      << " paise (Rs " << dynamicFares[i] / 100 << ".00)\n";
        }
        std::cout << "\nRuntime Access: dynamicFares[zone] is O(1) after O(z) startup ingestion.\n";
    } catch (const std::exception& e) {
        std::cerr << "Configuration Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
