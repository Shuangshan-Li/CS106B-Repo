/*  Description: This function takes in as inputs the length of a line and a number of people
 *               and returns all the possible arrangement that comply with social distancing rule
**/
#include "GUI/SimpleTest.h"
#include "set.h"
#include "strlib.h"
#include <string>
#include <algorithm>
using namespace std;

/* %% Place all the code you want us to grade below this line. Do not edit this comment. %% */

//Helper function that can do 'string math'
string multiplyString(const string& str, int times) {
    string result = "";
    for (int i = 0; i < times; i++) {
        result += str;
    }
    return result;
}

Set<string> safeArrangementsOfRec(int numDots, int numPeople, const string& soFar) {
    string remaining = "";
    if (numDots == 0) {
        remaining += multiplyString("P", numPeople);
        return {soFar + remaining};
    }
    if (numPeople == 0) {
        remaining += multiplyString(".", numDots);
        return {soFar + remaining};
    }

    Set<string> result;
    //For the first person in line
    if (!endsWith(soFar, "P")) {
        //The number of dots in front of the first person must allow for
        //at least 2 dots between each of the following people
        for (int i = 0; i <= numDots - (numPeople - 1) * 2; i++) {
            string cur = "";
            cur += multiplyString(".", i);
            cur += "P";
            result += safeArrangementsOfRec(numDots - i, numPeople - 1, soFar + cur);
        }
    } else {
        //For a person that is not at the first of the line, the numer of dots in front must
        //satisfy the same distancing rule for the first person, but also, it should be at least
        //2 itself.
        for (int i = 2; i <= numDots - (numPeople - 1) * 2; i++) {
            string cur = "";
            cur += multiplyString(".", i);
            cur += "P";
            result += safeArrangementsOfRec(numDots - i, numPeople - 1, soFar + cur);
        }
    }
    return result;
    for (int i = 0; i <= numDots; i++) {
        string cur = "";
        cur += multiplyString(".", i);
        cur += "P";
        result += safeArrangementsOfRec(numDots - i, numPeople - 1, soFar + cur);
    }
    return result;
}


Set<string> safeArrangementsOf(int lineLength, int numPeople) {
    int numDots = lineLength - numPeople;
    //If there are not enought space for social distancing, no need to arrange at all
    if (numDots < (numPeople - 1) * 2) {
        return {};
    }
    return safeArrangementsOfRec(numDots, numPeople, "");
}




/* %% Place all the code you want us to grade above this line. Do not edit this comment. %% */





/* * * * * Test Cases Below This Point * * * * */

PROVIDED_TEST("Handles one person in small lines.") {
    EXPECT_EQUAL(safeArrangementsOf(0, 1), { });
    EXPECT_EQUAL(safeArrangementsOf(1, 1), {"P"});
    EXPECT_EQUAL(safeArrangementsOf(2, 1), {"P.", ".P"});
    EXPECT_EQUAL(safeArrangementsOf(3, 1), {"P..", ".P.", "..P"});
    EXPECT_EQUAL(safeArrangementsOf(4, 1), {"P...", ".P..", "..P.", "...P"});


}

PROVIDED_TEST("Handles two people in small lines.") {
    EXPECT_EQUAL(safeArrangementsOf(0, 2), { });
    EXPECT_EQUAL(safeArrangementsOf(1, 2), { });
    EXPECT_EQUAL(safeArrangementsOf(2, 2), { });
    EXPECT_EQUAL(safeArrangementsOf(3, 2), { });
    EXPECT_EQUAL(safeArrangementsOf(4, 2), { "P..P" });
}

PROVIDED_TEST("Handles no people in small lines.") {
    EXPECT_EQUAL(safeArrangementsOf(0, 0), {""});
    EXPECT_EQUAL(safeArrangementsOf(1, 0), {"."});
    EXPECT_EQUAL(safeArrangementsOf(2, 0), {".."});
    EXPECT_EQUAL(safeArrangementsOf(3, 0), {"..."});
    EXPECT_EQUAL(safeArrangementsOf(4, 0), {"...."});
}

PROVIDED_TEST("Handles three people in small lines.") {
    EXPECT_EQUAL(safeArrangementsOf(0, 3), { });
    EXPECT_EQUAL(safeArrangementsOf(1, 3), { });
    EXPECT_EQUAL(safeArrangementsOf(2, 3), { });
    EXPECT_EQUAL(safeArrangementsOf(3, 3), { });
    EXPECT_EQUAL(safeArrangementsOf(4, 3), { });
}

PROVIDED_TEST("Handles three people in medium-sized lines.") {
    EXPECT_EQUAL(safeArrangementsOf(7, 3), { "P..P..P" });
    EXPECT_EQUAL(safeArrangementsOf(8, 3), { "P..P..P.",
                                             "P..P...P",
                                             "P...P..P",
                                             ".P..P..P" });
}

PROVIDED_TEST("Handles four people in medium-sized lines.") {
    EXPECT_EQUAL(safeArrangementsOf(12, 4), {
                     "P..P..P..P..",
                     "P..P..P...P.",
                     "P..P..P....P",
                     "P..P...P..P.",
                     "P..P...P...P",
                     "P..P....P..P",
                     "P...P..P..P.",
                     "P...P..P...P",
                     "P...P...P..P",
                     "P....P..P..P",
                     ".P..P..P..P.",
                     ".P..P..P...P",
                     ".P..P...P..P",
                     ".P...P..P..P",
                     "..P..P..P..P",
                 });
}

#include "filelib.h"
PROVIDED_TEST("Stress test: Handles ten people in a reasonably-sized line. (This should take at most a few seconds.)") {
    /* TODO: When you're ready to run the stress test, comment out the line below this one. */
//    SHOW_ERROR("Stress test is disabled by default. To run it, comment out line " + to_string(__LINE__) + " of " + getTail(__FILE__) + ".");

    EXPECT_EQUAL(safeArrangementsOf(32, 10).size(), 1001);
}
