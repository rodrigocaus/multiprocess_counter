/* Contador de números primos
 *
 * Este programa recebera uma serie números inteiros positivos em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela a
 * quantidade de números primos que recebeu e, apos, encerrar.
 */

#include <stdio.h>

/*
 *  Verifica se x é primo.
 *  @return: 1 se x é primo, zero caso contrário
 */
int ehPrimo(int x);


int main() {
    int x = 0, y;
    while(scanf("%d ", &y) != EOF) {
        if(ehPrimo(y))
            x++;
    }
    printf("%d\n", x);
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
