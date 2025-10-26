// Inclusão das bibliotecas necessárias pro programa funcionar
#include <stdio.h>
#include <stdlib.h> // Para rand(), srand() e exit()
#include <time.h>   // Para a função time() (semente do rand)
#include <string.h> // Para strcmp (embora não esteja sendo usado, foi incluído)

// CONSTANTES E ESTRUTURAS GLOBAIS

#define TAMANHO_FILA 5     // Quantas peças aparecem na fila
#define CAPACIDADE_PILHA 3 // Quantas peças pode reservar (segurar)

// Struct 'Peca', o 'molde' para uma peça do jogo
struct Peca {
    char nome;
    int id;
};

// VARIÁVEIS GLOBAIS PARA A FILA E PILHA

// Fila Circular FIFO (First In, First Out)
struct Peca fila[TAMANHO_FILA];
int inicioFila = 0; // Índice de onde a peça sai
int fimFila = 0;   // Índice de onde a peça entra
int totalFila = 0;   // Contador de quantas peças tem na fila

// Pilha Linear LIFO (Last In, First Out)
struct Peca pilha[CAPACIDADE_PILHA];
int topo = -1; // Pilha vazia

// Funções
struct Peca gerarPeca();
void enqueue();
struct Peca dequeue();
void push(struct Peca peca);
struct Peca pop();
void jogarPeca();
void reservarPeca();
void usarPecaReservada();
void trocarPecaAtual(); // Nova função
void trocaMultipla(); // Nova função
void exibirEstado();

// FUNÇÃO PRINCIPAL
int main() {
    srand(time(NULL)); // Prepara o gerador aleatório
    int opcao;

    printf("Inicializando o Tetris Stack - Nível Mestre...\n");
    // Enche a fila de peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue();
    }

    while (1) { // Loop do jogo
        exibirEstado();
        printf("\nOpções Disponíveis:\n");
        printf("1. Jogar peça da frente da fila\n");
        printf("2. Enviar peça da fila para a pilha de reserva\n");
        printf("3. Usar peça da pilha de reserva\n");
        printf("4. Trocar peça da frente da fila com o topo da pilha\n");
        printf("5. Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0. Sair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: jogarPeca(); break;
            case 2: reservarPeca(); break;
            case 3: usarPecaReservada(); break;
            case 4: trocarPecaAtual(); break;
            case 5: trocaMultipla(); break;
            case 0: printf("\nSaindo do jogo...\n"); exit(0);
            default: printf("\n*** Opção inválida! ***\n");
        }
    }
    return 0;
}

// FUNÇÕES DE GERAÇÃO E ESTRUTURAS DE DADOS
struct Peca gerarPeca() {
    // 'static' faz o ID não zerar
    static int proximoId = 0;
    struct Peca novaPeca;
    char tiposDePeca[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    novaPeca.nome = tiposDePeca[rand() % 7];
    novaPeca.id = proximoId++;
    return novaPeca;
}

// Coloca uma peça no final da fila
void enqueue() {
    if (totalFila >= TAMANHO_FILA) return; // Fila cheia
    fila[fimFila] = gerarPeca();
    fimFila = (fimFila + 1) % TAMANHO_FILA; // O '%' faz a "volta" no vetor
    totalFila++;
}

// Tira uma peça do inicio da fila
struct Peca dequeue() {
    struct Peca pecaRemovida = {' ', -1}; // Peça vazia em caso de erro
    if (totalFila == 0) return pecaRemovida; // Fila vazia
    pecaRemovida = fila[inicioFila];
    inicioFila = (inicioFila + 1) % TAMANHO_FILA; // O 'inicio' avança
    totalFila--;
    return pecaRemovida;
}

// Coloca uma peça no topo da pilha
void push(struct Peca peca) {
    if (topo >= CAPACIDADE_PILHA - 1) return; // Pilha cheia
    topo++;
    pilha[topo] = peca;
}

// Tira uma peça do topo da pilha
struct Peca pop() {
    struct Peca pecaRemovida = {' ', -1}; // Peça vazia
    if (topo == -1) return pecaRemovida; // Pilha vazia
    pecaRemovida = pilha[topo];
    topo--;
    return pecaRemovida;
}

// FUNÇÕES DE AÇÃO DO JOGADOR

// Tira a peça da frente da fila e coloca uma nova no final
void jogarPeca() {
    if (totalFila == 0) { printf("\n>>> Fila vazia! <<<\n"); return; }
    struct Peca p = dequeue(); // Tira do início
    printf("\n>>> Ação: Peça [%c %d] jogada da fila. <<<\n", p.nome, p.id);
    enqueue(); // Adiciona uma nova no fim
}

// Tira a peça da frente da fila e coloca no topo da pilha (reserva)
void reservarPeca() {
    if (topo >= CAPACIDADE_PILHA - 1) { printf("\n>>> Pilha cheia! <<<\n"); return; }
    if (totalFila == 0) { printf("\n>>> Fila vazia! <<<\n"); return; }
    struct Peca p = dequeue(); // Tira da fila
    push(p); // Coloca na pilha
    printf("\n>>> Ação: Peça [%c %d] enviada para a reserva. <<<\n", p.nome, p.id);
    enqueue(); // Adiciona uma nova no fim da fila
}

// Tira a peça do topo da pilha (reserva) para usar no jogo
void usarPecaReservada() {
    if (topo == -1) { printf("\n>>> Pilha vazia! <<<\n"); return; }
    struct Peca p = pop(); // Tira da pilha
    printf("\n>>> Ação: Peça [%c %d] usada da reserva. <<<\n", p.nome, p.id);
}

// Troca a peça da frente da fila (inicioFila) com a peça do topo da pilha (topo)
// Não usa push/pop ou enqueue/dequeue, altera direto nos vetores
void trocarPecaAtual() {
    // Checa se a fila está vazia (totalFila == 0) ou se a pilha está vazia (topo == -1)
    if (totalFila == 0 || topo == -1) {
        printf("\n>>> Ação inválida: Fila e Pilha precisam ter peças para a troca. <<<\n");
        return;
    }
    struct Peca temp = fila[inicioFila]; // 1. Guarda a peça da fila
    fila[inicioFila] = pilha[topo];       // 2. Peça da pilha vai para a fila
    pilha[topo] = temp;                   // 3. Peça guardada (temp) vai para a pilha
    printf("\n>>> Ação: Troca simples realizada entre a frente da fila e o topo da pilha. <<<\n");
}

// Troca as 3 primeiras peças da fila com AS 3 peças da pilha
void trocaMultipla() {
    // Validação: checa se tem 3 na fila E se a pilha está cheia
    // 'topo == 2' significa que os índices 0, 1, 2 estão ocupados (pilha cheia)
    if (totalFila < 3 || topo < 2) {
        printf("\n>>> Ação inválida: A fila precisa ter no mínimo 3 peças e a pilha precisa estar cheia. <<<\n");
        return;
    }

    struct Peca temp;
    // Faz a troca 3 vezes (i=0, i=1, i=2)
    for (int i = 0; i < 3; i++) {
        // Calcula o índice na fila circular
        int indiceFila = (inicioFila + i) % TAMANHO_FILA;
        // O índice da pilha é direto (0, 1, 2)
        int indicePilha = i;

        // Faz a troca
        temp = fila[indiceFila];
        fila[indiceFila] = pilha[indicePilha];
        pilha[indicePilha] = temp;
    }
    printf("\n>>> Ação: Troca múltipla realizada entre as 3 primeiras peças da fila e as 3 da pilha. <<<\n");
}

// FUNÇÃO DE VISUALIZAÇÃO
// Mostra o estado da Fila e da Pilha
void exibirEstado() {
    printf("\n=======================================================\n");
    printf("                      ESTADO ATUAL\n");
    printf("-------------------------------------------------------\n");
    // Exibe a Fila
    printf("Fila de peças (Frente -> Fim):\n");
    if (totalFila == 0) {
        printf("[VAZIA]\n");
    } else {
        // Loop para mostrar na ordem certa (do 'inicio' até o 'fim')
        for (int i = 0; i < totalFila; i++) {
            int indice = (inicioFila + i) % TAMANHO_FILA;
            printf("[%c %d] ", fila[indice].nome, fila[indice].id);
        }
        printf("\n");
    }
    // Exibe a Pilha
    printf("\nPilha de reserva (Topo -> Base):\n");
    if (topo == -1) {
        printf("[VAZIA]\n");
    } else {
        // Mostra do topo (i = topo) para a base (i = 0)
        for (int i = topo; i >= 0; i--) {
            printf("[%c %d] ", pilha[i].nome, pilha[i].id);
        }
        printf("\n");
    }
    printf("=======================================================\n");
}
