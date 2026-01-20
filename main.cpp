#include <iostream>
#include <vector>
#include <chrono>
#include "./headers/FNC.h"
#include "./headers/heuristics.h"
#include "./headers/DimacsParser.h"
#include "./headers/ia.h"
#include <iomanip>

using namespace std;

struct SolutionResult
{
    bool sat;
    Assignment assignment;

    SolutionResult(int n) : sat(false), assignment(n) {}
};

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Uso: " << argv[0] << " <arquivo.cnf> <num_execucoes>\n";
        return -1;
    }

    int N_RUNS = stoi(argv[2]);
    if (N_RUNS <= 0)
    {
        cout << "Numero de execucoes deve ser positivo.\n";
        return -1;
    }

    try
    {
        FNC formula = DimacsParser::parse(argv[1]);

        cout << "\n========================== Formula ===========================\n\n  ";

        formula.printFNC();

        long long humano_force = 0;
        long long humano_dpll = 0;
        long long ia_force = 0;
        long long ia_dpll = 0;

        SolutionResult sol_h_force(formula.getNumVariables());
        SolutionResult sol_h_dpll(formula.getNumVariables());
        SolutionResult sol_ia_force(formula.getNumVariables());
        SolutionResult sol_ia_dpll(formula.getNumVariables());

        for (int i = 0; i < N_RUNS; i++)
        {
            // ================= HUMANO =================
            auto start = chrono::high_resolution_clock::now();
            Assignment h1 = Assignment(formula.getNumVariables());
            bool sat_hf = forceSolution(formula, h1);
            auto end = chrono::high_resolution_clock::now();
            humano_force += chrono::duration_cast<chrono::microseconds>(end - start).count();

            start = chrono::high_resolution_clock::now();
            Assignment h2 = Assignment(formula.getNumVariables());
            bool sat_hd = dpllSolution(formula, h2);
            end = chrono::high_resolution_clock::now();
            humano_dpll += chrono::duration_cast<chrono::microseconds>(end - start).count();

            if (i == 0)
            {
                sol_h_force.sat = sat_hf;
                sol_h_dpll.sat = sat_hd;
                sol_h_force.assignment = h1;
                sol_h_dpll.assignment = h2;
            }

            // ================= IA =====================
            Assignment a1(formula.getNumVariables());
            start = chrono::high_resolution_clock::now();
            bool sat_if = BruteForceSAT::solve(formula, a1);
            end = chrono::high_resolution_clock::now();
            ia_force += chrono::duration_cast<chrono::microseconds>(end - start).count();

            Assignment a2(formula.getNumVariables());
            start = chrono::high_resolution_clock::now();
            bool sat_id = DPLLSAT::solve(formula, a2);
            end = chrono::high_resolution_clock::now();
            ia_dpll += chrono::duration_cast<chrono::microseconds>(end - start).count();

            if (i == 0)
            {
                sol_ia_force.sat = sat_if;
                sol_ia_dpll.sat = sat_id;
                sol_ia_force.assignment = a1;
                sol_ia_dpll.assignment = a2;
            }
        }

        // ================= TABELA DE TEMPOS =================
        cout << "\n================ TEMPO MEDIO (microssegundos) ================\n\n";
        cout << left << setw(25) << "Algoritmo"
             << setw(20) << "Humano"
             << setw(20) << "IA" << endl;

        cout << string(62, '-') << endl;

        cout << left << setw(25) << "Forca Bruta"
             << setw(20) << (humano_force / N_RUNS)
             << setw(20) << (ia_force / N_RUNS) << endl;

        cout << left << setw(25) << "DPLL"
             << setw(20) << (humano_dpll / N_RUNS)
             << setw(20) << (ia_dpll / N_RUNS) << endl;

        cout << string(62, '-') << endl;

        // ================= TABELA DE SOLUCOES =================
        auto printTruth = [](const TruthValue s)
        {
            switch (s)
            {
            case TruthValue::TRUE:
                return "TRUE";
                break;

            case TruthValue::FALSE:
                return "FALSE";
                break;

            default:
                return "UNASSIGNED";
                break;
            };
        };

        if (sol_h_force.sat && sol_ia_force.sat)
        {
            cout << "\n==================== SOLUCOES ENCONTRADAS ====================\n\n";
            cout << left << setw(15) << "Variável" << setw(20) << "Humano" << setw(20) << "IA" << endl;
            cout << string(62, '-') << endl;
            cout << "Força bruta" << endl;
            cout << string(62, '-') << endl;

            Assignment h_sol = sol_h_force.assignment;
            Assignment i_sol = sol_ia_force.assignment;

            for (int i = 1; i <= formula.getNumVariables(); i++)
            {
                cout << left << setw(13 + to_string(i).length()) << "x" + to_string(i) + ": " << setw(20)
                     << printTruth(h_sol.get(i)) << setw(20)
                     << printTruth(i_sol.get(i)) << endl;
            }

            cout << string(62, '-') << endl;
            cout << "DPLL" << endl;
            cout << string(62, '-') << endl;

            h_sol = sol_h_dpll.assignment;
            i_sol = sol_ia_dpll.assignment;

            for (int i = 1; i <= formula.getNumVariables(); i++)
            {
                cout << left << setw(13 + to_string(i).length()) << "x" + to_string(i) + ": " << setw(20)
                     << printTruth(h_sol.get(i)) << setw(20)
                     << printTruth(i_sol.get(i)) << endl;
            }
                }
        else
        {
            cout << "----------------- Nenhuma solução encontrada -----------------\n";
        }
        cout << string(62, '-') << endl;
    }
    catch (const std::exception &e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}
