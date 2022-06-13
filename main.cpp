#include <iostream>
#include <vector>

using namespace std;

#include "Equation.h"

int main() {
    int total = 0;
    vector<int> ops = {'+', '-'};
    // find all puzzles with solution
    for (int first = 0; first < 10; ++first) {
        for (int second = 0; second < 10; ++second) {
            for (int third = 0; third < 10; ++third) {
                for (const auto &op: ops) {
                    auto e = Equation(first, op, second, third);
                    auto solutions = e.solve();
                    if (!solutions.empty()) {
                        cout << "found one\n";
                        total++;
                        cout << "puzzle:\n";
                        e.print();
                        cout << "solutions: \n";
                        for (const auto &s: solutions) {
                            s.print();
                        }
                        cout << "-------------------------------------------\n";
                    }
                }
            }
        }
    }

    cout << "in total " << total << endl;
}