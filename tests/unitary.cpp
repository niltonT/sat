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
    vector<Decision> decisions = vector<Decision>();

    cout << "Valores iniciais:\n";
    cout << "Formula: \n";
    formula.printFNC();
    cout << "Valores: \n";
    assignment.print();
    cout << "Decisions: \n";
    for (Decision &d : decisions)
    {
        cout << "Decision: Variable " << d.var << ", Value " << d.value << endl;
    }

    bool unitary = evaluateUnitary(formula, assignment, decisions);

    cout << "Valores finais:\n";
    cout << "Formula: \n";
    formula.printFNC();
    cout << "Valores: \n";
    assignment.print();
    cout << "SATISFIABLE: " << (unitary ? "TRUE" : "FALSE") << endl;
    cout << "Decisions: \n";
    for (Decision &d : decisions)
    {
        cout << "Decision: Variable " << d.var << ", Value " << d.value << endl;
    }

    return 0;
}