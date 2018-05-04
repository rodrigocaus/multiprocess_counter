/* Contador de números primos
 *
 * Este programa recebera uma serie números inteiros positivos em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela a
 * quantidade de números primos que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define N_FILHOS_MAX 4
#define ENTRADA_MAX 50

typedef struct node{
    pid_t id;
    struct node *prox;
} pid_node;

/*
 *  Verifica se x é primo.
 *  @return: 1 se x é primo, zero caso contrário
 */
int ehPrimo(int x);

/*
 *  Adiciona um nó PID à cabeça da lista, cujo ID=x
 *  @return: novo nó pid_node adicionado
 */
pid_node *addNode(int x, pid_node *list);

/*
 *  Limpa a lista de pid_node, aguardando os processos
 *  e dando free em cada nó
 */
void cleanList(pid_node *list);

int main() {

    //Vetor contador de primos compartilhado entre processos
    int *n_primos;
    n_primos = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANONYMOUS, 0, 0);

    //Vetor dos elementos de entrada
    unsigned long entrada[ENTRADA_MAX];

    int n_processos = 0;

    pid_node *pid_list = NULL;

    int index = 0;
    // Lê todos os numeros da entrada antes
    while(scanf("%lu ", &(entrada[index++])) != EOF);

    pid_t filho;
    for(int i = 0; i < index; i++) {
        if(n_processos < N_FILHOS_MAX) {
            filho = fork();
            if(filho == 0) {
                //Processos filhos calculam os primos
                int calc_primo = ehPrimo(entrada[i]);
                if(calc_primo)
                    (*n_primos) += 1;
                //sleep(1);
                exit(0);
            }
            else {
                //Somente o processo pai incrementa o número de processos
                n_processos++;
                //Adiciona à pilha de processos
                pid_list = addNode(filho, pid_list);
                if(pid_list == NULL) return 1;
            }
        }
        else {
            //O processo pai espera algum filho encerrar o calculo
            wait(NULL);
            n_processos--;
            i--;
        }
    }

    //Encerra todos os processos antes de imprimir
    cleanList(pid_list);

    printf("%d\n", (*n_primos));
    munmap(n_primos, sizeof(int));

    return 0;
}


int ehPrimo(int x) {
    if(x < 2)
        return 0;
    //Um numero primo só é divisível por 1 e por ele mesmo
    //Testamos a divisibilidade com i, 1<i<x
    for(int i = 2; i < x; i++) {
        if(!(x%i))
            return 0;
    }
    return 1;
}

pid_node *addNode(int x, pid_node *list) {
    pid_node *new = malloc(sizeof(pid_node));
    if(!new) return NULL;
    new->id = x;
    new->prox = list;
    return new;
}

void cleanList(pid_node *list) {
    while(list) {
        pid_node *aux = list;
        list = list->prox;
        waitpid(aux->id, NULL, 0);
        free(aux);
    }
}
