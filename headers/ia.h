#ifndef IA_H
#define IA_H


// gerado pela ia

#include "assignment.h"
#include <vector>

class BruteForceSAT
{
public:
    static bool solve(const FNC &f, Assignment &solution)
    {
        int n = f.getNumVariables();
        int total = 1 << n;

        for (int mask = 0; mask < total; mask++)
        {
            Assignment a(n);

            for (int i = 0; i < n; i++)
            {
                bool value = (mask & (1 << i)) != 0;
                a.assign(i + 1, value);
            }

            TruthValue result = evaluateFormula(f, a);
            if (result == TruthValue::TRUE)
            {
                solution = a;
                return true;
            }
        }

        return false;
    }
};

bool unitPropagation(const FNC &f, Assignment &a)
{
    bool changed;

    do
    {
        changed = false;

        for (const Clause &c : f.getClauses())
        {
            if (!c.isUnit())
                continue;

            const Literal &lit = c.getLiterals()[0];
            TruthValue v = evaluateLiteral(lit, a);

            if (v == TruthValue::FALSE)
                return false;

            if (v == TruthValue::UNASSIGNED)
            {
                a.assign(lit.getVar(), !lit.isNegated());
                changed = true;
            }
        }

    } while (changed);

    return true;
}

int chooseUnassignedVariable(const Assignment &a)
{
    for (int i = 1; i <= a.getNumVariables(); i++)
    {
        if (a.get(i) == TruthValue::UNASSIGNED)
            return i;
    }
    return -1;
}


bool dpll(const FNC &f, Assignment &a)
{
    TruthValue status = evaluateFormula(f, a);

    if (status == TruthValue::TRUE)
        return true;

    if (status == TruthValue::FALSE)
        return false;

    if (!unitPropagation(f, a))
        return false;

    int var = chooseUnassignedVariable(a);
    if (var == -1)
        return false;

    // Tentativa TRUE
    {
        Assignment a_true = a;
        a_true.assign(var, true);
        if (dpll(f, a_true))
        {
            a = a_true;
            return true;
        }
    }

    // Tentativa FALSE
    {
        Assignment a_false = a;
        a_false.assign(var, false);
        if (dpll(f, a_false))
        {
            a = a_false;
            return true;
        }
    }

    return false;
}

class DPLLSAT
{
public:
    static bool solve(const FNC &f, Assignment &solution)
    {
        Assignment a(f.getNumVariables());
        if (dpll(f, a))
        {
            solution = a;
            return true;
        }
        return false;
    }
};


#endif