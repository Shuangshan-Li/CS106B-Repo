/* File: OnlyConnect.cpp
 * TODO: "TNT". Hint: A 2020 movie that confuses everybody.
 * Citation: Function "isVowel" is inspired by the textbook(chapter 3).
 */
#include "OnlyConnect.h"
#include "GUI/SimpleTest.h"
#include "strlib.h"
#include <cctype>
using namespace std;

bool isVowel(char ch);

string onlyConnectize(string phrase) {
    if (phrase == "") return string("");
    if (!isalpha(phrase[0]) || isVowel(phrase[0])) {
        return onlyConnectize(phrase.substr(1));
    } else {
        return charToString(toUpperCase(phrase[0])) + onlyConnectize(phrase.substr(1));
    }
}

/* Takes a singer character and return whether it is a vowel*/
bool isVowel(char ch) {
    char res = toupper(ch);
    switch (res) {
     case 'A': case 'E': case 'I': case 'O': case 'U': case 'Y':
        return true;
     default:
        return false;
    }
}





/* * * * * * Provided Test Cases * * * * * */

PROVIDED_TEST("Converts lower-case to upper-case.") {
    EXPECT_EQUAL(onlyConnectize("lowercase"), "LWRCS");
    EXPECT_EQUAL(onlyConnectize("uppercase"), "PPRCS");
}

PROVIDED_TEST("Handles non-letter characters properly.") {
    EXPECT_EQUAL(onlyConnectize("2.718281828459045"), "");
    EXPECT_EQUAL(onlyConnectize("'Hi, Mom!'"), "HMM");
}

PROVIDED_TEST("Handles single-character inputs.") {
    EXPECT_EQUAL(onlyConnectize("A"), "");
    EXPECT_EQUAL(onlyConnectize("+"), "");
    EXPECT_EQUAL(onlyConnectize("Q"), "Q");
}

STUDENT_TEST("Handles void inputs properly.") {
    EXPECT_EQUAL(onlyConnectize(""), "");
}






