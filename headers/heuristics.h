#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <iostream>
#include <vector>
#include <stack>
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

    if (value == TruthValue::TRUE)
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

struct Decision
{
    int var;
    bool value;
};

bool evaluateUnitary(FNC &formula, Assignment &assignment, vector<Decision> &decisions)
{

    bool changed;
    do
    {
        changed = false;
        for (const Clause &clause : formula.getClauses())
        {
            const Literal *literal = nullptr;
            int unassingedCount = 0;
            bool isTrue = false;
            unassingedCount = 0;

            for (const Literal &lit : clause.getLiterals())
            {
                TruthValue value = evaluateLiteral(lit, assignment);
                if (value == TruthValue::TRUE)
                {
                    isTrue = true;
                    break;
                }

                if (value == TruthValue::UNASSIGNED)
                {
                    literal = &lit;
                    unassingedCount++;
                }
            }

            cout << "Debug: Clause processed, isTrue=" << isTrue << ", unassingedCount=" << unassingedCount << endl;

            if (isTrue)
            {
                continue;
            }

            if (unassingedCount == 0)
            {
                return false;
            }

            if (unassingedCount == 1)
            {
                assignment.assign(literal->getVar(), !literal->isNegated());
                decisions.push_back(Decision{literal->getVar(), !literal->isNegated()});
                changed = true;
            }
            assignment.print();
        }
    } while (changed);

    return true;
}

inline bool dpll(
    FNC formula,
    Assignment assignment,
    vector<Decision> decisions = vector<Decision>())
{

    if(!evaluateUnitary(formula, assignment, decisions))
    {
        return false;
    }

    if (evaluateFormula(formula, assignment) == TruthValue::TRUE)
    {
        return true;
    }

    
}

#endif