#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <iostream>
#include <vector>
#include "FNC.h"

enum class TruthValue
{
    TRUE,
    FALSE,
    UNASSIGNED
};

inline std::string to_string(TruthValue value)
{
    switch (value)
    {
    case TruthValue::TRUE:
        return "TRUE";
    case TruthValue::FALSE:
        return "FALSE";
    case TruthValue::UNASSIGNED:
        return "UNASSIGNED";
    }
    return "UNASSIGNED";
}

class Assignment
{
private:
    std::vector<TruthValue> values;

public:
    Assignment(int numVars)
        : values(numVars + 1, TruthValue::UNASSIGNED) {}

    void assign(int var, bool value)
    {
        values[var] = value ? TruthValue::TRUE : TruthValue::FALSE;
    }

    TruthValue get(int var) const
    {
        return values[var];
    }

    void assign(const std::vector<bool> &values)
    {
        for (int i = 1; i <= values.size(); i++)
        {
            assign(i, values[i-1]);
        }
    }

    void print()
    {
        for (int i = 1; i < values.size(); i++)
        {
            std::cout << "x" << i << ": " << to_string(values[i]) << std::endl;
        }
    }

    int getNumVariables() const
    {
        return values.size() - 1;
    }
};

inline TruthValue evaluateLiteral(const Literal &lit, const Assignment &a)
{
    TruthValue v = a.get(lit.getVar());

    if (v == TruthValue::UNASSIGNED)
        return TruthValue::UNASSIGNED;

    bool value = (v == TruthValue::TRUE);
    return (lit.isNegated() ? !value : value)
               ? TruthValue::TRUE
               : TruthValue::FALSE;
}

inline TruthValue evaluateClause(const Clause &clause, const Assignment &a)
{
    bool hasUnassigned = false;

    for (const auto &lit : clause.getLiterals())
    {
        TruthValue v = evaluateLiteral(lit, a);
        if (v == TruthValue::TRUE)
            return TruthValue::TRUE;
        if (v == TruthValue::UNASSIGNED)
            hasUnassigned = true;
    }

    return hasUnassigned ? TruthValue::UNASSIGNED : TruthValue::FALSE;
}

inline TruthValue evaluateFormula(const FNC &f, const Assignment &a)
{
    bool hasUnassigned = false;

    for (const auto &clause : f.getClauses())
    {
        TruthValue v = evaluateClause(clause, a);
        if (v == TruthValue::FALSE)
            return TruthValue::FALSE;
        if (v == TruthValue::UNASSIGNED)
            hasUnassigned = true;
    }

    return hasUnassigned ? TruthValue::UNASSIGNED : TruthValue::TRUE;
}

#endif