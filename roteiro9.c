/* Esta é uma Atividade Prática de programação:

Usando pipes como mecanismo de comunicação entre processos, crie um programa que execute o seguinte procedimento:

[check]- O processo pai, P0, lê do teclado um valor X inteiro entre 1-5.
[check]- O processo pai, P0, lê do teclado um segundo valor Y inteiro entre 6 e 10.
[check]- P0 cria três filhos, P1, P2 e P3.
[check]- P0 envia para o filho P1 via pipe (pipe1) o valor de X.
[check]- P0 envia para o filho P2 via pipe (pipe2) o valor de Y.

[check]- P0 envia para P1, pelo mesmo pipe1, a seguinte mensagem (string): "Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor X" (obs: ele envia o valor de X e não a letra 'X').

[check]- P0 envia para P2, pelo mesmo pipe2, a seguinte mensagem (string): "Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor Y" (obs: ele envia o valor de Y e não a letra 'Y').

[check]- P1 e P2 recebem e imprimem a mensagem enviada pelo pai (identifique se é P1 ou P2 quem printa cada mensagem).

[check]- P1 e P2 criam seus arrays de tamanho randômico, entre 100 e 200, preenchendo-os com valores X (no caso de P1) e Y (no caso de P2), conforme solicitado pelo pai.

[check]- P1 e P2 enviam os arrays para o irmão P3, via pipe3 (sim... eles usarão o mesmo pipe!! mas cuidado! as escritas podem se embaralhar.... resolvam isso!).

[check]- P3 lê os dois arrays enviados e printa os arrays na saída padrão. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h> // pipes
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

int main (){

    // LEITURA DE X E Y
    int X, Y;

    printf("Digite um valor de 1 a 5 para X: ");
    while(scanf("%d", &X) != 1 || X < 1 || X > 5) {
        printf("Inválido, tente novamente: ");
        while(getchar() != '\n');
    }
    
    printf("Digite um valor de 6 a 10 para Y: ");
    while(scanf("%d", &Y) != 1 || Y < 6 || Y > 10) {
        printf("Inválido, tente novamente: ");
        while(getchar() != '\n');
    }

    // CRIA PIPES * IMPORTANTE! TEM QUE SER CRIADO ANTES DO FORK PARA PROCESSOS FILHOS! *
    int pipe1[2], pipe2[2], pipe3[2];

    if (pipe(pipe1) == -1) {
        perror("erro ao criar pipe1");
        exit(1);
    }
    if (pipe(pipe2) == -1) {
        perror("erro ao criar pipe2");
        exit(1);
    }
    if (pipe(pipe3) == -1) {
        perror("erro ao criar pipe3");
        exit(1);
    }

    // CRIA FILHOS
    int p1 = fork(); //cria primeiro filho
    int p2 = fork(); //cria segundo e terceiro filho

    if (p1 > 0 && p2 > 0){ // Código do PAI
       
        close(pipe1[0]); // |
        close(pipe2[0]); // | fecha os lados de leitura de todos os pipes
        close(pipe3[0]); // |

        write(pipe1[1], &X, sizeof(X)); // envia o valor de X via pipe1
        write(pipe2[1], &Y, sizeof(Y)); // envia o valor de Y via pipe2

        // ENVIO DAS MSGs
        char msgX[150] = "Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor ";
        char msgY[150] = "Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor ";

        char X_str[2], Y_str[3]; // y precisa de 1 espaço a mais caso valor seja 10

        sprintf(X_str, "%d", X); // converte o valor de X para string
        sprintf(Y_str, "%d", Y); // converte o valor de Y para string
        strcat(msgX, X_str); // concatena X à mensagemX
        strcat(msgY, Y_str); // concatena Y à mensagemY
        write(pipe1[1], &msgX, sizeof(msgX)); // envia a mensagem de X via pipe1
        write(pipe2[1], &msgY, sizeof(msgY)); // envia a mensagem de Y via pipe2

        sleep(1);

        close(pipe1[1]); // |
        close(pipe2[1]); // | fecha os lados de escrita de todos os pipes
        close(pipe3[1]); // |

    } else if(p1 == 0 && p2 > 2){ // Código do PRIMEIRO FILHO

        char msg1[150];

        close(pipe1[1]); // fecha o lado de Escrita do pipe1
        close(pipe3[0]); // fecha o lado de Leitura do pipe3

        read(pipe1[0], &X, sizeof(X)); // lê o valor de X via pipe1
        read(pipe1[0], &msg1, sizeof(msg1)); // lê a mensagem via pipe1

        printf("[P1] %s\n", msg1); // imprime a mensagem recebida do pai pelo pipe1

        // VETOR DE TAMANHO RANDÔMICO ENTRE 100 E 200 PREENCHIDO COM  O VALOR DE X:
        
        srand(time(0)); // estabelece tempo atual como semente do gerador de numeros aleatórios

        int tam = rand() % 100 + 100; // "sorteia" tam entre 100 e 200
        char vetor[tam];
        char X_str[2];

        sprintf(X_str, "%d", X); // converte o valor de X para string

        for (int i=0; i<tam-1; i++) vetor[i] = X_str[0]; // preenche vetor com string X
        vetor[tam-1] = '\0'; // adiciona \0 ao final do vetor

        // == CUIDADO AQUI == pode embaralhar!
        write(pipe3[1], &vetor, sizeof(vetor)); // envia o vetor preenchido para o terceiro filho via pipe3
        
        close(pipe1[0]); // fecha o lado de Leitura do pipe1
        close(pipe3[1]); // fecha o lado de Escrita do pipe3

    } else if(p1 > 0 && p2 == 0){ // Código do SEGUNDO FILHO

        char msg2[150];

        close(pipe2[1]); // fecha o lado de Escrita do pipe2
        close(pipe3[0]); // fecha o lado de Leitura do pipe3
        
        read(pipe2[0], &Y, sizeof(Y)); // lê valor de Y via pipe2
        read(pipe2[0], &msg2, sizeof(msg2)); // lê mensagem via pipe2

        printf("[P2] %s\n", msg2); // imprime a mensagem recebida do pai pelo pipe2

        // VETOR DE TAMANHO RANDÔMICO ENTRE 100 E 200 PREENCHIDO COM  O VALOR DE Y:

        srand(time(0)); // estabelece tempo atual como semente do gerador de numeros aleatórios

        int tam = rand() % 100 + 100; // "sorteia" tam entre 100 e 200
        char vetor[tam];
        char Y_str[3];

        sprintf(Y_str, "%d", Y); // converte o valor de Y para string

        // Preenche vetor com string Y:
        if(Y_str[0] == '1' && Y_str[1] == '0'){ // caso número 10
            for (int i=0; i<tam; i+=2){
                vetor[i] = Y_str[0];
                vetor[i+1] = Y_str[1];
            }
        } else for (int i=0; i<tam; i++) vetor[i] = Y_str[0]; 
        
        vetor[tam-1] = '\0'; // adiciona \0 ao final do vetor

        // == CUIDADO AQUI == pode embaralhar!
        write(pipe3[1], &vetor, sizeof(vetor)); // envia o vetor preenchido para o terceiro filho via pipe3

        close(pipe2[0]); // fecha o lado de Leitura do pipe2
        close(pipe3[1]); // fecha o lado de Escrita do pipe3

    } else { // Código do TERCEIRO FILHO

        char msg3X[200], msg3Y[200];

        close(pipe3[1]); // fecha o lado de Escrita do pipe3

        read(pipe3[0], &msg3Y, sizeof(msg3Y)); // leitura da mensagem de Y via pipe3
        read(pipe3[0], &msg3X, sizeof(msg3X)); // leitura da mensagem de X via pipe3

        printf("[P3 from P1] %s\n", msg3X); 
        printf("[P3 from P2] %s\n", msg3Y);

        close(pipe3[0]); // fecha o lado de Leitura do pipe3

    }
    return 0;
}