#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"

// TODO: Your Sudoku class goes here:

#include <set>
using std::set;

#include <vector>
using std::vector;

#include <cmath>
using std::sqrt;

#include "SudokuSquare.h"

#include <utility>
using std::pair;
using std::make_pair;

class Sudoku : public Searchable {

private:

    vector<vector<SudokuSquareSet>> board;
    vector<vector<bool>> singlesVisited; // shows if a sqaure with 1 value has been visited

public:

    /* 
        Initialises a board of a given size
        Inserts a set of numbers into the empty board
    */
    Sudoku(const int & boardSize)
        :board(boardSize) {
        SudokuSquareSet initialisationSet;
        for (int i = 1; i <= boardSize; ++i) {
            initialisationSet.insert(i);
        }
        for (int y = 0; y < boardSize; ++y) {
            vector<SudokuSquareSet> insideVector;
            for (int x = 0; x < boardSize; x++) {
                insideVector.push_back(initialisationSet);
            }
            board[y] = insideVector; 
        }

        // initialises the singlesVisited vector all to false
        vector<bool> boolVector;
        for (int i = 0; i < boardSize; ++i) { 
            boolVector.push_back(false);
        }
        for (int i = 0; i < boardSize; ++i) {
            singlesVisited.push_back(boolVector);
        }
    }


    Sudoku(const Sudoku & boardToBeCopied)
        : board(boardToBeCopied.board), singlesVisited(boardToBeCopied.singlesVisited) {
    }


    /*
        Returns value in set if set only has one value
        Else, returns -1 
    */
    int getSquare(const int row, const int col) {
        if(board[row][col].size() == 1) { 
            return board[row][col].begin().operator*();
        } else {
            return -1;
        }
    }

    /*
        Sets the value of a given sqaure
    */
    bool setSquare(const int row, const int col, const int value) {
        board[row][col].clear();
        board[row][col].insert(value);

        bool newSingleFound = true;
        while(newSingleFound) {
            newSingleFound = false;
            for (int y = 0; y < board.size(); ++y) {
                for (int x = 0; x < board[y].size(); ++x) {
                    if (board[y][x].size() < 1) {
                        return false;
                    }
                    if (board[y][x].size() == 1) {
                        if (!singlesVisited[y][x]) {
                           singlesVisited[y][x] = true;
                           newSingleFound = true;
                           int currValue = board[y][x].begin().operator*();
                           // removals

                           // delete from squares on the same row
                           for (int i = 0; i < board.size(); ++i) {
                               if (x != i) {
                                    SudokuSquareSet::Iterator itr = board[y][i].find(currValue);
                                    if (itr != board[y][i].end()) {
                                        board[y][i].erase(itr);
                                        if (board[y][i].size() < 1) {
                                            return false;
                                        }
                                    }
                               }
                           }

                           // delete from squares in the same column
                            for (int i = 0; i < board.size(); ++i) {
                                if (i != y) {
                                    SudokuSquareSet::Iterator itr = board[i][x].find(currValue);
                                    if (itr != board[i][x].end()) {
                                        board[i][x].erase(itr);
                                        if (board[i][x].size() < 1) {
                                            return false;
                                        }
                                    }
                                }
                            }
    
                            // delete from subgrid
                            int startY = y - y% (int)(sqrt(board.size()));
                            int startX = x - x% (int)(sqrt(board.size()));
                            int endY = startY + (int) (sqrt(board.size()));
                            int endX = startX + (int) (sqrt(board.size()));
                            for (int subY = startY; subY < endY; ++subY) {
                                for (int subX = startX; subX < endX; ++subX) {
                                    if (subY != y || subX != x) {
                                        SudokuSquareSet::Iterator itr = board[subY][subX].find(currValue);
                                        if (itr != board[subY][subX].end()) {
                                            board[subY][subX].erase(itr);
                                            if (board[subY][subX].size() < 1) {
                                                return false;
                                            }
                                        }
                                    }  
                                }
                            }
                        }
                    }
                     if (board[y][x].size() == 2) {
                        // check for pairs on the same row
                        int foundIndex = -1;
                        for (int i = 0; i < board.size(); ++i) {
                            if (board[y][i].size() == 2 && i != x) {
                                if (board[y][i] == board[y][x]) {
                                    foundIndex = i;
                                    break;
                                }
                            }
                        }
                        
                        if (foundIndex >= 0) {
                            int value1 = board[y][x].begin().operator*();
                            auto itr = board[y][x].begin();
                            itr.operator++();
                            int value2 = itr.operator*();
                            for (int index = 0; index < board.size(); ++index) {
                                if (x != index && index != foundIndex) {
                                    SudokuSquareSet::Iterator itr1 = board[y][index].find(value1);
                                    if (itr1 != board[y][index].end()) {
                                        board[y][index].erase(itr1);
                                        if (board[y][index].size() < 1) {
                                            return false;
                                        }
                                    }
                                    SudokuSquareSet::Iterator itr2 = board[y][index].find(value2);
                                    if (itr2 != board[y][index].end()) {
                                        board[y][index].erase(itr2);
                                        if (board[y][index].size() < 1) {
                                            return false;
                                        }
                                    }
                               }
                            }
                        }

                        // check for pairs on the same column
                        foundIndex = -1;
                        for (int i = 0; i < board.size(); ++i) {
                            if (board[i][x].size() == 2 && i != y) {
                                if (board[i][x] == board[y][x]) {
                                    foundIndex = i;
                                    break;
                                }
                            }
                        }

                        if (foundIndex >= 0) {
                            int value1 = board[y][x].begin().operator*();
                            auto itr = board[y][x].begin();
                            itr.operator++();
                            int value2 = itr.operator*();
                            for (int index = 0; index < board.size(); ++index) {
                                if (y != index && index != foundIndex) {
                                    SudokuSquareSet::Iterator itr1 = board[index][x].find(value1);
                                    if (itr1 != board[index][x].end()) {
                                        board[index][x].erase(itr1);
                                        if (board[index][x].size() < 1) {
                                            return false;
                                        }
                                    }
                                    SudokuSquareSet::Iterator itr2 = board[index][x].find(value2);
                                    if (itr2 != board[index][x].end()) {
                                        board[index][x].erase(itr2);
                                        if (board[index][x].size() < 1) {
                                            return false;
                                        }
                                    }
                               }
                            }
                        }
                        
                        // check if the grid has a pair
                        int foundY = -1;
                        int foundX = -1;
                        int startY = y - y% (int)(sqrt(board.size()));
                        int startX = x - x% (int)(sqrt(board.size()));
                        int endY = startY + (int) (sqrt(board.size()));
                        int endX = startX + (int) (sqrt(board.size()));
                        for (int subY = startY; subY < endY; ++subY) {
                            for (int subX = startX; subX < endX; ++subX) {
                                if (board[subY][subX].size() == 2 && subY != y || subX != x) {
                                    if (board[y][x] == board[subY][subX]) {
                                        foundY = subY;
                                        foundX = subX;
                                        break;
                                    }
                                }
                            }
                            if (foundY > -1 && foundX > -1) {
                                break;
                            }
                        }

                        if (foundY >= 0 && foundX >= 0) {
                            int value1 = board[y][x].begin().operator*();
                            auto itr = board[y][x].begin();
                            itr.operator++();
                            int value2 = itr.operator*();
                            for (int subY = startY; subY < endY; ++subY) {
                                for (int subX = startX; subX < endX; ++subX) {
                                    
                                    if ((subY != y || subX != x) &&(subY != foundY || subX != foundX)) {
                                        SudokuSquareSet::Iterator itr1 = board[subY][subX].find(value1);
                                        if (itr1 != board[subY][subX].end()) {
                                            board[subY][subX].erase(itr1);
                                            if (board[subY][subX].size() < 1) {
                                                return false;
                                            }
                                        }

                                        SudokuSquareSet::Iterator itr2 = board[subY][subX].find(value2);
                                        if (itr2 != board[subY][subX].end()) {
                                            board[subY][subX].erase(itr2);
                                            if (board[subY][subX].size() < 1) {
                                                return false;
                                            }
                                        }
                                    }
                                }
                            }
                        }
 
                    }
                }
            }
        }
        return true;
    }

    bool isSolution() const override {
        for (int y = 0; y < board.size(); ++y) {
            for(int x = 0; x < board.size(); ++x) {
                if (board[y][x].size() != 1) {
                    return false;
                }
            }
        }
        return true;
    }

    void write(ostream & o) const override {
        for (int y = 0; y < board.size(); ++y) {
            for (int x = 0; x < board.size(); ++x) {
                if (board[y][x].size() == 1) {
                    o << board[y][x].begin().operator*();
                } else {
                    o << " ";
                }
            }
            o << '\n';
        }
    }

    vector<unique_ptr<Searchable> > successors() const override {
        vector<unique_ptr<Searchable>> successorsToReturn;
        int foundRow = -1;
        int foundCol = -1;
        for (int y = 0; y < board.size(); ++y) {
            for (int x = 0; x < board.size(); ++x) {
                if (board[y][x].size() > 1) {
                    foundRow = y;
                    foundCol = x;
                    break;
                }
            }
        }

        if (foundCol >= 0 && foundRow >= 0) {
            SudokuSquareSet possibleValues = board[foundRow][foundCol];
            for (int value : possibleValues) {
                Sudoku * boardPtr = new Sudoku(*this);
                if (boardPtr->setSquare(foundRow, foundCol, value)) {
                    successorsToReturn.push_back(unique_ptr<Sudoku>(boardPtr));
                } else {
                    delete boardPtr;
                }
            }
        }

        if (successorsToReturn.size() == 1) {
            if (!successorsToReturn.begin().operator*().get()->isSolution()) {
                return successorsToReturn.begin().operator*().get()->successors();
            }
        }
        return successorsToReturn;
    }

    int heuristicValue() const override {
        int numOSqaures = 0;
        for (int y = 0; y < board.size(); ++y) {
            for (int x = 0; x < board.size(); ++x) {
                if (board[y][x].size() > 1) {
                    numOSqaures++;
                }
            }
        }
        return numOSqaures;
    }
};



#endif
