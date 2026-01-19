#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <iostream>
#include <vector>
#include "assignment.h"
#include "FNC.h"

using namespace std;

inline vector<vector<bool>> generateAllCombinations(int numVariables)
{
    vector<vector<bool>> combinations;
    int total = 1 << numVariables;
    for (int mask = 0; mask < total; mask++)
    {
        vector<bool> combination;
        for (int i = numVariables - 1; i >= 0; i--)
        {
            bool value = (mask & (1 << i)) != 0;
            combination.push_back(value);
        }
        combinations.push_back(combination);
    }
    return combinations;
}

inline void force(FNC formula)
{

    int numVariables = formula.getNumVariables();
    Assignment assignment(numVariables);

    vector<vector<bool>> combinations = generateAllCombinations(numVariables);
    TruthValue value = TruthValue::UNASSIGNED;
    int index = 0;

    while (value != TruthValue::TRUE && index < combinations.size())
    {
        assignment.assign(combinations[index]);
        value = evaluateFormula(formula, assignment);
        index++;
    }

    if(value == TruthValue::TRUE)
    {
        cout << "Solution: \n";
        assignment.print();
    }
    else
    {
        cout << "Unsatisfiable!" << endl;
    }

    return;
}

#endif