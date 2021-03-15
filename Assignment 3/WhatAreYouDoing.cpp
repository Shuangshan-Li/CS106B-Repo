/* This function takes as input a string and return all the possible scenarios
 * of whether making a word in that string upper case or lower case.
 * Use wrapper function to pass more information for recursion.
 */
#include "WhatAreYouDoing.h"
#include "GUI/SimpleTest.h"
#include "strlib.h"
#include <cctype>
using namespace std;


Set<string> allEmphasesOfRec(const Vector<string>& words, const string& processed) {
   if (words.isEmpty()) {
       return {processed};
   }
   Set<string> result;
   // Is it is not a word, do not branch on it, simply add it to the processed
   if (!isalpha(words[0][0])) {
       result += allEmphasesOfRec(words.subList(1), processed + words[0]);

   } else {
       result += allEmphasesOfRec(words.subList(1), processed + toLowerCase(words[0]));
       result += allEmphasesOfRec(words.subList(1), processed + toUpperCase(words[0]));

   }
   return result;
}

Set<string> allEmphasesOf(const string& sentence) {
    if (sentence.size() == 0) {
        error("No input!");
    }
    Vector<string> words = tokenize(sentence);
    return allEmphasesOfRec(words, "");
}


/* * * * * * Test Cases * * * * * */

STUDENT_TEST("No input") {
    EXPECT_ERROR(allEmphasesOf(""));
}


/* * * * * * Test cases from the starter files below this point. * * * * * */

PROVIDED_TEST("Provided Test: Enumerates all options in a simple case.") {
    Set<string> expected = {
        "hello",
        "HELLO",
    };

    EXPECT_EQUAL(allEmphasesOf("Hello"), expected);
}

PROVIDED_TEST("Provided Test: Each option has the right length.") {
    string sentence = "Hello, world!";
    for (string option: allEmphasesOf(sentence)) {
        EXPECT_EQUAL(option.length(), sentence.length());
    }
}

PROVIDED_TEST("Provided Test: Enumerates all options in a more typical case.") {
    Set<string> expected = {
        "you are?",
        "you ARE?",
        "YOU are?",
        "YOU ARE?"
    };

    EXPECT_EQUAL(allEmphasesOf("you are?"), expected);
}

PROVIDED_TEST("Provided Test: Stress Test: Recursion only branches on words.") {
    /* We're producing a string consisting of fifty copies of the * character. This tokenizes
     * into fifty separate stars. A star is the same whether or not you capitalize it - there
     * is no such thing as an "upper-case" or "lower-case" star. Therefore, your code should
     * not try to form two versions of the sentence, one with the star capitalized and one
     * without, because the two versions will end up being the same and the work to compute
     * both options will dramatically increase the runtime.
     *
     * For reference, if you do try branching the recursion and checking what happens both if
     * you capitalize the star and if you don't, you'll try exploring 2^50 different possible
     * capitalizations. That's 1,125,899,906,842,624 options, and even doing a billion of
     * these a second is going to take over two years to generate them all! And of course,
     * that's all wasted work, since there's only one way to capitalize this sentence, and
     * that's just to leave it as-is.
     *
     * If your code is hanging when this test is running, it likely means that your code is
     * trying to enumerate all of these options. See if you can edit your code so that, if
     * you're given a non-word token, you just leave it as-is and don't change anything.
     */
    string punctuation(50, '*'); // 50 copies of *

    /* The only emphasis is itself. */
    Set<string> expected = {
        punctuation
    };

    EXPECT_EQUAL(allEmphasesOf(punctuation), expected);
}
