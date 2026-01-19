#ifndef DIMACSPARSER_H
#define DIMACSPARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "FNC.h"

class DimacsParser
{
public:
    static FNC parse(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Erro ao abrir arquivo DIMACS: " + filename);
        }

        FNC formula;
        std::string token;

        int declaredVars = -1;
        int declaredClauses = -1;
        int parsedClauses = 0;

        while (file >> token)
        {

            // -------------------------
            // Comentários
            // -------------------------
            if (token == "c")
            {
                std::string ignored;
                std::getline(file, ignored); // ignora o resto da linha
                continue;
            }

            // -------------------------
            // Linha do problema
            // -------------------------
            if (token == "p")
            {
                std::string format;
                file >> format >> declaredVars >> declaredClauses;

                if (format != "cnf")
                {
                    throw std::runtime_error("Formato inválido (esperado 'cnf')");
                }

                if (declaredVars <= 0 || declaredClauses < 0)
                {
                    throw std::runtime_error("Número inválido de variáveis ou cláusulas");
                }

                formula.setNumVariables(declaredVars);
                continue;
            }

            // -------------------------
            // Cláusulas
            // -------------------------
            int lit = std::stoi(token);
            Clause clause;

            while (lit != 0)
            {
                int var = std::abs(lit);

                if (declaredVars == -1)
                {
                    throw std::runtime_error("Linha 'p cnf' não encontrada antes das cláusulas");
                }

                if (var < 1 || var > declaredVars)
                {
                    throw std::runtime_error("Variável fora do intervalo: " + std::to_string(var));
                }

                bool negated = (lit < 0);
                clause.addLiteral(Literal(var, negated));

                if (!(file >> lit))
                {
                    throw std::runtime_error("Arquivo terminou inesperadamente dentro de uma cláusula");
                }
            }

            formula.addClause(clause);
            parsedClauses++;
        }

        // -------------------------
        // Validações finais
        // -------------------------
        if (declaredVars == -1)
        {
            throw std::runtime_error("Arquivo DIMACS inválido: linha 'p cnf' ausente");
        }

        if (parsedClauses != declaredClauses)
        {
            throw std::runtime_error(
                "Número de cláusulas divergente. Declarado: " +
                std::to_string(declaredClauses) +
                ", Lido: " +
                std::to_string(parsedClauses));
        }

        return formula;
    }
};

#endif