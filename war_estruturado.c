// ============================================================================
//         PROJETO WAR ESTRUTURADO
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
#define NUM_TERRITORIOS 10
#define NUM_MISSOES 5
#define MAX_NOME 30
#define MAX_COR 15

// --- Estrutura de Dados ---
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa);
void exibirMissao(int missaoId);
void faseDeAtaque(Territorio* mapa, const char* corJogador);
void simularAtaque(Territorio* mapa, int origem, int destino, const char* corJogador);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, const char* corJogador, int missaoId);
void limparBufferEntrada();

// --- Função Principal ---
int main() {
    srand(time(NULL));
    
    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para o mapa!\n");
        return 1;
    }
    
    inicializarTerritorios(mapa);
    
    const char* corJogador = "Azul";
    int missaoId = sortearMissao();
    
    printf("=== WAR ESTRUTURADO - DOMINE O MUNDO ===\n\n");
    printf("Bem-vindo, General do Exercito %s!\n", corJogador);
    printf("Sua missao foi definida. Boa sorte!\n\n");
    
    int opcao;
    int jogoAtivo = 1;
    
    do {
        exibirMapa(mapa);
        exibirMissao(missaoId);
        exibirMenuPrincipal();
        
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();
        
        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;
            case 2:
                if (verificarVitoria(mapa, corJogador, missaoId)) {
                    printf("\n*** PARABENS! Voce completou sua missao e venceu o jogo! ***\n");
                    jogoAtivo = 0;
                } else {
                    printf("\n*** Missao ainda nao foi cumprida. Continue lutando! ***\n");
                }
                break;
            case 0:
                printf("\nObrigado por jogar WAR Estruturado!\n");
                jogoAtivo = 0;
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
        
        if (jogoAtivo && opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while (jogoAtivo);
    
    liberarMemoria(mapa);
    return 0;
}


Territorio* alocarMapa() {
    return (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

void inicializarTerritorios(Territorio* mapa) {
    const char* nomes[NUM_TERRITORIOS] = {
        "Brasil", "Argentina", "Franca", "Alemanha", "Egito",
        "China", "Japao", "Australia", "Canada", "Mexico"
    };
    
    const char* cores[5] = {"Vermelho", "Verde", "Amarelo", "Preto", "Azul"};
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[rand() % 5]);
        mapa[i].tropas = rand() % 5 + 1;
    }
    
    strcpy(mapa[0].cor, "Azul");
    strcpy(mapa[1].cor, "Azul");
    mapa[0].tropas = 3;
    mapa[1].tropas = 2;
}

void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}

void exibirMenuPrincipal() {
    printf("\n=== MENU DE ACOES ===\n");
    printf("1. Atacar territorio inimigo\n");
    printf("2. Verificar condicao de vitoria\n");
    printf("0. Sair do jogo\n");
    printf("=====================\n");
}

void exibirMapa(const Territorio* mapa) {
    printf("\n=== MAPA DO MUNDO ===\n");
    printf("%-3s %-15s %-12s %-6s\n", "ID", "TERRITORIO", "EXERCITO", "TROPAS");
    printf("================================================\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-3d %-15s %-12s %-6d\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("================================================\n");
}

void exibirMissao(int missaoId) {
    printf("\n=== SUA MISSAO SECRETA ===\n");
    
    switch (missaoId) {
        case 1:
            printf("*** Destruir completamente o exercito VERMELHO\n");
            break;
        case 2:
            printf("*** Destruir completamente o exercito VERDE\n");
            break;
        case 3:
            printf("*** Conquistar pelo menos 6 territorios\n");
            break;
        case 4:
            printf("*** Conquistar pelo menos 4 territorios\n");
            break;
        case 5:
            printf("*** Destruir completamente o exercito AMARELO\n");
            break;
        default:
            printf("*** Missao desconhecida\n");
    }
    printf("==========================\n");
}

void faseDeAtaque(Territorio* mapa, const char* corJogador) {
    int origem, destino;
    
    printf("\n=== FASE DE ATAQUE ===\n");
    printf("Escolha o territorio de origem (1-%d): ", NUM_TERRITORIOS);
    scanf("%d", &origem);
    
    printf("Escolha o territorio de destino (1-%d): ", NUM_TERRITORIOS);
    scanf("%d", &destino);
    
    limparBufferEntrada();
    
    origem--;
    destino--;
    
    if (origem < 0 || origem >= NUM_TERRITORIOS || destino < 0 || destino >= NUM_TERRITORIOS) {
        printf("*** Territorios invalidos!\n");
        return;
    }
    
    if (origem == destino) {
        printf("*** Nao e possivel atacar o proprio territorio!\n");
        return;
    }
    
    simularAtaque(mapa, origem, destino, corJogador);
}

void simularAtaque(Territorio* mapa, int origem, int destino, const char* corJogador) {
    if (strcmp(mapa[origem].cor, corJogador) != 0) {
        printf("*** Voce so pode atacar a partir de seus proprios territorios!\n");
        return;
    }
    
    if (strcmp(mapa[destino].cor, corJogador) == 0) {
        printf("*** Voce nao pode atacar seus proprios territorios!\n");
        return;
    }
    
    if (mapa[origem].tropas <= 1) {
        printf("*** Voce precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }
    
    printf("\n*** BATALHA: %s (%s, %d tropas) VS %s (%s, %d tropas)\n",
           mapa[origem].nome, mapa[origem].cor, mapa[origem].tropas,
           mapa[destino].nome, mapa[destino].cor, mapa[destino].tropas);
    
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;
    
    printf("*** Atacante rolou: %d | Defensor rolou: %d\n", dadoAtacante, dadoDefensor);
    
    if (dadoAtacante > dadoDefensor) {
        mapa[destino].tropas--;
        printf("*** Ataque bem-sucedido! %s perdeu 1 tropa.\n", mapa[destino].nome);
        
        if (mapa[destino].tropas == 0) {
            strcpy(mapa[destino].cor, corJogador);
            mapa[destino].tropas = 1;
            mapa[origem].tropas--;
            printf("*** TERRITORIO CONQUISTADO! %s agora pertence ao exercito %s!\n",
                   mapa[destino].nome, corJogador);
        }
    } else {
        mapa[origem].tropas--;
        printf("*** Ataque repelido! %s perdeu 1 tropa.\n", mapa[origem].nome);
    }
}

int sortearMissao() {
    return rand() % NUM_MISSOES + 1;
}

int verificarVitoria(const Territorio* mapa, const char* corJogador, int missaoId) {
    int territoriosJogador = 0;
    int temVermelho = 0, temVerde = 0, temAmarelo = 0;
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            territoriosJogador++;
        } else if (strcmp(mapa[i].cor, "Vermelho") == 0) {
            temVermelho = 1;
        } else if (strcmp(mapa[i].cor, "Verde") == 0) {
            temVerde = 1;
        } else if (strcmp(mapa[i].cor, "Amarelo") == 0) {
            temAmarelo = 1;
        }
    }
    
    switch (missaoId) {
        case 1:
            return !temVermelho;
        case 2:
            return !temVerde;
        case 3:
            return territoriosJogador >= 6;
        case 4:
            return territoriosJogador >= 4;
        case 5:
            return !temAmarelo;
        default:
            return 0;
    }
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
