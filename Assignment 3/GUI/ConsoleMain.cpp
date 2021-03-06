#include "Core.h"
#include "ConsoleUtils.h"
#include "TextUtils.h"
#include "MemoryDiagnostics.h"
#include "SimpleTest.h"
#include "TestDriver.h"
#include "error.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include <cstdlib>
using namespace std;

namespace {
    void printIntro() {
        cout << "You have switched to the console window. Press ENTER to continue." << endl;
        (void) getLine();
    }

    int makeMenuSelection() {
        Vector<string> options;
        for (const auto& entry: MiniGUI::Config::menuOptions()) {
            options += entry.name;
        }
        options += "Quit";

        cout << MiniGUI::Config::programTitle() << endl;
        return makeSelectionFrom("Please make a selection:", options);
    }
}

namespace MiniGUI {
    namespace Detail {
        void consoleMain() {
            printIntro();
            do {
                int selection = makeMenuSelection();

                /* Quit is the last option. */
                if (selection == int(MiniGUI::Config::menuOptions().size())) {
                    break;
                }
                MiniGUI::Config::menuOptions()[selection].callback();

                cout << endl;
            } while (getYesOrNo("You are back at the main menu. Would you like to pick again?"));

            cout << endl;
            cout << "Exiting..." << endl;
            
            /* Hard exit the program; the normal exit route can cause segfaults. */
            _Exit(0);
        }
    }
}
