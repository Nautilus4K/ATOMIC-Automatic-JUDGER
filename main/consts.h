#include <string>
#include <vector>
#include <QtCore/QString>
#include <filesystem>

// Constants
// -> Values
#define NONE_PLACEHOLDER -0x7fffffff

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

// Paths
static const std::string dirPath = std::filesystem::current_path().string();

// -> Qt Style Sheet
static const QString STYLE_BIGLABEL = "font-size: 16px; font-weight: bold;";
static const QString STYLE_BOLDLABEL = "font-weight: bold;";

// -> Others
static const std::string GITHUB_PAGE = "\"https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER\"";
static const int JUDGING_EXITPORT = 28472;
static const std::string JUDGING_EXITADDR = "127.0.0.1";
static const int WEBSERVER_EXITPORT = 28473;
static const std::string WEBSERVER_EXITADDR = "127.0.0.1";

// -> ANSI Color codes
static const std::string INFO_COL = "\x1b[0m";
static const std::string ERROR_COL = "\x1b[31m";
static const std::string WARN_COL = "\x1b[33m";
static const std::string OK_COL = "\x1b[32m";
static const std::string RESET_COL = "\x1b[0m";