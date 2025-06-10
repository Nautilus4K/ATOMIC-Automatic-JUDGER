#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <fstream>
#include <nlohmann/json.hpp>

#include "consts.h"

using nlohmann::json;

// Custom functions as tools
inline std::string intToString(int n) {
    // Conversion from INTEGER to STRING
    if (n == 0) return "0";
    
    std::string value = "";
    while (n > 0) {
        value += char((n % 10) + '0'); // Literal magic
        n /= 10;
    }
    std::reverse(value.begin(), value.end());

    return value;
}

inline std::string doubleToString(double d) {
    // Conversion from DOUBLE to STRING
    std::ostringstream oss;
    oss << d; // Yeah this is the true magic
    return oss.str();
}

inline double stringToDouble(const std::string &str) {
    try {
        return std::stod(str); // Built-in function
    } catch (const std::invalid_argument &e) {
        return 0.0; // Handle invalid input (e.g., empty string or non-numeric data)
    } catch (const std::out_of_range &e) {
        return 0.0; // Handle out-of-range errors
    }
}


inline int stringToInt(const std::string &s) {
    int result = 0;
    bool negative = false;
    std::size_t i = 0;

    if (s[i] == '-') {
        negative = true;
        i++;
    }

    for (; i < s.length(); ++i) {
        if (s[i] < '0' || s[i] > '9') {
            // Invalid character for number
            break;
        }
        result = result * 10 + (s[i] - '0');
    }

    return negative ? -result : result;
}

inline std::string turnStringUppercaseA(const std::string& s) {
    std::string result = "";

    for (char x: s) {
        if (islower(x)) {
            x -= 32;
        }

        result.push_back(x);
    }

    return result;
}

inline bool saveContestsInfo(const json& val) {
    bool successfullyOpenFile = false;
    std::fstream contestsFile(dirPath + CONTESTS_PATH, std::ios::out);
    if (contestsFile.is_open()) {
        contestsFile << val;

        // Tbh I am just gonna refresh the entire thing to make sure its safe and
        // up to date. WELP. IT DOESN'T WORK THAT WAY. Let's set a variable after we closed this fully.
        // toCnts(currentCnts); // SOUNDS GOOD
        successfullyOpenFile = true;
    }
    contestsFile.close();

    return successfullyOpenFile;
}

#endif