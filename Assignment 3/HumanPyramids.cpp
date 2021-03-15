#include "HumanPyramids.h"
#include "GUI/SimpleTest.h"
#include "hashmap.h"
#include "gridlocation.h"
using namespace std;

const int WEIGHT = 160;

//HashMap gives constant time acess to the kee value pair inside.
double weightOnBackOfRec(int row, int col, int pyramidHeight, HashMap<GridLocation, double>& table) {
    if (row < 0 || col < 0 || row > (pyramidHeight - 1) || col > row) {
        error("This coordinate is out of bounds!");
    }
    if (row == 0 && col == 0) {
        return 0.0;
    }
    // Because the pyramid is symmetric, persons at symmetric location have same weight on back of them
    GridLocation person(row, col);
    GridLocation personSymmetric(row, row - col);
    if (table.containsKey(person)) {
        return table[person];
    } else {
        double result = 0;
        if (col == 0 || col == row) {
            // Because each person has the same weight and the pyramid is symmertcial,
            // persons on the rightmost side can be treated equally as persons on the
            // leftmost side.
            result += (WEIGHT + weightOnBackOfRec(row - 1, 0, pyramidHeight, table)) / 2;
        } else {
            result += (WEIGHT + weightOnBackOfRec(row - 1, col - 1, pyramidHeight, table)) / 2;
            result += (WEIGHT + weightOnBackOfRec(row - 1, col, pyramidHeight, table)) / 2;
        }
        table.put(person, result);
        table.put(personSymmetric, result);
        return result;
    }
}

double weightOnBackOf(int row, int col, int pyramidHeight) {
    HashMap<GridLocation, double> table;
    return weightOnBackOfRec(row, col, pyramidHeight, table);
}


/* * * * * * Test Cases * * * * * */

STUDENT_TEST("Check base case, namely person A from the handout") {
    EXPECT_EQUAL(weightOnBackOf(0, 0, 5), 0.0);
}

STUDENT_TEST("Check persons of the same row but are are the leftmost and rightmost respectively can be treated equally") {
    EXPECT_EQUAL(weightOnBackOf(200, 0, 100000), weightOnBackOf(200, 200, 100000));
}

/* * * * * * Test cases from the starter files below this point. * * * * * */

PROVIDED_TEST("Check Person E from the handout.") {
    /* Person E is located at row 2, column 1. */
    EXPECT_EQUAL(weightOnBackOf(2, 1, 5), 240);
}

PROVIDED_TEST("Function reports errors in invalid cases.") {
    EXPECT_ERROR(weightOnBackOf(-1, 0, 10));
    EXPECT_ERROR(weightOnBackOf(10, 10, 5));
    EXPECT_ERROR(weightOnBackOf(-1, 10, 20));
}

PROVIDED_TEST("Stress test: Memoization is implemented.") {
    /* TODO: Yes, we are asking you to make a change to this test case! Delete the
     * line immediately after this one - the one that starts with SHOW_ERROR - once
     * you have implemented memoization to test whether it works correctly.
     */
    /* Do not delete anything below this point. :-) */

    /* This will take a LONG time to complete if memoization isn't implemented.
     * We're talking "heat death of the universe" amounts of time. :-)
     */
    EXPECT(weightOnBackOf(100, 50, 200) >= 10000);
}

