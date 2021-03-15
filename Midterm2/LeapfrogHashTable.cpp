/*  Description: This is my implementation of hashtable using leapfrog strategy
**/
#include "LeapfrogHashTable.h"
#include "GUI/SimpleTest.h"
using namespace std;

LeapfrogHashTable::LeapfrogHashTable(HashFunction<string> hashFn) {
    hash = hashFn;
    capacity = hash.numSlots();
    curSize = 0;
    //populate the array with default values
    elems = new Slot[capacity];
    for (int i = 0; i < capacity; i++) {
        elems[i] = {"", SlotType::EMPTY, NOT_LINKED};
    }
}

LeapfrogHashTable::~LeapfrogHashTable() {
    delete [] elems;
}

bool LeapfrogHashTable::contains(const string& key) const {
    int pos = hash(key);
    //If this starting position is empty, the key cannot be in this table.
    if (elems[pos].type == SlotType::EMPTY) return false;
    //keep tracing through the link untile we at at the end of this chain
    while (elems[pos].link != NOT_LINKED) {
        if (elems[pos].value == key) return true;
        pos = elems[pos].link;
    }
    //if the end cell does not conatin key, key is not in this table!
    return elems[pos].value == key;


}

bool LeapfrogHashTable::insert(const string& key) {
    //we cannot insert if the table is full or the key is already present
    if (size() == capacity) return false;
    if (contains(key)) return false;

    int pos = hash(key);
    //If the starting position is empty, we are pretty all set!
    if (elems[pos].type == SlotType::EMPTY) {
        elems[pos].value = key;
        elems[pos].type = SlotType::FILLED;
    //else we need to walk to the end of this chain
    } else {
        while (elems[pos].link != NOT_LINKED) {
            pos = elems[pos].link;
        }
        //and then search for the next empty postion to place key in
        int tail = pos;
        while (elems[tail].type == SlotType::FILLED) {
            tail = (tail + 1) % capacity;
        }
        elems[pos].link = tail;
        elems[tail].type = SlotType::FILLED;
        elems[tail].value = key;
    }

    curSize++;
    return true;

}

int LeapfrogHashTable::size() const {
    return curSize;
}

bool LeapfrogHashTable::isEmpty() const {
    return size() == 0;
}

void LeapfrogHashTable::printDebugInfo() const {
    for (int i = 0; i < capacity; i++) {
        cout << elems[i] << endl;
    }
}

/* Space for test cases. You are not required to write any test cases, but you
 * are encouraged to do so because our test coverage is not as extensive as what
 * you saw in Assignment 7.
 */
