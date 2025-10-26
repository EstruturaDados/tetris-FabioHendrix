// Inclusão das bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h> // Para as funções rand(), srand() (aleatório) e exit() (sair)
#include <time.h>   // Para a função time() (usada no srand)

// --- CONSTANTES E ESTRUTURAS GLOBAIS ---

// Define o tamanho máximo da nossa fila de peças.
#define TAMANHO_FILA 5

// Struct 'Peca', o 'molde' para uma peça do jogo
struct Peca {
    char nome; // 'I', 'O', 'T', 'L'
    int id;   // Um número único pra peça
};

// VARIÁVEIS GLOBAIS PARA A FILA CIRCULAR 
struct Peca fila[TAMANHO_FILA]; // O vetor onde as peças ficam
int inicio = 0;         // Índice de onde a peça vai SAIR (dequeue)
int fim = 0;            // Índice de onde a próxima peça vai ENTRAR (enqueue)
int totalItens = 0;     // Contador para saber se a fila está cheia ou vazia

// Funções
struct Peca gerarPeca();
void enqueue();
void dequeue();
void exibirFila();

// FUNÇÃO PRINCIPAL 
int main() {
    // Prepara o gerador de números aleatórios (rand)
    // Usa o 'time(NULL)' como semente para as peças serem diferentes a cada jogo
    srand(time(NULL));

    int opcao;

    // Enche a fila de peças antes do jogo começar
    printf("Inicializando a fila de peças...\n");
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue();
    }

    // Loop principal do menu
    while (1) {
        exibirFila();
        printf("\nOpções de ação:\n");
        printf("1. Jogar peça (dequeue)\n");
        printf("2. Inserir nova peça (enqueue)\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                dequeue(); // Tira do começo
                break;
            case 2:
                enqueue(); // Coloca no fim
                break;
            case 0:
                printf("\nSaindo do Tetris Stack...\n");
                exit(0); // Fecha o programa
            default:
                printf("\n*** Opção inválida! Tente novamente. ***\n");
        }
    }

    return 0;
}

// Função que cria uma nova peça aleatória
struct Peca gerarPeca() {
    // 'static' faz o 'proximoId' NÃO ser zerado toda vez que a função é chamada.
    // Ele guarda o valor da última execução (0, 1, 2, 3...)
    static int proximoId = 0;

    struct Peca novaPeca;
    char tiposDePeca[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    
    // Sorteia um tipo (rand() % 7 dá um número de 0 a 6)
    novaPeca.nome = tiposDePeca[rand() % 7];
    // Atribui o ID e depois aumenta (proximoId++)
    novaPeca.id = proximoId++;
    
    return novaPeca;
}

// Adiciona uma nova peça no FIM da fila
void enqueue() {
    // Checa se a fila está cheia
    if (totalItens >= TAMANHO_FILA) {
        printf("\n>>> A fila de peças futuras está cheia! Jogue uma peça primeiro. <<<\n");
        return; // Sai da função
    }

    // Cria uma peça nova
    struct Peca novaPeca = gerarPeca();

    // Coloca a peça na posição 'fim'
    fila[fim] = novaPeca;

    // Atualiza o 'fim' para a próxima posição (de forma circular)
    // O '%' (módulo) faz o 'fim' voltar para 0 se ele chegar no limite.
    fim = (fim + 1) % TAMANHO_FILA;

    // Avisa que tem mais um item
    totalItens++;

    printf("\n>>> Nova peça [%c %d] adicionada à fila! <<<\n", novaPeca.nome, novaPeca.id);
}

// Remove uma peça do INÍCIO da fila
void dequeue() {
    // Checa se a fila está vazia
    if (totalItens == 0) {
        printf("\n>>> A fila está vazia! Não há peças para jogar. <<<\n");
        return;
    }

    // Pega a peça que está no 'inicio' (só para mostrar qual foi)
    struct Peca pecaJogada = fila[inicio];

    // Atualiza o 'inicio' para a próxima posição (de forma circular)
    inicio = (inicio + 1) % TAMANHO_FILA;
    
    // Avisa que tem um item a menos
    totalItens--;

    printf("\n>>> Peça [%c %d] foi jogada! <<<\n", pecaJogada.nome, pecaJogada.id);
}

// Mostra o estado atual da fila (do início ao fim)
void exibirFila() {
    printf("\n-----------------------------------\n");
    printf("Fila de peças: ");
    if (totalItens == 0) {
        printf("[VAZIA]");
    } else {
        // Para mostrar na ordem certa (do 'inicio' até o 'fim')
        for (int i = 0; i < totalItens; i++) {
            // Calcula o índice real (dando a volta no vetor se precisar)
            int indice = (inicio + i) % TAMANHO_FILA;
            printf("[%c %d] ", fila[indice].nome, fila[indice].id);
        }
    }
    printf("\n-----------------------------------\n");
}
