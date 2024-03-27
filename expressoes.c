#include <stdio.h>
#include <string.h>
#include <math.h>
int first_op = -1, first_saida = -1, first_vars = -1;
char expr[100];

// Função que analisa o nível de precedência dos operadores lógicos
int lvl_precedencia(char op) {
    switch(op) {
        case '<':
            return 1;
        case '-':
            return 2;
        case '>':
            return 3;
        case 'v':
            return 4;
        case '^':
            return 5;
        case '~':
            return 6;
        default:
            return 0;
    }
}

// Função que calcula e retorna o resultado de uma operação lógica
int calc_logic_op(int a, int b, char op) {
    switch(op) {
        case '<': {
            if (a == b){
               return 1; 
            }
            return 0;
        }
        case '-': {
            if (a == 1 && b == 0){
               return 0; 
            }
            return 1;
        }
        case 'v':
            return a || b;
        case '^':
            return a && b;
        case '~':
            return !a;
        default:
            return -1;
    }
}

// Função que remove o operador to topo da lista de operadores e coloca na lista de saída.
// Se o operador for <-> ou ->, remove 3 e 2 caracteres, respectivamente, e insire
void rmv_op(char *lista_op, char *lista_saida) {
    int qnt_rmv;

    if (lista_op[first_op] == '>') {
        int op_ant = first_op - 2;
        char aux;

        if (lista_op[op_ant] == '<') {
            // Inverção dos caracteres '<' e '>' na lista de operador para quando inserir na outra ficar na ordem correta
            aux = lista_op[first_op];
            lista_op[first_op] = lista_op[op_ant];
            lista_op[op_ant] = aux;

            qnt_rmv=3;
        }
        else {
            // Inverção dos caracteres '-' e '>' na lista de operador para quando inserir na outra ficar na ordem correta
            op_ant++;
            aux = lista_op[first_op];
            lista_op[first_op] = lista_op[op_ant];
            lista_op[op_ant] = aux;

            qnt_rmv=2;
        }

    } else {
        qnt_rmv = 1;
    }

    for (int j = 0; j < qnt_rmv; j++){
        lista_saida[++first_saida] = lista_op[first_op];
        lista_saida[++first_saida] = '\0';
        first_saida--;
        first_op--;
    }
}
// Adiciona operador na lista de operadores
void add_op(char *lista_op, char valor_op) {
    lista_op[++first_op] = valor_op;
}

// Função para verificar de uma variável da expressão é repetida
int var_repetida(char *vars, char var, int index, int invertido) {
    if (!invertido) {
        for (int i = 0; i < index; i++) {
            if (vars[i] == var) {
                return 1;
            }
        }
        return 0;
    } else {
        for (int i = (int)strlen(vars) - 1; i > index; i--) {
            if (vars[i] == var) {
                return i;
            }
        }
        return 0;
    }
}

// Função que printa a tabela verdade
void print_tabela(int *binario, int resultado, char *vars) {
    for (int i = 0; i < (int)strlen(vars); i++) {
        if (!var_repetida(vars, vars[i], i, 1)) // se for variavel repetida não printa a coluna
            printf("%d\t", binario[i]);
    }
    printf("|\t%d\n", resultado);
    
}

char * shunting_yard(char *lista_saida, char *vars) {
    char lista_op[20];

    printf( "\nDigite uma expressao logica para receber sua tabela verdade\n"\
            "Simbolos permitidos: (), ~, ^, v, ->, <->\n");
    getchar();
    fgets(expr, sizeof(expr), stdin);

    
    for (int i = 0; i < (int)strlen(expr); i++) { // Enquanto houverem símbolos para serem lidos...
        
        if (expr[i] == ' ' || expr[i] == '\n') { continue; }

        else if (expr[i] != '(' && expr[i] != ')' && expr[i] != '~' && expr[i] != '^' && expr[i] != 'v' && expr[i] != '-' && expr[i] != '<' && expr[i] != '>' ) {
            lista_saida[++first_saida] = expr[i]; // Se é uma variavel, adicione à lista de saída
            vars[++first_vars] = expr[i]; // Guarda a variavel para printar depois
            vars[first_vars + 1] = '\0';
        } 
        
        else { // Se é um operador...

            if (expr[i] == '(') { // Se é uma abertura de parênteses, colocar na lista de operadores
                add_op(lista_op, expr[i]); 
            } 

            else if (expr[i] == ')') { // Se é um fechamento de parênteses...

                /* Enquanto o topo da lista de operadores não for uma abertura de parênteses, 
                jogue o operador do topo na lista de saída. */
                while (lista_op[first_op] != '(') {
                    rmv_op(lista_op, lista_saida);
                }

                first_op--; // Remova a abertura de parênteses da pilha temporária.
            }

            else { // Qualquer outro operador (o1)
                /* Enquanto existir um operador no topo da pilha temporária (o2)
                tal que a precedência de o1 seja menor que o2, mova o2 para a saída.
                Se o1 for um '~', coloca direto na lista de operadores, já que podem 
                haver operadores '~~' em sequencia */

                while ((lista_op[first_op] != '(') && (lvl_precedencia(lista_op[first_op]) > lvl_precedencia(expr[i]) || (lvl_precedencia(lista_op[first_op]) == lvl_precedencia(expr[i]) && lvl_precedencia(expr[i]) != 6))) {
                    rmv_op(lista_op, lista_saida);
                }
                add_op(lista_op, expr[i]); // Coloque o1 na lista de operadores.
            }
        }
    }

    // Ao não ter mais o que ler, mova todos os operadores da lista de operadores para a de saída
    while (first_op >= 0){
        rmv_op(lista_op, lista_saida);
    }
    return vars;
}

// Função que passa por cada valor possível das variáveis e imprime a tabela verdade
void eval_expr(char *expr_npi, char *vars) {
    int result, lista_result[30], first_lista = -1, count_var = 0;

    // Printa a primeira linha da tabela com o nome das variaveis e a expressão
    // Se for variável repetida não printa a coluna
    printf( "--------------------Tabela Verdade--------------------\n");
    for (int i = 0; i < (int)strlen(vars); i++) {
        if (!var_repetida(vars, vars[i], i, 0)) {
            printf("%c\t", vars[i]);
        } else {
            count_var++;
        }
    }
    printf("|\t%s\n", expr);

    int QNT_LINHAS = pow(2, ((int)strlen(vars) - count_var));
    for (int i = 0; i < QNT_LINHAS; i++) {
        int binario[(int)strlen(vars)], decimal_value = i;

        // calcula valor binário para as variáveis. ex.: [a, b, c] = [0, 0, 0]
        // Se for variavel repetida atribui o mesmo valor da outra
        for (int j = (int)strlen(vars) - 1; j >= 0; j--) {
            if (!var_repetida(vars, vars[j], j, 1)) {
                binario[j] = decimal_value % 2;
                decimal_value /= 2;
            } else {
                int index = var_repetida(vars, vars[j], j, 1);
                binario[j] = binario[index];
            }
        }

        // Executa as operações como notação polonesa inversa (npi) com os valores binarios calculados
        int count_var=0;
        for (int j=0; j < (int)strlen(expr_npi); j++) { // Enquanto houverem simbolos a serem lidos...
            if (expr_npi[j] != '~' && expr_npi[j] != '^' && expr_npi[j] != 'v' && expr_npi[j] != '-' && expr_npi[j] != '<' && expr_npi[j] != '>' ) {
                lista_result[++first_lista] = binario[count_var++]; // Adiciona valor binário da variavel na lista para calculo
            } 
            
            else if (expr_npi[j] == '-') {
                if (expr_npi[j-1] == '<') { // Se a ultima operação tiver sido um <->, pela para prox simbolo
                    j++;
                } else { // Se a operação for apenas um ->, calcula, joga o resultado pra lista e pula para o prox simbolo
                    result = calc_logic_op(lista_result[first_lista-1], lista_result[first_lista], expr_npi[j]);
                    lista_result[--first_lista] = result;
                    j++;
                }
            }

            else if (expr_npi[j] == '~') { // Se a operação for um ~, pega apenas o ultimo valor da lista e calcula
                result = calc_logic_op(lista_result[first_lista], 0, expr_npi[j]);
                lista_result[first_lista] = result;
            }
            
            else { // Qualquer outra operação calcula e joga resultado para a lista;
                result = calc_logic_op(lista_result[first_lista-1], lista_result[first_lista], expr_npi[j]);
                lista_result[--first_lista] = result;
            }
        }

        print_tabela(binario, lista_result[0], vars);
        first_lista--; // reinicializa a lista para proximo calculo
    }
}

int main(void) {
    int op = 0;

    do {
		printf( "\n---------------------------------------------------\n"\
                "Escolha uma opcao:\n"\
                "(1) Calcular tabela verdade de expressao logica\n"\
                "(0) Sair\n");
		scanf("%d", &op);

		switch (op) {
			case 0: { break; }

			case 1: {
                char lista_saida[30], vars1[10], *vars2;

				vars2 = shunting_yard(lista_saida, vars1);
                eval_expr(lista_saida, vars2);
                
                first_op = -1;
                first_saida = -1;
                first_vars = -1;
				break;
            }

            default: {
                printf("\nOpção invalida.\n");
				break;
            }
				
		}
	} while (op != 0);

    return 0;
}