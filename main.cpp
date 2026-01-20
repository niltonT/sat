#include <iostream>
#include <vector>
#include "./headers/FNC.h"
#include "./headers/heuristics.h"
#include "./headers/DimacsParser.h"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 2){
        cout << "Args invalidos" << endl;
        return -1;
    }
    
    try
    {
        FNC formula = DimacsParser::parse(argv[1]);
        formula.printFNC();
        cout << "Solução por força bruta:" << endl;
        forceSolution(formula);
        cout << "Solução por DPLL:" << endl;
        dpllSolution(formula);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}