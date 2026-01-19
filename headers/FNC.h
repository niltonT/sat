#ifndef FNC_H
#define FNC_H

class Literal
{
private:
    int var;      // identificador da variável (ex: 1, 2, 3...)
    bool negated; // true se for ¬var

public:
    Literal(int var, bool negated = false)
        : var(var), negated(negated) {}

    int getVar() const
    {
        return var;
    }

    bool isNegated() const
    {
        return negated;
    }

    Literal negation() const
    {
        return Literal(var, !negated);
    }
};

#include <vector>

class Clause
{
private:
    std::vector<Literal> literals;

public:
    Clause() = default;

    Clause(const std::vector<Literal> &lits)
        : literals(lits) {}

    void addLiteral(const Literal &lit)
    {
        literals.push_back(lit);
    }

    const std::vector<Literal> &getLiterals() const
    {
        return literals;
    }

    bool isUnit() const
    {
        return literals.size() == 1;
    }

    bool isEmpty() const
    {
        return literals.empty();
    }
};

class FNC
{

private:
    std::vector<Clause> clauses;
    int numVariables;

public:
    FNC(int numVars = 0)
        : numVariables(numVars) {}

    void addClause(const Clause &clause)
    {
        clauses.push_back(clause);
    }

    const std::vector<Clause> &getClauses() const
    {
        return clauses;
    }

    int getNumVariables() const
    {
        return numVariables;
    }

    void setNumVariables(int n)
    {
        numVariables = n;
    }

    void printFNC(){
        for (int i = 0; i < clauses.size(); i++) {
            std::cout << "(";
            for (int j = 0; j < clauses[i].getLiterals().size(); j++) {
                Literal lit = clauses[i].getLiterals()[j];
                if (lit.isNegated()) {
                    std::cout << "¬";
                }
                if(j < clauses[i].getLiterals().size() - 1){
                    std::cout << "x" << lit.getVar() << " v ";
                }else{
                    std::cout << "x" << lit.getVar();
                }
            }
            if (i < clauses.size() - 1){
                std::cout << ") ^ ";
            }else{
                std::cout << ")";
            }
        }
        std::cout << std::endl;
    }
};

#endif