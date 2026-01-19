# sat
O trabalho consiste em implementar uma solução do SAT, famoso problema NP-Completo. A ideia desse trabalho também é comparar a minha implementação com a implementação de uma LLM.

# Arquivos DIMACS

Como o trabalho lida com formulas em forma normal conjuntiva, foi implementado um parser de arquivos DIMACS. Um arquivo DIMACS segue as seguintes regras:

- Se a linha começa com 'c' é um comentário.
    
    c isso é um comentário em DIMACS

- A primeira linha válida começa com 'p cnf <num_variaveis> <num_clausulas>

    p cnf 3 2

- Após isso, cada linha representa uma cláusula, listando seus literais como inteiros:
- Número positivo i → variável 
- Número negativo -i → negação 
- Cada cláusula termina com 0

1 -2 0   -> (x1 v ~x2)
2  3 0   -> (x2 v x3)

