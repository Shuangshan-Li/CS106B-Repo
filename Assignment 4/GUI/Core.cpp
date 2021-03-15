#include "Core.h"
#include "BarrierGUI.h"
#include "GUIMain.h"
#include "ConsoleMain.h"
#include "filelib.h"
#include <algorithm>
#include <map>
#include <set>
using namespace std;

/* Get all #defines out of the config file. */
#define RUN_TESTS_MENU_OPTION()
#define MENU_ORDER(...)
#define WINDOW_TITLE(title)
#define TEST_ORDER(...)
#define TEST_BARRIER(demo, ...)
#include "../Demos/GUIConfig.h"
#undef WINDOW_TITLE
#undef MENU_OPTION


/* Capture any #defines. */
#ifdef MG_CONSOLE_MODE
    #define MG_INTERNAL_CONSOLE_MODE
    #undef MG_CONSOLE_MODE

    /* Activate the console in console mode. */
    #include "console.h"
#endif

/* Undef any #defines so that we can import again in the future. */
#undef WINDOW_TITLE
#undef TEST_BARRIER
#undef MENU_ORDER
#undef TEST_ORDER
#undef RUN_TESTS_MENU_OPTION

/* Use X Macro Trick to get a list of all filenames. */
namespace {
    const vector<string>& demoFileOrder() {
        static const vector<string> kAllFiles = {
            #define RUN_TESTS_MENU_OPTION() "TestingGUI.cpp",
            #define MENU_ORDER(...) __VA_ARGS__,
            #define TEST_ORDER(...)
            #define TEST_BARRIER(demo, ...)
            #define WINDOW_TITLE(title)

            #include "../Demos/GUIConfig.h"

            #ifdef MG_CONSOLE_MODE
            #undef MG_CONSOLE_MODE
            #endif

            #undef TEST_ORDER
            #undef TEST_BARRIER
            #undef WINDOW_TITLE
            #undef MENU_ORDER
            #undef RUN_TESTS_MENU_OPTION
        };
        return kAllFiles;
    }

    const vector<string>& testFileOrder() {
        static const vector<string> kAllFiles = {
            #define RUN_TESTS_MENU_OPTION()
            #define MENU_ORDER(...)
            #define TEST_ORDER(...) __VA_ARGS__
            #define TEST_BARRIER(demo, ...)
            #define WINDOW_TITLE(title)

            #include "../Demos/GUIConfig.h"

            #ifdef MG_CONSOLE_MODE
            #undef MG_CONSOLE_MODE
            #endif

            #undef TEST_BARRIER
            #undef TEST_ORDER
            #undef WINDOW_TITLE
            #undef MENU_ORDER
            #undef RUN_TESTS_MENU_OPTION
        };
        return kAllFiles;
    }

    /* Returns the index of the file in the filename list, or a large value if it isn't present. */
    size_t fileIndex(const string& filename) {
        return find(demoFileOrder().begin(), demoFileOrder().end(), filename) - demoFileOrder().begin();
    }

    /* Compares two filenames, ordering by their appearance in the demo files list. */
    int demoFileCompare(const string& lhs, const string& rhs) {
        /* Compare filename ordering. */
        size_t leftIndex  = fileIndex(lhs);
        size_t rightIndex = fileIndex(rhs);
        if (leftIndex < rightIndex) return -1;
        if (leftIndex > rightIndex) return +1;

        /* If the filenames have the same index but the filenames themselves
         * are different, tiebreak on that.
         */
        if (lhs < rhs) return -1;
        if (lhs > rhs) return +1;
        return 0;
    }
}

/* Graphics handlers. */
namespace {
    struct GraphicsHandler {
        string filename;
        size_t line;
        string name;
        function<shared_ptr<ProblemHandler>(GWindow&)> callback;
    };

    /* Master list of all graphics handlers; populated by static initializers. */
    vector<GraphicsHandler>& graphicsHandlers() {
        static vector<GraphicsHandler> theRawHandlers;
        return theRawHandlers;
    };

    #ifndef MG_INTERNAL_CONSOLE_MODE
        /* Sorts the handlers first by filename, then by line. */
        void sortGraphicsHandlers() {
            static bool sorted = false;
            if (!sorted) {
                sort(graphicsHandlers().begin(), graphicsHandlers().end(),
                     [](const GraphicsHandler& lhs, const GraphicsHandler& rhs) {
                        int fileComp = demoFileCompare(lhs.filename, rhs.filename);
                        if (fileComp != 0) {
                            return fileComp < 0;
                        }

                        /* Same file order, same file. Now tiebreak by line. */
                        return lhs.line < rhs.line;
                });

                sorted = true;
            }
        }
    #endif
}

/* Console handlers. */
namespace {
    struct ConsoleHandler {
        string filename;
        size_t line;
        string name;
        function<void()> callback;
    };

    /* Master list of all graphics handlers; populated by static initializers. */
    vector<ConsoleHandler>& consoleHandlers() {
        static vector<ConsoleHandler> theRawHandlers;
        return theRawHandlers;
    };

    #ifdef MG_INTERNAL_CONSOLE_MODE
        /* Sorts the handlers first by filename, then by line. */
        void sortConsoleHandlers() {
            static bool sorted = false;
            if (!sorted) {
                sort(consoleHandlers().begin(), consoleHandlers().end(),
                     [](const ConsoleHandler& lhs, const ConsoleHandler& rhs) {
                        int fileComp = demoFileCompare(lhs.filename, rhs.filename);
                        if (fileComp != 0) {
                            return fileComp < 0;
                        }

                        /* Same file order, same file. Now tiebreak by line. */
                        return lhs.line < rhs.line;
                });

                sorted = true;
            }
        }
    #endif
}

/* Use X Macro Trick a second time to define the window title. */
static string kWindowTitle =
    #define MENU_ORDER(...)
    #define RUN_TESTS_MENU_OPTION()
    #define WINDOW_TITLE(title) title
    #define TEST_ORDER(...)
    #define CONSOLE_MODE()
    #define TEST_BARRIER(demo, ...)

    #include "../Demos/GUIConfig.h"

    #ifdef MG_CONSOLE_MODE
    #undef MG_CONSOLE_MODE
    #endif

    #undef CONSOLE_MODE
    #undef TEST_BARRIER
    #undef WINDOW_TITLE
    #undef GUI_HANDLER
    #undef MENU_ORDER
    #undef TEST_ORDER
;

/* Installers. */
namespace MiniGUI {
    namespace Detail {
        GraphicsInstaller::GraphicsInstaller(FileKey key, const char* name,
                                             function<shared_ptr<ProblemHandler>(GWindow&)> callback) {
            /* Confirm that the file in question exists in the file list. If not, don't include it
             * in the list of options, since it may be private or disabled.
             */
            if (find(demoFileOrder().begin(), demoFileOrder().end(), getTail(key.filename)) != demoFileOrder().end()) {
                graphicsHandlers().push_back({
                    getTail(key.filename),
                    key.line,
                    name,
                    callback
                });
            }
        }

        ConsoleInstaller::ConsoleInstaller(FileKey key, const char* name,
                                           function<void()> callback) {
            /* Confirm that the file in question exists in the file list. If not, don't include it
             * in the list of options, since it may be private or disabled.
             */
            if (find(demoFileOrder().begin(), demoFileOrder().end(), getTail(key.filename)) != demoFileOrder().end()) {
                consoleHandlers().push_back({
                    getTail(key.filename),
                    key.line,
                    name,
                    callback
                });
            }
        }
    }
}

/* Test barriers. */
namespace {
    const map<string, set<string>> kTestBarriers = {
        #define MENU_ORDER(...)
        #define RUN_TESTS_MENU_OPTION()
        #define WINDOW_TITLE(title)
        #define TEST_ORDER(...)
        #define CONSOLE_MODE()
        #define TEST_BARRIER(demo, ...) { demo, {__VA_ARGS__} },

        #include "../Demos/GUIConfig.h"

        #ifdef MG_CONSOLE_MODE
        #undef MG_CONSOLE_MODE
        #endif

        #undef CONSOLE_MODE
        #undef TEST_BARRIER
        #undef WINDOW_TITLE
        #undef GUI_HANDLER
        #undef MENU_ORDER
        #undef TEST_ORDER
    };
}

namespace MiniGUI {
    namespace Config {
        /* Title of the program. */
        std::string programTitle() {
            return kWindowTitle;
        }

        /* List of menu options. */
        vector<MenuOption> menuOptions() {
            vector<MenuOption> result;
            #ifdef MG_INTERNAL_CONSOLE_MODE
                sortConsoleHandlers();
                for (const auto& entry: consoleHandlers()) {
                    auto itr = kTestBarriers.find(entry.filename);
                    if (itr != kTestBarriers.end()) {
                        result.push_back({ entry.name, ifPassedThen(itr->second, entry.callback)});
                    } else {
                        result.push_back({ entry.name, entry.callback });
                    }
                }
            #else
                sortGraphicsHandlers();
                for (const auto& entry: graphicsHandlers()) {
                    auto constructor = entry.callback;

                    /* If we have a test barrier, put it up before invoking the constructor. */
                    auto itr = kTestBarriers.find(entry.filename);
                    if (itr != kTestBarriers.end()) {
                        constructor = ifPassedThen(itr->second, entry.callback);
                    }

                    /* Wrap callbacks into something that installs into the graphics window. */
                    result.push_back({ entry.name, [=]() {
                        MiniGUI::Detail::setActiveDemo(constructor(MiniGUI::Detail::graphicsWindow()));
                    }});
                }
            #endif

            return result;
        }

        /* List of files without callbacks, in case you want that. */
        vector<string> testOrder() {
            return testFileOrder();
        }
    }
}

int main() {
    #ifdef MG_INTERNAL_CONSOLE_MODE
        MiniGUI::Detail::consoleMain();
    #else
        MiniGUI::Detail::graphicsMain();
    #endif

    return 0;
}
