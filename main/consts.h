#ifndef CONSTS_H
#define CONSTS_H

#include <string>
#include <vector>
#include <QtCore/QString>
#include <filesystem>

// Constants
// -> Values
#define NONE_PLACEHOLDER -1

// -> Extensions
const std::vector<std::string> supportedExtensions = {".cpp", ".py", ".pas"};
const std::string logExt = ".log";

// -> Paths
static const std::string THEME_PATH = "/source/theme.qss";
static const std::string THEMECOLORS_PATH = "/source/theme_color.opt";

static const std::string SETTINGS_PATH = "/source/settings.json";
static const std::string VERSION_PATH = "/source/version.json";
static const std::string ALIAS_PATH = "/source/aliases.json";
static const std::string CLASSES_PATH = "/source/classes.json";
static const std::string CONTESTS_PATH = "/source/contests.json";
static const std::string USERDATA_PATH = "/source/users.json";

static const std::string USERSTATS_DIR = "/workspace/result/";
static const std::string USERQUEUE_DIR = "/workspace/queue/";
static const std::string USERSUBHISTORY_DIR = "/userdata/";
static const std::string SUBMITLOG_DIR = "/workspace/result/logs/";  // EVERY SINGLE DIRECTORY PATH NEEDS TO HAVE A TRAILING `/`

static const std::string PYDIR = "/python3118/python.exe";
static const std::string JUDGING_PATH = "/judge.py";
static const std::string WEBSERVER_PATH = "/apache.py";

static const std::string LOG_PATH = "/central/valkyrie.log";
static const std::string ICON_PATH = "/icon.ico";
static const QString EXITICON_PATH = ":/images/exit.png";
static const QString CONTESTSICON_PATH = ":/images/contests.png";
static const QString DELETEICON_PATH = ":/images/delete.png";
static const QString EDITICON_PATH = ":/images/edit.png";
static const QString USERSICON_PATH = ":/images/users.png";
static const QString CLASSESICON_PATH = ":/images/classes.png";
static const QString SUBMITDIRICON_PATH = ":/images/submitdir.png";
static const QString TOEXCELICON_PATH = ":/images/toexcel.png";
static const QString FROMEXCELICON_PATH = ":/images/fromexcel.png";
static const QString BACKUPICON_PATH = ":/images/backup.png";
static const QString LOADBACKUPICON_PATH = ":/images/loadbackup.png";

static const char* DEBUGLOG_PATH = "/debug.log";

// Paths
static const std::string dirPath = std::filesystem::current_path().string();

// -> Qt Style Sheet
static const QString STYLE_BIGLABEL = "font-size: 16px; font-weight: bold;";
static const QString STYLE_BOLDLABEL = "font-weight: bold;";
static const QString STYLE_SMALLALEL = "font-size: 11px; font-style: italic;";

// -> Others
static const std::string GITHUB_PAGE = "\"https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER\"";
static const int JUDGING_EXITPORT = 28472;
static const std::string JUDGING_EXITADDR = "127.0.0.1";
static const int WEBSERVER_EXITPORT = 28473;
static const std::string WEBSERVER_EXITADDR = "127.0.0.1";
static const std::string SHA256_PASSWD_123 = "a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3";

// -> ANSI Color codes
static const std::string INFO_COL = "\x1b[0m";
static const std::string ERROR_COL = "\x1b[31m";
static const std::string WARN_COL = "\x1b[33m";
static const std::string OK_COL = "\x1b[32m";
static const std::string RESET_COL = "\x1b[0m";

// -> Excel colors

// Adding a whole ass struct just to deal with colors easier
// It's me boys
struct EXCELCOLOR {
    int r, g, b;
};

static const EXCELCOLOR XLSX_HEADERHEXCOL = {0, 32, 96};
static const EXCELCOLOR XLSX_HEADERTEXTHEXCOL = {255, 255, 255};

static const EXCELCOLOR XLSX_USERROWODDFILL = {255, 255, 255}; // Interwoven colors
static const EXCELCOLOR XLSX_USERROWEVENFILL = {214, 235, 255};
static const EXCELCOLOR XLSX_USERSUMCELL = {176, 216, 255};

// -> Some characters related to the backup functionality
static const char BACKUP_SEPARATOR = (char)30;
static const char BACKUP_SECTIONMARKER = (char)31;
static const char BACKUP_EACHVALUE_SEPARATOR = (char)6; // Each value separator is the separator for EACH value
static const char BACKUP_EACHGROUP_SEPARATOR = (char)29; // Each pair separator is the seprator for EACH pair, which is bigger than each value

static const std::string BACKUP_SANITIZATION_PREMARKER = "/@/";

#endif