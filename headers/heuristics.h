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

// tenta a solução por força bruta.
// avalia todas as combinações possíveis.
inline bool forceSolution(FNC &formula, Assignment &assignment)
{

    int numVariables = formula.getNumVariables();

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
        return true;
    }

    return false;
}

// avalia todas as cláusulas unitárias.
// caso encontre contradição, retorna false.
bool evaluateUnitary(FNC &formula, Assignment &assignment)
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
                changed = true;
            }
        }
    } while (changed);

    return true;
}

// retorna a primeira variável sem atribuição.
int getUnassignnedVar(const Assignment &assignment)
{
    int numVariables = assignment.getNumVariables();
    for (int i = 1; i <= numVariables; i++)
    {
        if (assignment.get(i) == TruthValue::UNASSIGNED)
        {
            return i;
        }
    }
    return -1;
}

// Método DPLL recursivo.
inline Assignment dpll(
    FNC &formula,
    Assignment &assignment
)
{

    // Caso base 1 - não tem solução
    if(!evaluateUnitary(formula, assignment))
    {
        return Assignment(0);
    }

    // Caso base 2 - tem solução
    if (evaluateFormula(formula, assignment) == TruthValue::TRUE)
    {
        return assignment;
    }

    // Seleciona uma variável sem atribuição
    int var = getUnassignnedVar(assignment);
    if (var == -1)
    {
        return Assignment(0);
    }

    //tenta var = true
    Assignment assignmentTrue = assignment;
    assignmentTrue.assign(var, true);
    Assignment result = dpll(formula, assignmentTrue);

    if (result.getNumVariables() != 0)
    {
        return result;
    }

    //tenta var = false
    Assignment assignmentFalse = assignment;
    assignmentFalse.assign(var, false);
    result = dpll(formula, assignmentFalse);

    return result;
}

inline bool dpllSolution(FNC &formula, Assignment &assignment)
{
    Assignment result = dpll(formula, assignment);
    if (result.getNumVariables() != 0)
    {
        assignment = result;
        return true;
    }

    return false;
}

#endif