#ifndef SUDOKUSQUARE_H
#define SUDOKUSQUARE_H

// Do not add any #include statements to this file

class SudokuSquareSet {

private:

    unsigned int setOfValues;
    int numOfValues;

public:

    class Iterator {

    private:

        int current; // the index of the value
        unsigned int setOfValues;

    public:
    
        /*
            Constructs a new iterator object
        */
        Iterator(const int currentIn, const unsigned int & setIn)
            : current(currentIn), setOfValues(setIn) {}

        /*
            Increments the iterator to point to the next element in the set
        */
        void operator++() {
            int count = 1;
            int foundCount = 0;
            unsigned int n = setOfValues;
            while(n > 0) {
                if (n % 2 == 1) {
                    if (count > current) {
                        current = count;
                        return;
                    }
                }
                count++;
                n = n / 2;
            }
            current = 0;
        }

        /*
            Returns the value associated with the iterator
        */
        int operator*() {
            return current; 
        }

        /*
            Compares if two iterators are equal
        */
        bool operator==(const Iterator & other) const {
            return (current == other.current);
        }

        /*
            Compares if two iterators are not equal
        */
        bool operator!=(const Iterator & other) const {
            return (current != other.current);
        }


    };

    SudokuSquareSet()
        : setOfValues(0), numOfValues(0) {}

    /*
        Returns the size of the set
    */
    const int size() const {
        return numOfValues;
    }

    /*
        Determines if the set if empty
    */
    bool empty() const {
        return (numOfValues == 0);
    }

    /*
        Clears the set
    */
    void clear() {
        setOfValues = 0;
        numOfValues = 0;
    }

    /*
        Determines if two SudokuSqaureSet objects are equal
    */
    bool operator==(const SudokuSquareSet & other) const {
        return (numOfValues == other.numOfValues && setOfValues == other.setOfValues);
    }

    /*
        Determines if two SudokuSqaureSet objects are not equal
    */
    bool operator!=(const SudokuSquareSet & other) const {
        return (numOfValues != other.numOfValues || setOfValues != other.setOfValues);
    }

    /*
        Returns an iterator to the first element in the set
    */
    Iterator begin() const {
        if (setOfValues == 0) {
           return end();
        } else {
            unsigned int n = setOfValues;
            int count = 1;
            while(n > 0) {
                if (n % 2 == 1) {
                    return Iterator(count, setOfValues);
                } else {
                    count++;
                    n = n / 2;
                }
            }
       }
       return Iterator(0, setOfValues);
    }

    /*
        Returns an iterator pointing off the end of the set
    */
    Iterator end() const {
        return Iterator(0, setOfValues);
    }

    /*
        Inserts the value into the set
    */
    SudokuSquareSet::Iterator insert(const int & value) {
        SudokuSquareSet::Iterator itr = find(value);
        if (itr == end()) {
            int sum = 1;
            for(int i = 1; i <= value - 1; i++) {
                sum = sum * 2;
            }
            setOfValues += sum;
            numOfValues++;
            return Iterator(value, setOfValues);
        } else {
            return itr;
        }
    }

    /*
        Returns an iterator to the element with the given value
    */
    SudokuSquareSet::Iterator find(const int & value) {
        bool foundNext = false;
        unsigned int n = setOfValues;
        int count = 1;
        while(!foundNext && n > 0) {
            if (n % 2 == 1) {
                if (count == value) {
                    return Iterator(value, setOfValues);
                }
            }
            n = n / 2;
            count++;
        }
        return end();
    }

    /*
        Removes the value from the set
    */
    void erase(const int & value) {
        auto itr = find(value);
        if (itr != end()) {
            int sum = 1;
            for (int i = 1; i < value; i++) {
                sum = sum * 2;
            }
            setOfValues = setOfValues - sum;
            numOfValues--;
        }
    }

    /*
        Removes the element associated with the given iterator
    */
    void erase(SudokuSquareSet::Iterator itr) {
        erase(itr.operator*());
    }

};


// Do not write any code below this line
static_assert(sizeof(SudokuSquareSet) == sizeof(unsigned int) + sizeof(int), "The SudokuSquareSet class needs to have exactly two 'int' member variables, and no others");


#endif
