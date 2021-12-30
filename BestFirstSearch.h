#ifndef BEST_FIRST_SEARCH_H
#define BEST_FIRST_SEARCH_H

#include "Searchable.h"

#include <memory>
using std::unique_ptr;

#include <utility>
using std::pair;
using std::make_pair;

#include <vector>
using std::vector;

#include <algorithm>
using std::sort;

/*
    Compares the heuristic values of two pairs
    Returns true if first pair has a lower heuristic value
    compared to the second pair
*/
bool compare(const pair<int, unique_ptr<Searchable> > & pair1,
     const pair<int, unique_ptr<Searchable> > & pair2) {
        return pair1.first < pair2.first;
}

class BestFirstSearch {

protected:

    /// Make sure you increment this every time you 'expand' a node, by getting it successors and putting them on the queue        
    int nodes = 0;
    
    vector<pair<int, unique_ptr<Searchable> > > pq;


public:

    BestFirstSearch(std::unique_ptr<Searchable> && startFrom) {
        int heuristicValue = startFrom.get()->heuristicValue();
        pq.push_back(std::make_pair(heuristicValue, std::move(startFrom)));
        sortVector();
    }
    
    int getNodesExpanded() const {
        return nodes;
    }

    Searchable * solve() {

        while(!pq.empty()) {

            if (pq.begin() != pq.end()) {

                if (pq.front().second.get()->isSolution()) {
                    return pq.begin().operator*().second.get();
                }

                ++nodes;

                unique_ptr<Searchable> current(std::move(pq.front().second));

                pq.erase(pq.begin());

                vector<unique_ptr<Searchable>> successors = current->successors();

                for (auto & successor : successors) {
                    int value = successor->heuristicValue();
                    pq.push_back(std::move(make_pair(value, std::move(successor))));
                }
                sortVector();
            }
        }
        return nullptr;
    }

private:

    void sortVector() {
        sort(pq.begin(), pq.end(), compare);
    }

};



// Do not edit below this line

#endif
