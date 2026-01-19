#include <iostream>
#include <vector>
#include "./headers/FNC.h"
#include "./headers/heuristics.h"

using namespace std;

int main()
{
    FNC formula(3);

    Clause c1;
    c1.addLiteral(Literal(1, false)); // x1
    c1.addLiteral(Literal(2, true));  // ¬x2

    Clause c2;
    c2.addLiteral(Literal(2, false)); // x2
    c2.addLiteral(Literal(3, false)); // x3

    formula.addClause(c1);
    formula.addClause(c2);

    formula.printFNC();

    force(formula);

    return 0;
}