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

#include <QtCore/QFile>

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

// Quick little function to find and replace all instances of a substring to another substring in an `std::string` type object
inline std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    if (from.empty())
        return str;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Move past the replacement
    }
    return str;
}

// Sanitize values that is gonna be fed into the BACKUP file. This is to avoid any exploits.
inline std::string sanitizeValue(std::string val) {
    // The sanitizer itself
    val = replaceAll(val, BACKUP_SANITIZATION_PREMARKER, BACKUP_SANITIZATION_PREMARKER + BACKUP_SANITIZATION_PREMARKER);

    // Separator
    val = replaceAll(val, std::string(1, BACKUP_SEPARATOR), BACKUP_SANITIZATION_PREMARKER + std::string(1, BACKUP_SEPARATOR));

    // Section marker (the *)
    val = replaceAll(val, std::string(1, BACKUP_SECTIONMARKER), BACKUP_SANITIZATION_PREMARKER + std::string(1, BACKUP_SECTIONMARKER));

    // Each value separator
    val = replaceAll(val, std::string(1, BACKUP_EACHVALUE_SEPARATOR), BACKUP_SANITIZATION_PREMARKER + std::string(1, BACKUP_EACHVALUE_SEPARATOR));

    // Each group separator
    val = replaceAll(val, std::string(1, BACKUP_EACHGROUP_SEPARATOR), BACKUP_SANITIZATION_PREMARKER + std::string(1, BACKUP_EACHGROUP_SEPARATOR));

    return val;
}

// Clean trailings stuff. Offbrand strip()
inline std::string cleanTrails(const std::string& s) {
    if (s.empty()) return ""; // handle this exception

    size_t actualBegin = 0;
    size_t actualEnd = s.size() - 1;

    while (actualBegin < s.size() && (s[actualBegin] == ' ' || s[actualBegin] == '\n')) actualBegin++;
    while (actualEnd > actualBegin && (s[actualEnd] == ' ' || s[actualEnd] == '\n')) actualEnd--;

    return s.substr(actualBegin, actualEnd - actualBegin + 1);
}

// Extract code from AI responses
inline std::string extractCodeBlocks(const std::string& response) {
    std::string normalized = response;
    normalized.erase(std::remove(normalized.begin(), normalized.end(), '\r'), normalized.end());
    size_t currentProgress = 0;
    std::string code = "";
    
    while (true) {
        size_t openingMarker = normalized.find("```", currentProgress);
        if (openingMarker == std::string::npos) break;
        
        size_t start = openingMarker + 3;
        
        // Check if there's a language identifier on the same line
        size_t newlinePos = normalized.find('\n', start);
        if (newlinePos == std::string::npos) break;
        
        // Only skip the line if it contains a language identifier (non-whitespace before newline)
        bool hasLangId = false;
        for (size_t i = start; i < newlinePos; i++) {
            if (normalized[i] != ' ' && normalized[i] != '\t') {
                hasLangId = true;
                break;
            }
        }
        
        if (hasLangId) {
            start = newlinePos + 1;
        }
        
        size_t end = normalized.find("```", start);
        if (end == std::string::npos) end = normalized.length();
        
        code += normalized.substr(start, end - start);
        currentProgress = end + 3;
    }
    
    return code;
}

inline void prepareStart() {
    // Files that need to exists
    std::vector<std::string> criticalFiles = {
        // JSONs (Empty-able)
        CLASSES_PATH,
        CONTESTS_PATH,
        USERDATA_PATH
    };

    // Folders that needs to exists
    std::vector<std::string> criticalDirectories {
        // User-related directories
        USERSTATS_DIR,
        USERQUEUE_DIR,
        USERSUBHISTORY_DIR,
        SUBMITLOG_DIR
    };

    // Files that if not found then this is done for (yeah theyre must haves)
    std::vector<std::string> mustFiles {
        // Scripts
        JUDGING_PATH,
        WEBSERVER_PATH,
    };

    for (const std::string& fps : criticalFiles) {
        std::filesystem::path path(dirPath + fps);
        if (!std::filesystem::exists(path)) {
            // Before we dive in, shall we try doing a big check?
            // Sure let's do it.
            // This is a check for all the DIRECTORIES that are the parents. They have to exist first
            
            std::filesystem::path currentParent = path.parent_path();
            std::vector<std::filesystem::path> trials; // Directories to be created.
            while (!std::filesystem::exists(currentParent)) {
                // Mark this as to be created
                trials.push_back(currentParent);
                currentParent = currentParent.parent_path();
            }

            // Good god.
            // Look at all these directories
            for (int i = trials.size() - 1; i >= 0; i--) {
                std::filesystem::create_directories(trials[i]);
            }
            
            std::fstream file(path, std::ios::out | std::ios::trunc);
            if (!file.is_open()) {
                std::cerr << "[prepareStart()] UNABLE TO CREATE FILE at " << dirPath + fps << '\n';
                exit(-24);
            }

            // Alright its fine
            file << "{}"; // They're all jsons actually...
            file.close();
            std::cout << "[prepareStart()] Generated file at: " << dirPath + fps << '\n';
        }
    }

    // Moving onto the folders
    for (const std::string& fls : criticalDirectories) {
        std::filesystem::path path(dirPath + fls);
        if (!std::filesystem::exists(path)) {
            std::filesystem::path currentParent = path.parent_path();
            std::vector<std::filesystem::path> trials; // Directories to be created.
            while (!std::filesystem::exists(currentParent)) {
                // Mark this as to be created
                trials.push_back(currentParent);
                currentParent = currentParent.parent_path();
            }

            // Good god.
            // Look at all these directories
            for (int i = trials.size() - 1; i >= 0; i--) {
                std::filesystem::create_directories(trials[i]);
            }

            std::filesystem::create_directories(fls);
        }
    }

    // Now I have to deal with these twos
    // SETTINGS_PATH,
    // ALIAS_PATH,
    std::fstream settingsFile(dirPath + SETTINGS_PATH, std::ios::out | std::ios::trunc);
    std::fstream aliasesFile(dirPath + ALIAS_PATH, std::ios::out | std::ios::trunc);

    if (!settingsFile.is_open()) {
        std::cerr << "[prepareStart()] UNABLE TO CREATE FILE at " << SETTINGS_PATH << '\n';
        exit(-24);
    } 
    if (!aliasesFile.is_open()) {
        std::cerr << "[prepareStart()] UNABLE TO CREATE FILE at " << ALIAS_PATH << '\n';
        exit(-24);
    }

    // This is the default general settings
    settingsFile << "{\"max_not_logged_in_session_seconds\":31536000,\"reload_time\":60.0,\"show_test\":true,\"wait_time\":1.0}";
    aliasesFile << R"(
{
    "footer": "Powered by <strong>ATOM://C</strong> & <strong>waitress</strong>",
    "hostname": "Lê Quang N",
    "slogan": "Keep typing, keep loving.",
    "softwarelink": "https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER",
    "website_name": "Lớp dạy chuyên tin"
}
    )";

    settingsFile.close();
    aliasesFile.close();

    // Now over to themes
    if (!std::filesystem::exists(dirPath + THEME_PATH)) {
        // Fuck
        // Let's regen shall we?
        std::fstream themeFile(dirPath + THEME_PATH, std::ios::out | std::ios::trunc);
        if (!themeFile.is_open()) {
            std::cerr << "[prepareStart()] UNABLE TO CREATE FILE at " << THEME_PATH << '\n';
            exit(-24);
        }

        QFile defThemeFile(DEFAULT_THEME_PATH);

        if (!defThemeFile.open(QIODeviceBase::ReadOnly)) {
            std::cerr << "[prepareStart()] UNABLE TO READ Qt RESOURCE at " << DEFAULT_THEME_PATH.toStdString() << '\n';
            exit(-24);
        }

        themeFile << defThemeFile.readAll().toStdString();
        defThemeFile.close();
        themeFile.close();
    }

    // With themes done its versions.
    if (!std::filesystem::exists(dirPath + VERSION_PATH)) {
        // Yo lets go
        std::fstream versionFile(dirPath + VERSION_PATH, std::ios::out | std::ios::trunc);
        if (!versionFile.is_open()) {
            std::cerr << "[prepareStart()] UNABLE TO CREATE FILE at " << VERSION_PATH << '\n';
            exit(-24);
        }

        QFile defVersionFile(DEFAULT_VERSION_PATH);

        if (!defVersionFile.open(QIODeviceBase::ReadOnly)) {
            std::cerr << "[prepareStart()] UNABLE TO READ Qt RESOURCE at " << DEFAULT_VERSION_PATH.toStdString() << '\n';
            exit(-24);
        }

        versionFile << defVersionFile.readAll().toStdString();
        defVersionFile.close();
        versionFile.close();
    }

    if (!std::filesystem::exists(dirPath + JUDGING_PATH)) {
        // Niceeeeee
        std::fstream judgingFile(dirPath + JUDGING_PATH, std::ios::out | std::ios::trunc);
        if (!judgingFile.is_open()) {
            std::cerr << "[prepareStart()] UNABLE TO CREATE FILE at " << JUDGING_PATH << '\n';
            exit(-24);
        }

        QFile defJudgingFile(DEFAULT_JUDGING_PATH);

        if (!defJudgingFile.open(QIODeviceBase::ReadOnly)) {
            std::cerr << "[prepareStart()] UNABLE TO READ Qt RESOURCE at " << DEFAULT_JUDGING_PATH.toStdString() << '\n';
            exit(-24);
        }
    }

    if (!std::filesystem::exists(dirPath + WSGI_PATH)) {
        // Niceeeeee
        std::fstream wsgiFile(dirPath + WSGI_PATH, std::ios::out | std::ios::trunc);
        if (!wsgiFile.is_open()) {
            std::cerr << "[prepareStart()] UNABLE TO CREATE FILE at " << WSGI_PATH << '\n';
            exit(-24);
        }

        QFile defWsgiFile(DEFAULT_WSGI_PATH);

        if (!defWsgiFile.open(QIODeviceBase::ReadOnly)) {
            std::cerr << "[prepareStart()] UNABLE TO READ Qt RESOURCE at " << DEFAULT_WSGI_PATH.toStdString() << '\n';
            exit(-24);
        }
    }

    if (!std::filesystem::exists(dirPath + WEBSERVER_PATH)) {
        // Niceeeeee
        std::fstream webserverFile(dirPath + WEBSERVER_PATH, std::ios::out | std::ios::trunc);
        if (!webserverFile.is_open()) {
            std::cerr << "[prepareStart()] UNABLE TO CREATE FILE at " << WEBSERVER_PATH << '\n';
            exit(-24);
        }

        QFile defWebserverFile(DEFAULT_WEBSERVER_PATH);

        if (!defWebserverFile.open(QIODeviceBase::ReadOnly)) {
            std::cerr << "[prepareStart()] UNABLE TO READ Qt RESOURCE at " << DEFAULT_WEBSERVER_PATH.toStdString() << '\n';
            exit(-24);
        }
    }
}


#ifdef _WIN32
// This is for the terminate process thing
#include <windows.h>
#include <tlhelp32.h>
inline bool terminateProcess(int pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        return false;
    }
    bool result = TerminateProcess(hProcess, 1);
    CloseHandle(hProcess);
    return result;
}

inline bool terminateProcessByName(const std::string& processName) {
    bool success = false;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, std::wstring(processName.begin(), processName.end()).c_str()) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (hProcess) {
                    if (TerminateProcess(hProcess, 1)) {
                        success = true;
                    }
                    CloseHandle(hProcess);
                }
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
    return success;
}

// Windows: automatically assume YES
inline bool isRoot() {
    return true;
}
#else
// Linux / Unix / macOS
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>

inline bool terminateProcess(int pid) {
    return kill(pid, SIGTERM) == 0;
}

inline bool terminateProcessByName(const std::string& processName) {
    // Portable way: use system pkill (works on Linux/Unix/macOS)
    std::string cmd = "pkill -TERM " + processName;
    int ret = system(cmd.c_str());
    return (ret == 0);
}

#include <unistd.h>
inline bool isRoot() {
    return (geteuid() == 0);
}
#endif

#endif