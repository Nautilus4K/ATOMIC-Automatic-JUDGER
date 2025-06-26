#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <fstream>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>

#include "consts.h"

using nlohmann::json;

// Custom functions as tools

// A conversion fron int to std::string
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

// A conversion from double to std::string
inline std::string doubleToString(double d) {
    // Conversion from DOUBLE to STRING
    std::ostringstream oss;
    oss << d; // Yeah this is the true magic
    return oss.str();
}

// A conversion from std::string to double
inline double stringToDouble(const std::string &str) {
    try {
        return std::stod(str); // Built-in function
    } catch (const std::invalid_argument &e) {
        return 0.0; // Handle invalid input (e.g., empty string or non-numeric data)
    } catch (const std::out_of_range &e) {
        return 0.0; // Handle out-of-range errors
    }
}

// A conversion from std::string to int
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

// Returns the input string as uppercase
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

// Save the designated value to `CONTESTS_PATH`
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

// Get json value from `CONTESTS_PATH`
inline json getContestsInfo() {
    json ret;

    std::fstream contestsFile(dirPath + CONTESTS_PATH, std::ios::in);
    if (contestsFile.is_open()) {
        try {
            ret = json::parse(contestsFile);

            // Successfully parsed. Prints logging
            std::cout << "[JSON: contests] " << ret << '\n';

            contestsFile.close();
        } catch (const json::parse_error& e) {
            std::fstream file(dirPath + CONTESTS_PATH, std::ios::out | std::ios::trunc);
            file << "{}";
            std::cout << "Repair successful\n";
            file.close();

            ret = getContestsInfo();
        }
    } else {
        // Creating file in case the file CAN'T BE ACCESSED???
        std::fstream file(dirPath + CONTESTS_PATH, std::ios::out | std::ios::trunc);
        file << "{}";
        std::cout << "Repair successful\n";
        file.close();

        ret = getContestsInfo();
    }

    return ret;
}

// Save the designated value to `USERDATA_PATH`
inline bool saveUsersInfo(const json& val) {
    bool successfullyOpenFile = false;
    std::fstream userdataFile(dirPath + USERDATA_PATH, std::ios::out);
    if (userdataFile.is_open()) {
        userdataFile << val;

        // Tbh I am just gonna refresh the entire thing to make sure its safe and
        // up to date. WELP. IT DOESN'T WORK THAT WAY. Let's set a variable after we closed this fully.
        // toCnts(currentCnts); // SOUNDS GOOD
        successfullyOpenFile = true;
    }
    userdataFile.close();

    return successfullyOpenFile;
}

// Get json value from `USERDATA_PATH`
inline json getUsersInfo() {
    json ret;

    std::fstream userdataFile(dirPath + USERDATA_PATH, std::ios::in);
    if (userdataFile.is_open()) {
        try {
            ret = json::parse(userdataFile);

            // Successfully parsed. Prints logging
            std::cout << "[JSON: users] " << ret << '\n';

            userdataFile.close();
        } catch (const json::parse_error& e) {
            // Same shit. Different day
            std::fstream file(dirPath + USERDATA_PATH, std::ios::out | std::ios::trunc);
            file << "{}";
            std::cout << "Repair successful\n";
            file.close();

            ret = getUsersInfo();
        }
    } else {
        // Creating file in case the file CAN'T BE ACCESSED??? Maybe.
        std::fstream file(dirPath + USERDATA_PATH, std::ios::out | std::ios::trunc);
        file << "{}";
        std::cout << "Repair successful\n";
        file.close();

        ret = getUsersInfo();
    }

    return ret;
}

// Save the designated value to `CLASSES_PATH`
inline bool saveClassesInfo(const json& val) {
    bool successfullyOpenFile = false;
    std::fstream classesFile(dirPath + CLASSES_PATH, std::ios::out);
    if (classesFile.is_open()) {
        classesFile << val;

        // Tbh I am just gonna refresh the entire thing to make sure its safe and
        // up to date. WELP. IT DOESN'T WORK THAT WAY. Let's set a variable after we closed this fully.
        // toCnts(currentCnts); // SOUNDS GOOD
        successfullyOpenFile = true;
    }
    classesFile.close();

    return successfullyOpenFile;
}

// Get json value from `CLASSES_PATH`
inline json getClassesInfo() {
    json ret;

    std::fstream classesFile(dirPath + CLASSES_PATH, std::ios::in);
    if (classesFile.is_open()) {
        try {
            ret = json::parse(classesFile);

            // Successfully parsed. Prints logging
            std::cout << "[JSON: classes] " << ret << '\n';

            classesFile.close();
        } catch (const json::parse_error& e) {
            // Same shit. Different day
            std::fstream file(dirPath + CLASSES_PATH, std::ios::out | std::ios::trunc);
            file << "{}";
            std::cout << "Repair successful\n";
            file.close();

            ret = getClassesInfo();
        }
    } else {
        // Creating file in case the file CAN'T BE ACCESSED??? Maybe.
        std::fstream file(dirPath + CLASSES_PATH, std::ios::out | std::ios::trunc);
        file << "{}";
        std::cout << "Repair successful\n";
        file.close();

        ret = getClassesInfo();
    }

    return ret;
}

// Save the designated value to `ALIAS_PATH`
inline bool saveAliasesInfo(const json& val) {
    bool successfullyOpenFile = false;
    std::fstream aliasesFile(dirPath + ALIAS_PATH, std::ios::out);
    if (aliasesFile.is_open()) {
        aliasesFile << val;

        // Tbh I am just gonna refresh the entire thing to make sure its safe and
        // up to date. WELP. IT DOESN'T WORK THAT WAY. Let's set a variable after we closed this fully.
        // toCnts(currentCnts); // SOUNDS GOOD
        successfullyOpenFile = true;
    }
    aliasesFile.close();

    return successfullyOpenFile;
}

// Get json value from `ALIAS_PATH`
inline json getAliasesInfo() {
    json ret;

    std::fstream aliasesFile(dirPath + ALIAS_PATH, std::ios::in);
    if (aliasesFile.is_open()) {
        try {
            ret = json::parse(aliasesFile);

            // Successfully parsed. Prints logging
            std::cout << "[JSON: aliases] " << ret << '\n';

            aliasesFile.close();
        } catch (const json::parse_error& e) {
            // Same shit. Different day
            std::fstream file(dirPath + ALIAS_PATH, std::ios::out | std::ios::trunc);
            file << "{}";
            std::cout << "Repair successful\n";
            file.close();

            ret = getAliasesInfo();
        }
    } else {
        // Creating file in case the file CAN'T BE ACCESSED??? Maybe.
        std::fstream file(dirPath + ALIAS_PATH, std::ios::out | std::ios::trunc);
        file << "{}";
        std::cout << "Repair successful\n";
        file.close();

        ret = getAliasesInfo();
    }

    return ret;
}

// Save the designated value to `SETTINGS_PATH`
inline bool saveSettingsInfo(const json& val) {
    bool successfullyOpenFile = false;
    std::fstream settingsFile(dirPath + SETTINGS_PATH, std::ios::out);
    if (settingsFile.is_open()) {
        settingsFile << val;

        // Tbh I am just gonna refresh the entire thing to make sure its safe and
        // up to date. WELP. IT DOESN'T WORK THAT WAY. Let's set a variable after we closed this fully.
        // toCnts(currentCnts); // SOUNDS GOOD
        successfullyOpenFile = true;
    }
    settingsFile.close();

    return successfullyOpenFile;
}

// Get json value from `SETTINGS_PATH`
inline json getSettingsInfo() {
    json ret;

    std::fstream settingsFile(dirPath + SETTINGS_PATH, std::ios::in);
    if (settingsFile.is_open()) {
        try {
            ret = json::parse(settingsFile);

            // Successfully parsed. Prints logging
            std::cout << "[JSON: settings] " << ret << '\n';

            settingsFile.close();
        } catch (const json::parse_error& e) {
            // Same shit. Different day
            std::fstream file(dirPath + SETTINGS_PATH, std::ios::out | std::ios::trunc);
            file << "{}";
            std::cout << "Repair successful\n";
            file.close();

            ret = getSettingsInfo();
        }
    } else {
        // Creating file in case the file CAN'T BE ACCESSED??? Maybe.
        std::fstream file(dirPath + SETTINGS_PATH, std::ios::out | std::ios::trunc);
        file << "{}";
        std::cout << "Repair successful\n";
        file.close();

        ret = getSettingsInfo();
    }

    return ret;
}

inline std::string sha256(const std::string& data) {
    // Create a char array for the hash to be made. this will be a sequence of characters in which
    // the SHA256 hash is gonna be created.
    unsigned char        hash[SHA256_DIGEST_LENGTH];
    const unsigned char  *dataArray = reinterpret_cast<const unsigned char*>(data.c_str());
    SHA256(dataArray, data.size(), hash);

    // We cannot use reinterpret_cast AGAIN because it will produce raw binary data
    // const char *hashUsable = reinterpret_cast<const char*>(hash);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i]; // The equivalent of .hexdigest() in Python
    }

    return ss.str();
}

inline std::string privatizesha256(const std::string& hash, int showLength) {
    std::string halfresult = hash.substr(0, showLength);

    return halfresult + "...";
}

// Get the contests of EACH user (in JSON format)
inline json getSubmissionInfo(const std::string& user) {
    std::fstream userSubmitResultFile(dirPath + USERSTATS_DIR + user + std::string(".json"), std::ios::in);
    
    bool failed = false;
    json submissions;
    if (userSubmitResultFile.is_open()) {
        try {
            submissions = json::parse(userSubmitResultFile);

            std::cout << "[JSON: submissions] " << submissions << '\n';
        } catch (const json::parse_error& e) {
            // If error caught. We just delete that file.
            remove((dirPath + USERSTATS_DIR + user + std::string(".json")).c_str());
            failed = true;
        }
    } else {
        failed = true;
    }

    if (failed) return json{}; // Null json value
    return submissions;
}

inline void saveSubmissionInfo(const std::string& user, const json& val) {
    std::fstream userSubmitResultFile(dirPath + USERSTATS_DIR + user + std::string(".json"), std::ios::out);
    
    json ret;
    if (userSubmitResultFile.is_open()) {
        userSubmitResultFile << val;
    }
    userSubmitResultFile.close();
}

// Converts a 1-based column number to Excel-style column letters
inline std::string columnToLetters(int col) {
    std::string result;
    while (col > 0) {
        col--; // Excel is 1-based, but we need 0-based for modulo
        result = char('A' + (col % 26)) + result;
        col /= 26;
    }
    return result;
}

// Map rows and columns into Excel format (e.g., B3, AA15, etc.)
inline std::string mapToExcelCell(const int& row, const int& col) {
    return columnToLetters(col) + intToString(row);
}

inline std::tuple<int, int, int> hexToRgb(const std::string& hex) {
    std::string cleanHex = hex.substr(1); // Remove the '#'
    int r = std::stoi(cleanHex.substr(0, 2), nullptr, 16);
    int g = std::stoi(cleanHex.substr(2, 2), nullptr, 16);
    int b = std::stoi(cleanHex.substr(4, 2), nullptr, 16);
    return {r, g, b};
}

#endif