#include <cctype>
#include <iostream>
#include <map>
#include <set>

#include "sudoku.cpp"

using namespace std;

int main(void) {
    int N;
    cin >> N;
    set<int> all_values;
    for (int i = 0; i < N; i++) {
        all_values.insert(i + 1);
    }
    map<pair<int, int>, set<int>> cells;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            char c;
            cin >> c;
            if (isdigit(c)) {
                cells[make_pair(i, j)] = set<int>{c - '0'};
            } else {
                cells[make_pair(i, j)] = all_values;
            }
        }
    }

    sudoku::SolutionIdea<set<pair<int, int>>, set<int>> idea {cells};
    // TODO: create constraints
    // and run the actual solving algorithm when it's implemented
}
