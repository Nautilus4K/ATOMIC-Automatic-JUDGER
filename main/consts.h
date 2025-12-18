#ifndef CONSTS_H
#define CONSTS_H

#include <string>
#include <vector>
#include <QtCore/QString>
#include <filesystem>

// Constants
// -> Values
#define NONE_PLACEHOLDER -1
#define OLLAMA_MAXRETRIES 10

// -> Extensions
const std::vector<std::string> supportedExtensions = {".cpp", ".py", ".pas"};
const std::string logExt = ".log";

// -> Paths
static const std::string THEME_PATH               = "/source/theme.qss";
static const std::string THEMECOLORS_PATH         = "/source/theme_color.opt";
static const QString     DEFAULT_THEME_PATH       = ":/meta/theme.qss";
static const QString     DEFAULT_THEMECOLORS_PATH = ":/meta/theme_color.opt";

static const std::string SETTINGS_PATH        = "/source/settings.json";
static const std::string VERSION_PATH         = "/source/version.json";
static const QString     DEFAULT_VERSION_PATH = ":/meta/version.json";
static const std::string ALIAS_PATH           = "/source/aliases.json";
static const std::string CLASSES_PATH         = "/source/classes.json";
static const std::string CONTESTS_PATH        = "/source/contests.json";
static const std::string USERDATA_PATH        = "/source/users.json";

static const std::string USERSTATS_DIR       = "/workspace/result/";
static const std::string USERQUEUE_DIR       = "/workspace/queue/";
static const std::string USERSUBHISTORY_DIR  = "/userdata/";
static const std::string SUBMITLOG_DIR       = "/workspace/result/logs/";  // EVERY SINGLE DIRECTORY PATH NEEDS TO HAVE A TRAILING `/`

static const QString     PKEXEC_PATH            = "/usr/bin/pkexec";
static const std::string PYDIR                  = "/pythonenv/Scripts/python";
static const std::string JUDGING_PATH           = "/judge.py";
static const std::string WSGI_PATH              = "/wsgi_interface.py";
static const std::string WEBSERVER_PATH         = "/apache.py";
static const QString     DEFAULT_JUDGING_PATH   = ":/scripts/judge.py";
static const QString     DEFAULT_WSGI_PATH      = ":/scripts/wsgi_interface.py";
static const QString     DEFAULT_WEBSERVER_PATH = ":/scripts/apache.py";

static const std::string LOG_PATH            = "/central/valkyrie.log";
static const std::string SESSIONS_PATH       = "/central/sessions.json";
static const std::string STATUS_PATH         = "/central/status.json";
static const std::string LASTACCESS_PATH     = "/central/lastaccess.json";
static const std::string CENTRAL_DIR         = "/central";
static const QString     ICON_PATH           = ":/images/icon.ico";
static const QString     EXITICON_PATH       = ":/images/exit.png";
static const QString     CONTESTSICON_PATH   = ":/images/contests.png";
static const QString     DELETEICON_PATH     = ":/images/delete.png";
static const QString     EDITICON_PATH       = ":/images/edit.png";
static const QString     USERSICON_PATH      = ":/images/users.png";
static const QString     CLASSESICON_PATH    = ":/images/classes.png";
static const QString     SUBMITDIRICON_PATH  = ":/images/submitdir.png";
static const QString     TOEXCELICON_PATH    = ":/images/toexcel.png";
static const QString     FROMEXCELICON_PATH  = ":/images/fromexcel.png";
static const QString     BACKUPICON_PATH     = ":/images/backup.png";
static const QString     LOADBACKUPICON_PATH = ":/images/loadbackup.png";
static const QString     AIGENICON_PATH      = ":/images/aigen.png";
static const QString     SPLASHIMAGE_PATH    = ":/images/splash.png";

static const QString     MONOSPACEFONT_PATH  = ":/fonts/CascadiaCode.ttf";
static const QString     DEFAULTFONT_PATH    = ":/fonts/OpenSans.ttf";

// -> Webserver packaged data paths
// first value will interface with QFile, therefore using QString
// second value, on the other hand, will interface with fstream and stdio, therefore using std::string
static const std::map<QString, std::string> WEBDATA_PATHS = {
   {":/www/assets/dark.png",                "/www/assets/dark.png",              },
   {":/www/assets/light.png",               "/www/assets/light.png",             },
   {":/www/assets/defaultuser.png",         "/www/assets/defaultuser.png",       },
   {":/www/assets/favicon.png",             "/www/assets/favicon.png",           },
   {":/www/js/contest.js",                  "/www/js/contest.js",                },
   {":/www/js/general.js",                  "/www/js/general.js",                },
   {":/www/js/generalDeferred.js",          "/www/js/generalDeferred.js",        },
   {":/www/js/home.js",                     "/www/js/home.js",                   },
   {":/www/js/login.js",                    "/www/js/login.js",                  },
   {":/www/js/scoreboard.js",               "/www/js/scoreboard.js",             },
   {":/www/js/useroptions.js",              "/www/js/useroptions.js",            },
   {":/www/reserved/contest.html",          "/www/reserved/contest.html",        },
   {":/www/reserved/error.html",            "/www/reserved/error.html",          },
   {":/www/reserved/statsbar.html",         "/www/reserved/statsbar.html",       },
   {":/www/reserved/userprofile.html",      "/www/reserved/userprofile.html",    },
   {":/www/styles/fonts/CascadiaCode.ttf",  "/www/styles/fonts/CascadiaCode.ttf",},
   {":/www/styles/contest.css",             "/www/styles/contest.css",           },
   {":/www/styles/error.css",               "/www/styles/error.css",             },
   {":/www/styles/general.css",             "/www/styles/general.css",           },
   {":/www/styles/home.css",                "/www/styles/home.css",              },
   {":/www/styles/login.css",               "/www/styles/login.css",             },
   {":/www/styles/scoreboard.css",          "/www/styles/scoreboard.css",        },
   {":/www/styles/useroptions.css",         "/www/styles/useroptions.css",       },
   {":/www/styles/userprofile.css",         "/www/styles/userprofile.css",       },
   {":/www/.html",                          "/www/.html",                        },
   {":/www/login.html",                     "/www/login.html",                   },
   {":/www/scoreboard.html",                "/www/scoreboard.html",              },
   {":/www/thanks.txt",                     "/www/thanks.txt",                   },
   {":/www/useroptions.html",               "/www/useroptions.html",             }
};
static const std::string ACEEDITOR_DIR = "/www/js/ext";

// -> Dynamic Paths
static const std::string dirPath = std::filesystem::current_path().string();

// -> Qt Style Sheet
static const QString     STYLE_BIGLABEL      = "font-size: 16px; font-weight: bold;";
static const QString     STYLE_BOLDLABEL     = "font-weight: bold;";
static const QString     STYLE_SMALLALEL     = "font-size: 11px; font-style: italic;";

// -> Others
static const std::string GITHUB_PAGE         = "\"https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER\"";
static const int         JUDGING_EXITPORT    = 28472;
static const int         WEBSERVER_EXITPORT  = 28473;
static const std::string JUDGING_EXITADDR    = "127.0.0.1";
static const std::string WEBSERVER_EXITADDR  = "127.0.0.1";
static const std::string SHA256_PASSWD_123   = "a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3";
static const std::string LICENSING           = R"(
====== Giấy phép ======

Phần mềm này được phát hành dưới Giấy phép Mã nguồn mở MIT (MIT License) đã qua sửa đổi. Thông tin chi tiết truy cập trang dự án GitHub.

====== Ghi nhận ======

Phần mềm được phát triển dựa trên hoặc sử dụng các dự án mã nguồn mở sau:

- Qt GUI Framework (https://qt.io)
- Python Interpreter (https://python.org)
- waitress (https://github.com/Pylons/waitress)
- Docker (https://docker.com)
- nlohmann/json - JSON for Modern C++ (https://github.com/nlohmann/json)
- boppreh/keyboard - Python keyboard library (https://github.com/boppreh/keyboard)
- Cascadia Code Font (https://github.com/microsoft/cascadia-code)
- tfussell/xlnt: Cross-platform user-friendly xlsx library for C++11+ (https://github.com/tfussell/xlnt)
- OpenSSL: The OpenSSL software library is a robust, commercial-grade, full-featured toolkit for general-purpose cryptography and secure communication. (https://openssl-library.org/)
)";

// -> ANSI Color codes
static const std::string INFO_COL  = "\x1b[0m";
static const std::string ERROR_COL = "\x1b[31m";
static const std::string WARN_COL  = "\x1b[33m";
static const std::string OK_COL    = "\x1b[32m";
static const std::string RESET_COL = "\x1b[0m";

// -> Excel colors

// Adding a whole ass struct just to deal with colors easier
// It's me boys
struct EXCELCOLOR {
    int r, g, b;
};

static const EXCELCOLOR XLSX_HEADERHEXCOL       = {0, 32, 96};
static const EXCELCOLOR XLSX_HEADERTEXTHEXCOL   = {255, 255, 255};

static const EXCELCOLOR XLSX_USERROWODDFILL     = {255, 255, 255}; // Interwoven colors
static const EXCELCOLOR XLSX_USERROWEVENFILL    = {214, 235, 255};
static const EXCELCOLOR XLSX_USERSUMCELL        = {176, 216, 255};

// -> Some characters related to the backup functionality
static const char       BACKUP_SEPARATOR           = (char)30;
static const char       BACKUP_SECTIONMARKER       = (char)31;
static const char       BACKUP_EACHVALUE_SEPARATOR = (char)6; // Each value separator is the separator for EACH value
static const char       BACKUP_EACHGROUP_SEPARATOR = (char)29; // Each pair separator is the seprator for EACH pair, which is bigger than each value

static const std::string BACKUP_SANITIZATION_PREMARKER = "/@/";

// -> Ollama AI generation
static const std::string OLLAMA_TARGET_MODEL  = "llama3.1:8b";
static const char        OLLAMA_IO_SPLIT_CHAR = (char)2;
static const char        OLLAMA_NEW_TEST_CHAR = (char)0;
static const std::string OLLAMA_SYSTEM_PROMPT = R"(
You are a Python code generator for competitive programming contests. Your task is to generate Python scripts that create multiple randomized test cases based on the problem description provided by the user. These test cases are intended to evaluate the correctness of scripts (submissions) sent by others. The generated Python script must be automated and use randomness to create diverse test cases, adapting dynamically to the problem description provided.

Each test case must adhere to the following format:

<INPUT><ASCII Character 2><OUTPUT><NEW TEST CHARACTER>

### Instructions:
1. **Output Format**:
   - `<INPUT>`: Represents the input for the test case.
   - `<ASCII Character 2>`: A non-printable ASCII character (code 2). Print it exactly as is (not \x00 but \x02).
   - `<OUTPUT>`: Represents the expected output for the test case.
   - `<NEW TEST CHARACTER>`: A newline character to separate test cases. This should be the good old NULL character (\x00)

2. **Behavior Guidelines**:
   - Do not include any additional formatting, explanations, or comments in the output.
   - Only output Python code; avoid any text outside the code block.
   - Do not respond as if responding to a human. Assume your response is going to be read by a computer.
   - Avoid using emojis or unnecessary Unicode characters.
   - Ensure the code is safe to execute without requiring a container (no system-level or root operations).
   - Do not include Markdown formatting symbols in the code.
   - Surround the generated Python code with Markdown code block delimiters (```).
   - Only have 1 singular code block.
   - Do not add any code that needs input, assume your code is going to be executed automatically to solve that specific problem, do not make it dynamic.

3. **Example**:
   - If the problem is "0 TEST CASES Sum of two integers," the script should generate test cases like:
   ```python
   import random

   def generate_test_cases(num_test_cases):
      for _ in range(num_test_cases):
         a = random.randint(0, 10**9)
         b = random.randint(0, 10**9)
         input_str = f"{a} {b}"
         output = str(a + b)
         print(f"{input_str}\x02{output}\x00")

   generate_test_cases(0)
   ```
   - The incorrect version:
   ```python
   import random

   def generate_test_cases(num_test_cases):
      for _ in range(num_test_cases):
         a = random.randint(0, 10**9)
         b = random.randint(0, 10**9)
         input_str = f"{a} {b}"
         output = str(a + b)
         print(f"{input_str}\x02{output}\x00")

   num_test_cases = int(input("Enter the number of test cases: "))
   generate_test_cases(num_test_cases)

4. **Clarifications**:
   - If the user specifies "Return X and Y," interpret it as the program's output, not a function return statement.
   - Assume the script's output will be processed programmatically, not read by a human.
   - Even if the user queries 0 tests, do it and make a script that returns 0 test cases, don't assume.

Your objective is to generate Python scripts that produce the required test cases in the specified format, adhering strictly to these guidelines. The script must leverage randomness to ensure diverse and comprehensive test coverage.
)";

// -> Platform specifics
#ifdef _WIN32
// This is for when we are on Windows
// ollama is gonna be installed alongside this thing and ran minimally as a cli tool
// Yeah basically running portablly
static const QString OLLAMA_PATH = QString::fromStdString(dirPath + "/ext/ollama/ollama.exe");
static const std::string TEMP_FOLDER = std::getenv("TEMP");
static const bool isWindows = true;
#else
// Yea its not gonna be as much of a problem as on Windows.
static const QString     OLLAMA_PATH = "/usr/bin/ollama";
static const std::string TEMP_FOLDER = "/tmp";
static const bool isWindows = false;
#endif
#endif