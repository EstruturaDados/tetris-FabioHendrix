// Inclusão das bibliotecas necessárias pro programa funcionar
#include <stdio.h>
#include <stdlib.h> // Para rand(), srand() e exit()
#include <time.h>   // Para a função time() (semente do rand)

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
int topo = -1; // A pilha começa vazia (topo = -1)

// Funções

// Funções de geração de peça
struct Peca gerarPeca();

// Funções de manipulação da Fila
void enqueue();
struct Peca dequeue();

// Funções de manipulação da Pilha
void push(struct Peca peca);
struct Peca pop();

// Funções de ação do jogador
void jogarPeca();
void reservarPeca();
void usarPecaReservada();

// Função de visualização
void exibirEstado();


// FUNÇÃO PRINCIPAL
int main() {
    srand(time(NULL)); // Prepara o gerador aleatório
    int opcao;

    printf("Inicializando o Tetris Stack...\n");
    // Enche a fila de peças antes do jogo começar
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue();
    }

    // Loop principal do jogo
    while (1) {
        exibirEstado(); // Mostra o tabuleiro (fila e pilha)
        printf("\nOpções de Ação:\n");
        printf("1. Jogar peça da fila\n");
        printf("2. Reservar peça (da fila para a pilha)\n");
        printf("3. Usar peça reservada (da pilha)\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarPecaReservada();
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                exit(0);
            default:
                printf("\n*** Opção inválida! ***\n");
        }
    }

    return 0;
}

// FUNÇÕES DE GERAÇÃO E ESTRUTURAS DE DADOS

struct Peca gerarPeca() {
    // 'static' faz o ID não zerar toda vez que a função é chamada
    static int proximoId = 0;
    struct Peca novaPeca;
    char tiposDePeca[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    novaPeca.nome = tiposDePeca[rand() % 7];
    novaPeca.id = proximoId++;
    return novaPeca;
}

// Operações da Fila
// Coloca uma peça no final da fila
void enqueue() {
    if (totalFila >= TAMANHO_FILA) return; // Fila cheia, não faz nada
    fila[fimFila] = gerarPeca();
    fimFila = (fimFila + 1) % TAMANHO_FILA; // O '%' faz a "volta" no vetor
    totalFila++;
}

// Tira uma peça do inicio da fila
struct Peca dequeue() {
    struct Peca pecaRemovida = { ' ', -1 }; // Cria uma "peça vazia" para retornar se der erro
    if (totalFila == 0) return pecaRemovida; // Fila vazia, retorna a peça vazia
    
    pecaRemovida = fila[inicioFila];
    inicioFila = (inicioFila + 1) % TAMANHO_FILA; // O inicio avança (dando a volta)
    totalFila--;
    return pecaRemovida;
}

// Operações da Pilha
// Coloca uma peça no topo da pilha
void push(struct Peca peca) {
    // Checa se a pilha está cheia (se o topo está no último índice)
    if (topo >= CAPACIDADE_PILHA - 1) { 
        printf("\n>>> A pilha de reserva está cheia! <<<\n");
        // (Nesse caso, a peça que ia entrar é "perdida")
        return;
    }
    topo++; // Sobe o topo
    pilha[topo] = peca; // Coloca a peça no novo topo
}

// Tira uma peça do topo da pilha
struct Peca pop() {
    struct Peca pecaRemovida = { ' ', -1 }; // Peça vazia para caso de erro
    if (topo == -1) return pecaRemovida; // Pilha vazia
    
    pecaRemovida = pilha[topo]; // Pega a peça do topo
    topo--; // Desce o topo
    return pecaRemovida;
}

// FUNÇÕES DE AÇÃO DO JOGADOR

// Tira a peça da frente da fila e coloca uma nova no final
void jogarPeca() {
    if (totalFila == 0) {
        printf("\n>>> Fila vazia, impossível jogar! <<<\n");
        return;
    }
    struct Peca pecaJogada = dequeue(); // Tira do início
    printf("\n>>> Peça [%c %d] foi jogada da fila! <<<\n", pecaJogada.nome, pecaJogada.id);
    enqueue(); // Adiciona uma nova peça no fim para repor
}

// Tira a peça da frente da fila e coloca no topo da pilha (reserva)
void reservarPeca() {
    // Checa se a pilha (destino) está cheia
    if (topo >= CAPACIDADE_PILHA - 1) {
        printf("\n>>> Pilha de reserva cheia! Impossível reservar. <<<\n");
        return;
    }
    // Checa se a fila (origem) está vazia
    if (totalFila == 0) {
        printf("\n>>> Fila vazia, impossível reservar! <<<\n");
        return;
    }
    struct Peca pecaReservada = dequeue(); // Tira da fila
    push(pecaReservada); // Coloca na pilha
    printf("\n>>> Peça [%c %d] movida da fila para a reserva! <<<\n", pecaReservada.nome, pecaReservada.id);
    enqueue(); // Adiciona uma nova peça no fim da fila para repor
}

// Tira a peça do topo da pilha (reserva) para usar no jogo
void usarPecaReservada() {
    if (topo == -1) {
        printf("\n>>> Pilha de reserva está vazia! <<<\n");
        return;
    }
    struct Peca pecaUsada = pop(); // Tira da pilha
    printf("\n>>> Peça [%c %d] foi usada da reserva! <<<\n", pecaUsada.nome, pecaUsada.id);
}


// FUNÇÃO DE VISUALIZAÇÃO

// Mostra o estado da Fila e da Pilha
void exibirEstado() {
    printf("\n============================================\n");
    printf("ESTADO ATUAL DO JOGO\n");
    
    // Exibe a Fila
    printf("Fila de peças (Frente -> Fim):\n");
    if (totalFila == 0) {
        printf("[VAZIA]\n");
    } else {
        // Loop para mostrar na ordem certa (do inicio até o fim)
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
    printf("============================================\n");
}
