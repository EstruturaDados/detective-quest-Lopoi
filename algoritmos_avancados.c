#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código implementa os três níveis: Novato, Aventureiro e Mestre

// ============================================
// 🌱 NÍVEL NOVATO: ESTRUTURAS E FUNÇÕES
// ============================================

typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Cria uma nova sala
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Conecta salas na árvore
void conectarSalas(Sala* pai, Sala* esquerda, Sala* direita) {
    if (pai != NULL) {
        pai->esquerda = esquerda;
        pai->direita = direita;
    }
}

// Explora as salas interativamente
void explorarSalas(Sala* salaAtual);

// ============================================
// 🔍 NÍVEL AVENTUREIRO: ESTRUTURAS E FUNÇÕES
// ============================================

typedef struct NoPista {
    char texto[100];
    struct NoPista* esquerda;
    struct NoPista* direita;
} NoPista;

// Insere pista na BST
NoPista* inserirPista(NoPista* raiz, const char* texto) {
    if (raiz == NULL) {
        NoPista* novaPista = (NoPista*)malloc(sizeof(NoPista));
        strcpy(novaPista->texto, texto);
        novaPista->esquerda = NULL;
        novaPista->direita = NULL;
        return novaPista;
    }
    
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    }
    
    return raiz;
}

// Lista pistas em ordem alfabética
void listarPistas(NoPista* raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esquerda);
        printf("  - %s\n", raiz->texto);
        listarPistas(raiz->direita);
    }
}

// ============================================
// 🧠 NÍVEL MESTRE: ESTRUTURAS E FUNÇÕES
// ============================================

#define TAM_HASH 10

typedef struct ListaPistas {
    char pista[100];
    struct ListaPistas* prox;
} ListaPistas;

typedef struct Suspeito {
    char nome[50];
    ListaPistas* pistas;
    int contador;
    struct Suspeito* prox;
} Suspeito;

typedef struct {
    Suspeito* tabela[TAM_HASH];
} TabelaHash;

// Função hash simples baseada na soma ASCII
int funcaoHash(const char* nome) {
    int soma = 0;
    for (int i = 0; nome[i] != '\0'; i++) {
        soma += nome[i];
    }
    return soma % TAM_HASH;
}

// Inicializa a tabela hash
void inicializarHash(TabelaHash* th) {
    for (int i = 0; i < TAM_HASH; i++) {
        th->tabela[i] = NULL;
    }
}

// Insere relação pista-suspeito na tabela hash
void inserirHash(TabelaHash* th, const char* pista, const char* nomeSuspeito) {
    int indice = funcaoHash(nomeSuspeito);
    Suspeito* atual = th->tabela[indice];
    
    // Procura se o suspeito já existe
    while (atual != NULL) {
        if (strcmp(atual->nome, nomeSuspeito) == 0) {
            // Adiciona pista ao suspeito existente
            ListaPistas* novaPista = (ListaPistas*)malloc(sizeof(ListaPistas));
            strcpy(novaPista->pista, pista);
            novaPista->prox = atual->pistas;
            atual->pistas = novaPista;
            atual->contador++;
            return;
        }
        atual = atual->prox;
    }
    
    // Cria novo suspeito
    Suspeito* novoSuspeito = (Suspeito*)malloc(sizeof(Suspeito));
    strcpy(novoSuspeito->nome, nomeSuspeito);
    novoSuspeito->contador = 1;
    
    // Adiciona primeira pista
    ListaPistas* novaPista = (ListaPistas*)malloc(sizeof(ListaPistas));
    strcpy(novaPista->pista, pista);
    novaPista->prox = NULL;
    novoSuspeito->pistas = novaPista;
    
    // Insere no início da lista encadeada
    novoSuspeito->prox = th->tabela[indice];
    th->tabela[indice] = novoSuspeito;
}

// Lista todas as associações pista-suspeito
void listarAssociacoes(TabelaHash* th) {
    printf("\n=== RELACAO PISTAS x SUSPEITOS ===\n");
    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito* atual = th->tabela[i];
        while (atual != NULL) {
            printf("\nSuspeito: %s (%d pistas)\n", atual->nome, atual->contador);
            ListaPistas* p = atual->pistas;
            while (p != NULL) {
                printf("  - %s\n", p->pista);
                p = p->prox;
            }
            atual = atual->prox;
        }
    }
}

// Encontra o suspeito mais citado
void encontrarCulpado(TabelaHash* th) {
    Suspeito* maisProvavel = NULL;
    int maxPistas = 0;
    
    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito* atual = th->tabela[i];
        while (atual != NULL) {
            if (atual->contador > maxPistas) {
                maxPistas = atual->contador;
                maisProvavel = atual;
            }
            atual = atual->prox;
        }
    }
    
    if (maisProvavel != NULL) {
        printf("\n=== ANALISE FINAL ===\n");
        printf("Suspeito mais provavel: %s\n", maisProvavel->nome);
        printf("Numero de pistas: %d\n", maxPistas);
    }
}

// ============================================
// FUNÇÕES AUXILIARES - NÍVEL AVENTUREIRO
// ============================================

// Explora salas com coleta de pistas
void explorarSalasComPistas(Sala* salaAtual, NoPista** pistasColetadas, TabelaHash* th) {
    if (salaAtual == NULL) {
        printf("Voce esta em um beco sem saida!\n");
        return;
    }
    
    printf("\n--> Voce esta na sala: %s\n", salaAtual->nome);
    
    // Adiciona pistas específicas por sala (Nível Aventureiro e Mestre)
    if (strcmp(salaAtual->nome, "Biblioteca") == 0) {
        printf("[!] Pista encontrada: Livro manchado de sangue\n");
        *pistasColetadas = inserirPista(*pistasColetadas, "Livro manchado de sangue");
        inserirHash(th, "Livro manchado de sangue", "Professor Blackwood");
    } else if (strcmp(salaAtual->nome, "Cozinha") == 0) {
        printf("[!] Pista encontrada: Faca desaparecida\n");
        *pistasColetadas = inserirPista(*pistasColetadas, "Faca desaparecida");
        inserirHash(th, "Faca desaparecida", "Chef Madame Rouge");
    } else if (strcmp(salaAtual->nome, "Sotao") == 0) {
        printf("[!] Pista encontrada: Carta anonima\n");
        *pistasColetadas = inserirPista(*pistasColetadas, "Carta anonima");
        inserirHash(th, "Carta anonima", "Senhorita Violet");
    } else if (strcmp(salaAtual->nome, "Jardim") == 0) {
        printf("[!] Pista encontrada: Pegadas na lama\n");
        *pistasColetadas = inserirPista(*pistasColetadas, "Pegadas na lama");
        inserirHash(th, "Pegadas na lama", "Jardineiro Brown");
    } else if (strcmp(salaAtual->nome, "Quarto Principal") == 0) {
        printf("[!] Pista encontrada: Alianca roubada\n");
        *pistasColetadas = inserirPista(*pistasColetadas, "Alianca roubada");
        inserirHash(th, "Alianca roubada", "Senhorita Violet");
    } else if (strcmp(salaAtual->nome, "Adega") == 0) {
        printf("[!] Pista encontrada: Garrafa de veneno\n");
        *pistasColetadas = inserirPista(*pistasColetadas, "Garrafa de veneno");
        inserirHash(th, "Garrafa de veneno", "Senhorita Violet");
    }
    
    // Se é folha, retorna
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("Esta sala nao tem outras saidas.\n");
        return;
    }
    
    char opcao;
    while (1) {
        printf("\nOpcoes:\n");
        if (salaAtual->esquerda != NULL) printf("  [e] Ir para esquerda\n");
        if (salaAtual->direita != NULL) printf("  [d] Ir para direita\n");
        printf("  [p] Ver pistas coletadas\n");
        printf("  [a] Ver associacoes suspeitos\n");
        printf("  [s] Sair da exploracao\n");
        printf("Escolha: ");
        scanf(" %c", &opcao);
        
        if (opcao == 'e' && salaAtual->esquerda != NULL) {
            explorarSalasComPistas(salaAtual->esquerda, pistasColetadas, th);
            printf("\n--> Voce voltou para: %s\n", salaAtual->nome);
        } else if (opcao == 'd' && salaAtual->direita != NULL) {
            explorarSalasComPistas(salaAtual->direita, pistasColetadas, th);
            printf("\n--> Voce voltou para: %s\n", salaAtual->nome);
        } else if (opcao == 'p') {
            printf("\n=== PISTAS COLETADAS (em ordem alfabetica) ===\n");
            if (*pistasColetadas == NULL) {
                printf("  Nenhuma pista coletada ainda.\n");
            } else {
                listarPistas(*pistasColetadas);
            }
        } else if (opcao == 'a') {
            listarAssociacoes(th);
        } else if (opcao == 's') {
            printf("Saindo da exploracao...\n");
            break;
        } else {
            printf("Opcao invalida!\n");
        }
    }
}

// Explora salas (versão simples - Nível Novato)
void explorarSalas(Sala* salaAtual) {
    if (salaAtual == NULL) {
        printf("Voce esta em um beco sem saida!\n");
        return;
    }
    
    printf("\n--> Voce esta na sala: %s\n", salaAtual->nome);
    
    // Se é folha, retorna
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("Esta sala nao tem outras saidas.\n");
        return;
    }
    
    char opcao;
    while (1) {
        printf("\nOpcoes:\n");
        if (salaAtual->esquerda != NULL) printf("  [e] Ir para esquerda\n");
        if (salaAtual->direita != NULL) printf("  [d] Ir para direita\n");
        printf("  [s] Sair da exploracao\n");
        printf("Escolha: ");
        scanf(" %c", &opcao);
        
        if (opcao == 'e' && salaAtual->esquerda != NULL) {
            explorarSalas(salaAtual->esquerda);
            printf("\n--> Voce voltou para: %s\n", salaAtual->nome);
        } else if (opcao == 'd' && salaAtual->direita != NULL) {
            explorarSalas(salaAtual->direita);
            printf("\n--> Voce voltou para: %s\n", salaAtual->nome);
        } else if (opcao == 's') {
            printf("Saindo da exploracao...\n");
            break;
        } else {
            printf("Opcao invalida!\n");
        }
    }
}

// ============================================
// FUNÇÃO PRINCIPAL
// ============================================

int main() {
    printf("=== DETECTIVE QUEST ===\n");
    printf("Bem-vindo a mansao do misterio!\n\n");
    
    // Construção da árvore binária da mansão (Nível Novato)
    Sala* hall = criarSala("Hall de Entrada");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* sotao = criarSala("Sotao");
    Sala* jardim = criarSala("Jardim");
    Sala* quartoP = criarSala("Quarto Principal");
    Sala* adega = criarSala("Adega");
    
    // Conectando salas
    conectarSalas(hall, biblioteca, cozinha);
    conectarSalas(biblioteca, sotao, jardim);
    conectarSalas(cozinha, quartoP, adega);
    
    // Escolha do nível
    int nivel;
    printf("Escolha o nivel de jogo:\n");
    printf("  [1] Novato - Apenas exploracao\n");
    printf("  [2] Aventureiro - Exploracao com pistas\n");
    printf("  [3] Mestre - Exploracao, pistas e suspeitos\n");
    printf("Nivel: ");
    scanf("%d", &nivel);
    
    if (nivel == 1) {
        // Nível Novato: apenas exploração
        printf("\n=== NIVEL NOVATO ===\n");
        explorarSalas(hall);
    } else if (nivel == 2 || nivel == 3) {
        // Nível Aventureiro ou Mestre
        printf("\n=== NIVEL %s ===\n", nivel == 2 ? "AVENTUREIRO" : "MESTRE");
        NoPista* pistasColetadas = NULL;
        TabelaHash tabelaHash;
        inicializarHash(&tabelaHash);
        
        explorarSalasComPistas(hall, &pistasColetadas, &tabelaHash);
        
        // Resumo final
        printf("\n\n=== RESUMO DA INVESTIGACAO ===\n");
        printf("\n--- Pistas Coletadas ---\n");
        if (pistasColetadas == NULL) {
            printf("Nenhuma pista foi coletada.\n");
        } else {
            listarPistas(pistasColetadas);
        }
        
        if (nivel == 3) {
            listarAssociacoes(&tabelaHash);
            encontrarCulpado(&tabelaHash);
        }
    } else {
        printf("Nivel invalido!\n");
    }
    
    printf("\n\nObrigado por jogar Detective Quest!\n");
    
    return 0;
}
