#include <stdio.h>

void torre(int n, char origem, char aux, char destino){
    //caso base
    //no caso quando tem apenas um disco ele não utiliza o pino auxiliar apenas vai da origem para o destino
    if (n==1){
        printf("mova o disco 1 do pino %c para o pino %c\n", origem, destino);
        return;
    }
    //passo recursivo
    // vamos mover n-1 discos da origem para o auxiliar, usando o destino como apoio
    torre(n-1, origem, destino, aux);

    // vamos mover o disco maior (n) da origem para o destino
    printf("Mova o disco %d do pino %c para o pino %c\n", n, origem, destino);

    // por fim vamos mover os n-1 discos do auxiliar para o destino, usando a origem como apoio
    torre(n-1, aux, origem, destino);
}
// dentro da main é onde vai rodar rodar nossa torre, a gente define os pinos e o numero de discos que a gente quer resolver
int main(){
    int numerodiscos = 3;
    printf("Passo a passo para resolver a torre de Hanoi com %d discos: \n\n", numerodiscos);

    torre(numerodiscos, 'A', 'B', 'C');
    return 0;
}