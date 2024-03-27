Programa: expressoes.c

Autor: João Pedro Neffa de Sousa Oliveira

Data de criação: 22/03/2024

Descrição: Programa que recebe uma expressão lógica, como (~A v B) ^ C, e imprime sua tabela verdade.

O programa executa as expressoes logicas em ordem de importancia: (), ~, ^, v, ->, <->
por meio do algoritmo de Shunting Yard, que coloca a expressão em notação polonesa inversa
para ser calculado com base nos valores da tabela verdade das entradas.

Um exemplo do Shutting Yard:

      Símbolo Lido             Ação              Pilha Temporária         Saída        
    +--------------+---------------------------+------------------+-------------------+
    |      2       | Por na saída              |                  | 2                 |
    +--------------+---------------------------+------------------+-------------------+
    |      *       | Por na pilha              | *                | 2                 |
    +--------------+---------------------------+------------------+-------------------+
    |      3       | Por na saída              | *                | 2 3               |
    +--------------+---------------------------+------------------+-------------------+
    |      +       | Mover da pilha para saída |                  | 2 3 *             |
    |              | Por na pilha              | +                | 2 3 *             |
    +--------------+---------------------------+------------------+-------------------+
    |      4       | Por na saída              | +                | 2 3 * 4           |
    +--------------+---------------------------+------------------+-------------------+
    |      *       | Por na pilha              | + *              | 2 3 * 4           |
    +--------------+---------------------------+------------------+-------------------+
    |      (       | Por na pilha              | + * (            | 2 3 * 4           |
    +--------------+---------------------------+------------------+-------------------+
    |      5       | Por na saída              | + * (            | 2 3 * 4 5         |
    +--------------+---------------------------+------------------+-------------------+
    |      -       | Por na pilha              | + * ( -          | 2 3 * 4 5         |
    +--------------+---------------------------+------------------+-------------------+
    |      6       | Por na saída              | + * ( -          | 2 3 * 4 5 6       |
    +--------------+---------------------------+------------------+-------------------+
    |      )       | Mover da pilha para saída | + * (            | 2 3 * 4 5 6 -     |
    |              | Anular com '('            | + *              | 2 3 * 4 5 6 -     |
    +--------------+---------------------------+------------------+-------------------+
    |              | Pilha toda para saída     |                  | 2 3 * 4 5 6 - * + |
    +--------------+---------------------------+------------------+-------------------+
                            Resultado: 2 3 * 4 5 6 - * +

Após isso o programa calcula a lista que está em notação polonesa inversa:

      Símbolo Lido       Pilha      
    +--------------+---------------+
    |      2       | 2             |
    +--------------+---------------+
    |      3       | 2 3           |
    +--------------+---------------+
    |      *       | 6             |
    +--------------+---------------+
    |      4       | 6 4           |
    +--------------+---------------+
    |      5       | 6 4 5         |
    +--------------+---------------+
    |      6       | 6 4 5 6       |
    +--------------+---------------+
    |      -       | 6 4 -1        |
    +--------------+---------------+
    |      *       | 6 -4          |
    +--------------+---------------+
    |      +       | 2             |
    +--------------+---------------+
            Resultado: 2

O programa adapta esse processo para realizar esse mesmo calculo para todos os possiveis 
valores binarios, Ex.: 
(~A v B) ^ C ---> A~BvC^ em notação polonesa inversa.
A = 0, B = 1, C = 1 (011)

    Símbolo Lido       Pilha      
    +--------------+---------------+
    |      0       | 0             |
    +--------------+---------------+
    |      ~       | 1             | --> ~0
    +--------------+---------------+
    |      1       | 1 1           |
    +--------------+---------------+
    |      v       | 1             | --> 1 v 1
    +--------------+---------------+
    |      1       | 1 1           |
    +--------------+---------------+
    |      ^       | 1             | --> 1 ^ 1
    +--------------+---------------+
            Resultado: 1

Assim segue para valores de 000 até 111 e forma a tabela:
    --------------------Tabela Verdade--------------------
    A       B       C       |       (~A v B) ^ C

    0       0       0       |       0
    0       0       1       |       1
    0       1       0       |       0
    0       1       1       |       1
    1       0       0       |       0
    1       0       1       |       0
    1       1       0       |       0
    1       1       1       |       1

    -------------------------------------------------------

Se houverem valores repetidos, o programa considera o valor da variável repetida
como igual a de mesmo nome, porém continua tratando como uma variável normal da equação,
com a excessão que ela não é printada em uma coluna.
Ex.: A v A ^ A (000 ou 111)

    --------------------Tabela Verdade--------------------
    A       |       A v A ^ A

    0       |       0
    1       |       1

    ---------------------------------------------------