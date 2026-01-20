#include <iostream>
#include <vector>
#include <chrono>
#include "./headers/FNC.h"
#include "./headers/heuristics.h"
#include "./headers/DimacsParser.h"
#include "./headers/ia.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Args invalidos" << endl;
        return -1;
    }

    try
    {
        FNC formula = DimacsParser::parse(argv[1]);
        formula.printFNC();

        cout << "============= Solução humana =============" << endl;
        cout << "Solução por força bruta:" << endl;
        auto start = chrono::high_resolution_clock::now();
        forceSolution(formula);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Tempo de execução: " << duration.count() << " microssegundos" << endl;
        cout << "Solução por DPLL:" << endl;
        start = chrono::high_resolution_clock::now();
        dpllSolution(formula);
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Tempo de execução: " << duration.count() << " microssegundos" << endl;

        cout << "=============== Solução IA ===============" << endl;
        cout << "Solução por força bruta:" << endl;
        auto start = chrono::high_resolution_clock::now();
        Assignment sol(formula.getNumVariables());
        if (BruteForceSAT::solve(formula, sol))
        {
            cout << "Solução encontrada: ";
            sol.print();
        }
        else
        {
            cout << "Nenhuma solução encontrada." << endl;
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Tempo de execução: " << duration.count() << " microssegundos" << endl;
        cout << "Solução por DPLL:" << endl;
        start = chrono::high_resolution_clock::now();
        Assignment sol(formula.getNumVariables());
        if (BruteForceSAT::solve(formula, sol))
        {
            cout << "Solução encontrada: ";
            sol.print();
        }
        else
        {
            cout << "Nenhuma solução encontrada." << endl;
        }
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Tempo de execução: " << duration.count() << " microssegundos" << endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}