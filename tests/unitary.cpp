#include <iostream>
#include <vector>
#include <stack>
#include "../headers/FNC.h"
#include "../headers/assignment.h"
#include "../headers/heuristics.h"
#include "../headers/DimacsParser.h"

using namespace std;

// teste da função que avalia as cláusulas unitárias

int main()
{

    FNC formula = DimacsParser::parse("./recursive.txt");
    Assignment assignment(formula.getNumVariables());

    cout << "Valores iniciais:\n";
    cout << "Formula: \n";
    formula.printFNC();
    cout << "Valores: \n";
    assignment.print();

    bool unitary = evaluateUnitary(formula, assignment);

    cout << "Valores finais:\n";
    cout << "Formula: \n";
    formula.printFNC();
    cout << "Valores: \n";
    assignment.print();
    cout << "SATISFIABLE: " << (unitary ? "TRUE" : "FALSE") << endl;

    return 0;
}