/*  Description: This function takes as input an integer n and a Vector to fill successful arrangement (If there is one).
 *               It return whether a perfectSquare arrangement an be achieved
 *
**/
#include "GUI/SimpleTest.h"
#include "error.h"
#include "vector.h"
#include "set.h"
using namespace std;

/* We've provided this function to you; you don't need to implement it. */
bool isPerfectSquare(int value);

bool dfs(int n, Vector<int>& sequence, const Vector<int>& chosen, Set<int>& seen) {
    //if we have used all numbers from 1 to n, we have a successful arrangement!
    if (seen.size() == n) {
        sequence += chosen;
        return true;
    }
    //at every position, explore all valid choices
    for (int i = 1; i <= n; i++) {
        //if a number has already been used, it cannot be a valid choice
        if (seen.contains(i)) continue;
        //if a number pluing the previous choice does not yield a perfect square number, it cannot be a valid choice
        if (chosen.size() > 0 && !isPerfectSquare(chosen.last() + i)) continue;
        //maek our choice
        seen.add(i);
        //if this choice eventually leads to a successful arrangement, we are done!
        if (dfs(n, sequence, chosen + i, seen)) return true;
        //unmark our choice
        seen.remove(i);

    }
    return false;
}



bool hasSquareSequence(int n, Vector<int>& sequence) {
    //n should at least be 1
    if (n <= 0) error("Nah!!! n should be at least 1");
    //use a set to track the choices we have made
    Set<int> seen;
    return dfs(n, sequence, {}, seen);
}

/* Space for test cases. You are not required to write any test cases, but you
 * are encouraged to do so because our test coverage is not as extensive as what
 * you saw in Assignment 4 or the first midterm exam.
 */
