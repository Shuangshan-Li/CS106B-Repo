/*  Description: function 'scoreFor' takes as inputs a roadNetwork and an arrangement of attractions, returns
 *               the total score of this roadNetwork.
 *
 *               function 'bestAttractionsFor' takes a roadNetwork in as an input and return the arrangement of
 *               attractions that gives this roadNetwork the highest total scores possible.
**/

#include "GUI/SimpleTest.h"
#include "map.h"
#include "set.h"
#include <string>
using namespace std;

/* List of all types of attractions. You aren't required to use this list,
 * but we figured it might be helpful.
 */
const Vector<string> kAllAttractions = {
    "Park",
    "Museum",
    "Stadium"
};

/* %% Place all the code you want us to grade below this line. Do not edit this comment. %% */




int scoreFor(const Map<string, Set<string>>& roadNetwork,
             const Map<string, string>& attractions) {
    int total = 0;
    for (string city: roadNetwork) {
        for (string neighbor: roadNetwork[city]) {
            if (attractions[city] != attractions[neighbor]) {
                total += 1;
            }
        }
    }
    return total;
}

void bestAttractionsForRec(const Map<string, Set<string>>& roadNetwork,
                                          Set<string> remainingCities,
                                          const Map<string, string>& soFar,
                                          Map<string, string>& attractions) {
    //Base case is when all cities have an attraction
    if (remainingCities.isEmpty()) {
        //If this arrangement is better, we update attractions with soFar.
        if (attractions.isEmpty() || scoreFor(roadNetwork, soFar) > scoreFor(roadNetwork, attractions)) {
            attractions = soFar;
        }
        return;
    }
    string curr = remainingCities.first();
    for (string choice: kAllAttractions) {
        auto newSoFar = soFar;
        newSoFar[curr] = choice;
        bestAttractionsForRec(roadNetwork, remainingCities - curr, newSoFar, attractions);
    }


}


Map<string, string> bestAttractionsFor(const Map<string, Set<string>>& roadNetwork) {
    Set<string> remainingCities;
    for (string city: roadNetwork) {
        remainingCities += city;
    }
    Map<string, string> soFar;
    Map<string, string> attractions;
    bestAttractionsForRec(roadNetwork, remainingCities, soFar, attractions);
    return attractions;
}





/* %% Place all the code you want us to grade above this line. Do not edit this comment. %% */

/* * * * * Tests Below This Point * * * * */

PROVIDED_TEST("scoreFor works for an isolated city.") {
    Map<string, Set<string>> roads = {
        { "Solipsist", {} }
    };
    Map<string, string> attractions = {
        { "Solipsist", "Museum" }
    };

    EXPECT_EQUAL(scoreFor(roads, attractions), 0);
}

PROVIDED_TEST("scoreFor works for two adjacent cities.") {
    Map<string, Set<string>> roads = {
        { "Castor", { "Pollux" } },
        { "Pollux", { "Castor" } }
    };

    /* Initially, give the same attraction. */
    Map<string, string> attractions = {
        { "Castor", "Museum" },
        { "Pollux", "Museum" }
    };
    EXPECT_EQUAL(scoreFor(roads, attractions), 0);

    /* Now, have each do something different. */
    attractions["Castor"] = "Stadium";
    attractions["Pollux"] = "Park";

    /* Each city has a score of one, so the total score will be 2. */
    EXPECT_EQUAL(scoreFor(roads, attractions), 2);
}

PROVIDED_TEST("scoreFor works for three cities in a triangle.") {
    Map<string, Set<string>> roads = {
        { "Executive",   { "Legislative", "Judicial" } },
        { "Legislative", { "Executive",   "Judicial" } },
        { "Judicial",    { "Legislative", "Executive" } },
    };

    /* Initially, give the same attraction. */
    Map<string, string> attractions = {
        { "Executive",   "Park" },
        { "Legislative", "Park" },
        { "Judicial",    "Park" },
    };
    EXPECT_EQUAL(scoreFor(roads, attractions), 0);

    /* Now, have each do something different. */
    attractions["Executive"]   = "Stadium";
    attractions["Legislative"] = "Museum";

    /* Each city has a score of two, so the total score will be 6. */
    EXPECT_EQUAL(scoreFor(roads, attractions), 6);

    /* Now, give two one attraction and the third another. */
    attractions["Executive"] = "Park";

    /* Two cities have a score of one; one city has a score of two. */
    EXPECT_EQUAL(scoreFor(roads, attractions), 4);
}

PROVIDED_TEST("scoreFor works for three cities in a line.") {
    Map<string, Set<string>> roads = {
        { "Alif", { "Ba"} },
        { "Ba",   { "Alif",   "Ta" } },
        { "Ta",   { "Ba" } },
    };

    /* Initially, give the same attraction. */
    Map<string, string> attractions = {
        { "Alif", "Park" },
        { "Ba",   "Park" },
        { "Ta",   "Park" },
    };
    EXPECT_EQUAL(scoreFor(roads, attractions), 0);

    /* Now, have each do something different. */
    attractions["Ba"] = "Stadium";
    attractions["Ta"] = "Museum";

    /* Alif and Ta have a score of one; Ba has a score of two. */
    EXPECT_EQUAL(scoreFor(roads, attractions), 4);

    /* Give Alif and Ta the same attraction and Ba a different one. */
    attractions["Ta"] = "Park";

    /* Alif and Ta have a score of one; Ba has a score of two. */
    EXPECT_EQUAL(scoreFor(roads, attractions), 4);

    /* Give Alif and Ba the same attraction and Ta a different one. */
    attractions["Alif"] = "Stadium";

    /* Alif scores zero; Ba and Ta score one. */
    EXPECT_EQUAL(scoreFor(roads, attractions), 2);
}

PROVIDED_TEST("scoreFor works for three isolated cities.") {
    Map<string, Set<string>> roads = {
        { "Ani",  { } },
        { "Bani", { } },
        { "Gani", { } },
    };

    /* Initially, give the same attraction. */
    Map<string, string> attractions = {
        { "Ani", "Park" },
        { "Bani",   "Park" },
        { "Gani",   "Park" },
    };
    EXPECT_EQUAL(scoreFor(roads, attractions), 0);

    /* Now, have each do something different. */
    attractions["Bani"] = "Stadium";
    attractions["Gani"] = "Museum";

    /* Scores should still be zero. */
    EXPECT_EQUAL(scoreFor(roads, attractions), 0);

    /* Give two cities the same attraction. */
    attractions["Gani"] = "Park";

    /* Score should still be zero. */
    EXPECT_EQUAL(scoreFor(roads, attractions), 0);
}

/* This is a helper function that's useful for designing test cases. You give it a Map
 * of cities and what they're adjacent to, and it then produces a new Map where if city
 * A links to city B, then city B links back to city A.
 */
Map<string, Set<string>> makeSymmetric(const Map<string, Set<string>>& source) {
    Map<string, Set<string>> result = source;

    for (const string& from: source) {
        for (const string& to: source[from]) {
            result[from] += to;
            result[to] += from;
        }
    }

    return result;
}

PROVIDED_TEST("scoreFor works for the second example from the description.") {
    Map<string, Set<string>> roads = makeSymmetric({
        { "A", { "B", "C", "E" } },
        { "B", { "C", "E" } },
        { "C", { "D", "E", "F", "G" } },
        { "D", { "G" } },
        { "F", { "G" } }
    });

    Map<string, string> attractions = {
        { "A", "Park" },
        { "B", "Stadium" },
        { "C", "Stadium" },
        { "D", "Park" },
        { "E", "Museum" },
        { "F", "Park" },
        { "G", "Museum" },
    };

    EXPECT_EQUAL(scoreFor(roads, attractions), 20);
}

PROVIDED_TEST("bestAttractionsFor works on a single, isolated city.") {
    Map<string, Set<string>> roads = {
        { "Solipsist", {} }
    };

    /* There's no way for the score to be anything other than zero. */
    auto result = bestAttractionsFor(roads);
    EXPECT_EQUAL(result.size(), 1);
    EXPECT(result.containsKey("Solipsist"));
    EXPECT(result["Solipsist"] == "Stadium" ||
           result["Solipsist"] == "Park" ||
           result["Solipsist"] == "Museum");
}

PROVIDED_TEST("bestAttractionsFor works for two adjacent cities.") {
    Map<string, Set<string>> roads = {
        { "Castor", { "Pollux" } },
        { "Pollux", { "Castor" } }
    };

    auto result = bestAttractionsFor(roads);
    EXPECT_EQUAL(result.size(), 2);
    EXPECT(result.containsKey("Castor"));
    EXPECT(result.containsKey("Pollux"));
    EXPECT(result["Castor"] == "Stadium" ||
           result["Castor"] == "Park" ||
           result["Castor"] == "Museum");
    EXPECT(result["Pollux"] == "Stadium" ||
           result["Pollux"] == "Park" ||
           result["Pollux"] == "Museum");

    /* The two cities should have different attractions. */
    EXPECT(result["Castor"] != result["Pollux"]);
}

PROVIDED_TEST("bestAttractionsFor solves \"Don't Be Greedy\" from the A4 handout") {

    /* Here's the map:
     *
     *     A       E
     *     |       |
     *     B - D - F
     *     | /   \ |
     *     C       G
     *
     * The best solution is to give cities B, C, and D different attractions.
     * From there, give F and G different attractions from D and from each other.
     * Finally, give A and E attractions different from B and F, respectively.
     * This gives a total score of 16.
     */

    Map<string, Set<string>> map = makeSymmetric({
        { "A", { "B" } },
        { "B", { "C", "D" } },
        { "C", { "D" } },
        { "D", { "F", "G" } },
        { "E", { "F" } },
        { "F", { "G" } },
    });

    auto result = bestAttractionsFor(map);
    EXPECT_EQUAL(result.size(), 7);
    EXPECT_EQUAL(scoreFor(map, result), 16);
}

PROVIDED_TEST("bestAttractionsFor works for four mutually adjacent cities.") {
    Map<string, Set<string>> map = makeSymmetric({
        { "A", { "B", "C", "D" } },
        { "B", { "C", "D" } },
        { "C", { "D" } },
    });

    auto result = bestAttractionsFor(map);
    EXPECT_EQUAL(result.size(), 4);

    /* The best option is to have one attraction of each type,
     * plus one duplicate. This means that all pairs of cities,
     * except one, have different attractions.
     */
    EXPECT_EQUAL(scoreFor(map, result), 10);
}

PROVIDED_TEST("bestAttractionsFor works for four isolated cities.") {
    Map<string, Set<string>> map = {
        { "Hoy",  { } },
        { "Lawe", { } },
        { "Hawt", { } },
        { "May",  { } }
    };

    auto result = bestAttractionsFor(map);
    EXPECT(result.containsKey("Hoy"));
    EXPECT(result.containsKey("Lawe"));
    EXPECT(result.containsKey("Hawt"));
    EXPECT(result.containsKey("May"));
    EXPECT_EQUAL(result.size(), 4);

    /* All options are equally good/bad. */
    EXPECT_EQUAL(scoreFor(map, result), 0);
}

#include "filelib.h"
PROVIDED_TEST("Stress test: bestAttractionsFor works for eight mutually adjacent cities. (Should take at most a few seconds.)") {
    /* TODO: When you're ready to run the stress test, comment out the line below this one. */
//    SHOW_ERROR("Stress test is disabled by default. To run it, comment out line " + to_string(__LINE__) + " of " + getTail(__FILE__) + ".");

    Map<string, Set<string>> map = makeSymmetric({
        { "A", { "B", "C", "D", "E", "F", "G", "H" } },
        { "B", { "C", "D", "E", "F", "G", "H" } },
        { "C", { "D", "E", "F", "G", "H" } },
        { "D", { "E", "F", "G", "H" } },
        { "E", { "F", "G", "H" } },
        { "F", { "G", "H" } },
        { "G", { "H" } },
    });

    auto result = bestAttractionsFor(map);
    EXPECT_EQUAL(result.size(), 8);

    /* The best option is to have three cities given one type,
     * three cities given another, and the final two given the
     * third.
     *
     * Cities in the first two groups each have a score of 5 (30 points),
     * and cities in the last group each have a score of 6 (12 points).
     */
    EXPECT_EQUAL(scoreFor(map, result), 42);
}
