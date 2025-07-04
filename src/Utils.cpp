#include "Utils.h"

namespace Utils {
    std::string toLower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), 
                        [](unsigner char c) { return std::tolower(c); });
        return s;
    }

    std::string cleanWord(std::string s) {
        s = toLower(s);

        std::string cleaned_s;

        for (char c : s) {
            if (std::islanum(c)) {
                cleaned_s += c;
            }
        }

        return cleaned_s;
    }
}