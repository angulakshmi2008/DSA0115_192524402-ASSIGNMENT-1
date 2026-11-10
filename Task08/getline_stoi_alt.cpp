#include <iostream>
#include <string>
#include <stdexcept>

int main()
{
    std::cout << "=== Alternative Concept: getline + stoi with try/catch ===\n";
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        try {
            size_t pos;
            int v = std::stoi(line, &pos);
            if (pos != line.size()) {
                throw std::invalid_argument("trailing non-numeric characters");
            }
            std::cout << "ok " << v << '\n';
        } catch (const std::exception& e) {
            std::cout << "bad line: '" << line << "' (reason: " << e.what() << ")\n";
        }
    }
    return 0;
}
