#include "LeapfrogHashTable.h"
#include "GUI/SimpleTest.h"
#include "Demos/Timer.h"
using namespace std;

PROVIDED_TEST("Basic insertion tests.") {
    /* Ten slots; each item hashes to its last digit. */
    LeapfrogHashTable table(Hash::identity(10));

    /* Insert 18; should get
     *
     *  .  .  .  .  .  .  .  . 18 .
     *                          -
     */
    EXPECT(table.insert("18"));
    EXPECT_EQUAL(table.size(), 1);

    EXPECT_EQUAL(table.elems[0].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[1].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[2].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[3].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[9].type, LeapfrogHashTable::SlotType::EMPTY);

    /* Insert 19; should get
     *
     *  .  .  .  .  .  .  .  . 18 19
     *                          -  -
     */
    EXPECT(table.insert("19"));
    EXPECT_EQUAL(table.size(), 2);

    EXPECT_EQUAL(table.elems[0].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[1].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[2].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[3].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });

    /* Insert 28; should get
     *
     * 28  .  .  .  .  .  .  . 18 19
     *  -                       0  -
     */
    EXPECT(table.insert("28"));
    EXPECT_EQUAL(table.size(), 3);

    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[1].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[2].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[3].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });

    /* Insert 29; should get
     *
     * 28 29  .  .  .  .  .  . 18 19
     *  -  -                    0  1
     */
    EXPECT(table.insert("29"));
    EXPECT_EQUAL(table.size(), 4);

    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[1],      { "29", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[2].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[3].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, 1 });

    /* Insert 38; should get
     *
     * 28 29 38  .  .  .  .  . 18 19
     *  2  -  -                 0  1
     */
    EXPECT(table.insert("38"));
    EXPECT_EQUAL(table.size(), 5);

    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, 2 });
    EXPECT_EQUAL(table.elems[1],      { "29", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[2],      { "38", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[3].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, 1 });

    /* Insert 10; should get
     *
     * 28 29 38 10  .  .  .  . 18 19
     *  2  -  3  -              0  1
     */
    EXPECT(table.insert("10"));
    EXPECT_EQUAL(table.size(), 6);

    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, 2 });
    EXPECT_EQUAL(table.elems[1],      { "29", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[2],      { "38", LeapfrogHashTable::SlotType::FILLED, 3 });
    EXPECT_EQUAL(table.elems[3],      { "10", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, 1 });

    /* Insert 15; should get
     *
     * 28 29 38 10  . 15  .  . 18 19
     *  2  -  3  -     -        0  1
     */
    EXPECT(table.insert("15"));
    EXPECT_EQUAL(table.size(), 7);

    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, 2 });
    EXPECT_EQUAL(table.elems[1],      { "29", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[2],      { "38", LeapfrogHashTable::SlotType::FILLED, 3 });
    EXPECT_EQUAL(table.elems[3],      { "10", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5],      { "15", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, 1 });
}

PROVIDED_TEST("Basic contains tests.") {
    /* Build the table from the previous test. */

    /* Ten slots; each item hashes to its last digit. */
    LeapfrogHashTable table(Hash::identity(10));

    EXPECT(table.insert("18"));
    EXPECT(table.insert("19"));
    EXPECT(table.insert("28"));
    EXPECT(table.insert("29"));
    EXPECT(table.insert("38"));
    EXPECT(table.insert("10"));
    EXPECT(table.insert("15"));

    /* Double-check that this worked. */
    EXPECT_EQUAL(table.size(), 7);
    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, 2 });
    EXPECT_EQUAL(table.elems[1],      { "29", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[2],      { "38", LeapfrogHashTable::SlotType::FILLED, 3 });
    EXPECT_EQUAL(table.elems[3],      { "10", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5],      { "15", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, 1 });

    /* Our table looks like this.
     *
     * 28 29 38 10  . 15  .  . 18 19
     *  2  -  3  -     -        0  1
     *
     * Look up each item that's here.
     */
    EXPECT_EQUAL(table.contains("18"), true);
    EXPECT_EQUAL(table.contains("19"), true);
    EXPECT_EQUAL(table.contains("28"), true);
    EXPECT_EQUAL(table.contains("29"), true);
    EXPECT_EQUAL(table.contains("38"), true);
    EXPECT_EQUAL(table.contains("10"), true);
    EXPECT_EQUAL(table.contains("15"), true);

    /* Look up many items that aren't there to try out different cases. */

    /* Empty slot. */
    EXPECT_EQUAL(table.contains("14"), false);

    /* Chain of length zero. */
    EXPECT_EQUAL(table.contains("25"), false);

    /* Long chain. */
    EXPECT_EQUAL(table.contains("48"), false);
}

PROVIDED_TEST("Stress Test: Handles pure insertion of elements (should take at most 0.1s).") {
    Timing::Timer timer;

    timer.start();
    const int kNumTrials = 50; // Do this lots of times to smoke out any errors that might be lurking.
    for (int trial = 0; trial < kNumTrials; trial++) {
        LeapfrogHashTable table(Hash::random(100));

        const int kNumElems = 75;
        for (int i = 0; i < kNumElems; i++) {
            /* Confirm only the proper elements exist at this point. */
            for (int j = 0; j < kNumElems; j++) {
                EXPECT_EQUAL(table.contains(to_string(j)), bool(j < i));
            }

            /* Add the element. */
            EXPECT(table.insert(to_string(i))); // Should succeed
            EXPECT_EQUAL(table.size(), i + 1);
        }
    }

    timer.stop();
    EXPECT_LESS_THAN(timer.timeInSeconds(), 0.1);
}

PROVIDED_TEST("Stress Test: Inserts/searches work in expected time O(1) (should take at most 1 second).") {
    Timing::Timer timer;

    /* Huge number of slots. */
    const int kNumSlots = 1000000;

    timer.start();

    /* Create an enormous hash table with a random hash function. */
    LeapfrogHashTable table(Hash::random(kNumSlots));

    /* Search the table for lots of elements. This should quick, since
     * the table is empty.
     */
    for (int i = 0; i < kNumSlots; i++) {
        EXPECT(!table.contains(to_string(i)));
    }

    /* Insert a lot elements. */
    const int kLotsOfElems = 100000; // 10% load factor - quite small!
    for (int i = 0; i < kLotsOfElems; i++) {
        EXPECT(table.insert(to_string(i)));
    }
    for (int i = 0; i < kLotsOfElems; i++) {
        EXPECT(table.contains(to_string(i)));
    }

    /* Confirm other elements aren't there. These false lookups should still be fast
     * due to the low load factor.
     */
    for (int i = kLotsOfElems; i < 2 * kLotsOfElems; i++) {
        EXPECT(!table.contains(to_string(i)));
    }

    timer.stop();
    EXPECT_LESS_THAN(timer.timeInSeconds(), 1);
}

#include <fstream>
PROVIDED_TEST("Stress Test: Handles large workflows with little free space (should take at most a second)") {
    Vector<string> english;
    ifstream input("res/EnglishWords.txt");

    for (string word; getline(input, word); ) {
        english += word;
    }

    Timing::Timer timer;
    timer.start();

    /* Load factor 0.97. */
    LeapfrogHashTable table(Hash::consistentRandom(english.size() / 0.97));

    /* Insert everything. */
    for (const string& word: english) {
        EXPECT(table.insert(word));
    }
    EXPECT_EQUAL(table.size(), english.size());

    /* Make sure everything is there, and that the upper-case versions aren't. */
    for (const string& word: english) {
        EXPECT(table.contains(word));
        EXPECT(!table.contains(toUpperCase(word)));
    }

    timer.stop();
    EXPECT_LESS_THAN(timer.timeInSeconds(), 1);
}
