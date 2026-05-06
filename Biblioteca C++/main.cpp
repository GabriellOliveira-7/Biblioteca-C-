#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constante para o tamanho máximo do acervo
#define TAMANHO_ACERVO 20
// Nome do arquivo de dados
#define NOME_ARQUIVO "acervo.dat"

// Estrutura (Struct) para armazenar os dados de um livro
struct Livro {
    int codigo;
    char titulo[50];
    char autor[30];
    char area[30];
    int ano;
    char editora[30];
};

// Assinaturas (Protótipos) das Funções
void exibirMenu();
void cadastrarLivros(struct Livro acervo[], int *totalLivros);
void imprimirLivros(struct Livro acervo[], int totalLivros);
void pesquisarLivro(struct Livro acervo[], int totalLivros, int codigoBusca);
void ordenarLivros(struct Livro acervo[], int totalLivros);
// Funções Novas para Manipulação de Arquivo
void carregarAcervo(struct Livro acervo[], int *totalLivros);
void salvarAcervo(struct Livro acervo[], int totalLivros);

// -----------------------------------------------------------
// Função Principal (main)
// -----------------------------------------------------------
int main() {
    struct Livro acervo[TAMANHO_ACERVO];
    int totalLivros = 0; 
    int opcao; 

    // CHAVE: Carrega os dados do arquivo antes de começar a usar o sistema
    carregarAcervo(acervo, &totalLivros);
    
    // Laço de Repetição do-while para exibir o menu repetidamente
    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        // Limpa o buffer após o scanf
        fflush(stdin); 
        
        switch (opcao) {
            case 1:
                cadastrarLivros(acervo, &totalLivros);
                break;
            case 2:
                imprimirLivros(acervo, totalLivros);
                break;
            case 3: {
                int codigoBusca;
                printf("\n--- Pesquisar Livro por Codigo ---\n");
                printf("Digite o codigo do livro: ");
                // Verifica se a leitura foi bem sucedida antes de usar a variável
                if (scanf("%d", &codigoBusca) == 1) {
                    pesquisarLivro(acervo, totalLivros, codigoBusca);
                } else {
                    printf("Entrada invalida para o codigo.\n");
                    // Limpa o buffer para evitar problemas no próximo loop
                    fflush(stdin); 
                }
                break;
            }
            case 4:
                ordenarLivros(acervo, totalLivros);
                break;
            case 5:
                // CHAVE: Salva os dados no arquivo antes de sair
                salvarAcervo(acervo, totalLivros);
                printf("\nDados salvos e Saindo do Sistema. Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida. Por favor, tente novamente.\n");
                break;
        }
        
        if (opcao != 5) {
            printf("\nPressione ENTER para continuar...");
            getchar(); 
            // system("clear"); 
        }
        
    } while (opcao != 5);
    
    return 0;
}

// -----------------------------------------------------------
// NOVAS FUNÇÕES DE ARQUIVO
// -----------------------------------------------------------

/**
 * Tenta carregar os dados do acervo do arquivo binário (acervo.dat).
 */
void carregarAcervo(struct Livro acervo[], int *totalLivros) {
    // Ponteiro para o arquivo
    FILE *arquivo;
    
    // Abre o arquivo em modo de leitura binária ("rb")
    arquivo = fopen(NOME_ARQUIVO, "rb");
    
    if (arquivo == NULL) {
        // Se o arquivo não existir ou não puder ser aberto, inicia com 0 livros.
        printf("Arquivo de acervo nao encontrado ou vazio. Iniciando um novo acervo.\n");
        *totalLivros = 0;
        return;
    }
    
    // Leitura do total de livros cadastrados (primeiro elemento do arquivo)
    // Lê o tamanho de um inteiro e armazena no endereço de totalLivros
    size_t lidos_total = fread(totalLivros, sizeof(int), 1, arquivo);
    
    // Se a leitura falhar (arquivo corrompido ou vazio inesperadamente)
    if (lidos_total != 1) {
        printf("Erro ao ler o total de livros do arquivo. Iniciando um novo acervo.\n");
        *totalLivros = 0;
        fclose(arquivo);
        return;
    }

    // Leitura do vetor de structs. Lê *totalLivros structs, cada uma do tamanho de struct Livro.
    // O total de livros lidos deve ser igual a *totalLivros
    size_t lidos_acervo = fread(acervo, sizeof(struct Livro), *totalLivros, arquivo);


    if (lidos_acervo == (size_t)*totalLivros) {
        printf("Acervo de %d livros carregado com sucesso!\n", *totalLivros);
    } else {
        printf("Alerta: Apenas %zu de %d livros foram lidos do arquivo.\n", lidos_acervo, *totalLivros);
        *totalLivros = (int)lidos_acervo; 
    }

    fclose(arquivo);
}

/**
 * Salva todos os livros cadastrados no arquivo binário (acervo.dat).
 */
void salvarAcervo(struct Livro acervo[], int totalLivros) {
    // Ponteiro para o arquivo
    FILE *arquivo;
    
    // Abre o arquivo em modo de escrita binária ("wb"). O "wb" tranca (apaga) o conteúdo anterior.
    arquivo = fopen(NOME_ARQUIVO, "wb");
    
    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel abrir/criar o arquivo para salvar!\n");
        return;
    }

    // Escrita do total de livros cadastrados (primeiro elemento do arquivo)
    // Isso é crucial para sabermos quantos structs ler depois.
    fwrite(&totalLivros, sizeof(int), 1, arquivo);

    // Escrita do vetor de structs
    // Escreve totalLivros structs, cada uma do tamanho de struct Livro
    size_t escritos = fwrite(acervo, sizeof(struct Livro), totalLivros, arquivo);
    
    if (escritos != (size_t)totalLivros) {
        printf("ALERTA: Apenas %zu de %d livros foram salvos no arquivo!\n", escritos, totalLivros);
    }
    
    fclose(arquivo);
}

// -----------------------------------------------------------
// FUNÇÕES RESTANTES (As funções de lógica permanecem as mesmas, apenas as assinaturas foram mantidas aqui)
// -----------------------------------------------------------

void exibirMenu() {
    printf("\n============================================\n");
    printf("        SISTEMA DE ACERVO DA BIBLIOTECA       \n");
    printf("============================================\n");
    printf("1 - Cadastrar livros\n");
    printf("2 - Imprimir todos os livros\n");
    printf("3 - Pesquisar livro por codigo\n");
    printf("4 - Ordenar livros por ano de publicacao\n");
    printf("5 - Sair do programa\n");
    printf("--------------------------------------------\n");
}

void cadastrarLivros(struct Livro acervo[], int *totalLivros) {
    int i;
    
    printf("\n--- Cadastro de Livros ---\n");
    
    if (*totalLivros >= TAMANHO_ACERVO) {
        printf("ERRO: O acervo esta cheio! Limite de %d livros atingido.\n", TAMANHO_ACERVO);
        return;
    }
    
    for (i = *totalLivros; i < TAMANHO_ACERVO; i++) {
        printf("\nLivro %d/%d\n", i + 1, TAMANHO_ACERVO);
        
        printf("Codigo: ");
        scanf("%d", &acervo[i].codigo);
        
        fflush(stdin); 
        
        printf("Titulo: ");
        scanf(" %[^\n]s", acervo[i].titulo); 
        
        printf("Autor: ");
        scanf(" %[^\n]s", acervo[i].autor);
        
        printf("Area: ");
        scanf(" %[^\n]s", acervo[i].area);
        
        printf("Ano de Publicacao: ");
        scanf("%d", &acervo[i].ano);
        
        fflush(stdin); // Limpa o buffer antes da última string
        
        printf("Editora: ");
        scanf(" %[^\n]s", acervo[i].editora);
        
        (*totalLivros)++;
        
        char continuar;
        printf("\nDeseja cadastrar outro livro (s/n)? ");
        scanf(" %c", &continuar); 
        
        if (continuar == 'n' || continuar == 'N') {
            break; 
        }
    }
}

void imprimirLivros(struct Livro acervo[], int totalLivros) {
    // ... (Implementação de imprimirLivros)
    int i;
    
    printf("\n--- Acervo Completo (%d livros) ---\n", totalLivros);
    
    if (totalLivros == 0) {
        printf("Nenhum livro cadastrado no acervo.\n");
        return;
    }
    
    for (i = 0; i < totalLivros; i++) {
        printf("\n--- Livro %d ---\n", i + 1);
        printf("Codigo: %d\n", acervo[i].codigo);
        printf("Titulo: %s\n", acervo[i].titulo);
        printf("Autor: %s\n", acervo[i].autor);
        printf("Area: %s\n", acervo[i].area);
        printf("Ano: %d\n", acervo[i].ano);
        printf("Editora: %s\n", acervo[i].editora);
    }
}

void pesquisarLivro(struct Livro acervo[], int totalLivros, int codigoBusca) {
    // ... (Implementação de pesquisarLivro)
    int i = 0;
    int encontrado = 0; 
    
    if (totalLivros == 0) {
        printf("Nenhum livro cadastrado para pesquisar.\n");
        return;
    }

    while (i < totalLivros) {
        if (acervo[i].codigo == codigoBusca) {
            printf("\nLIVRO ENCONTRADO:\n");
            printf("Titulo: %s\n", acervo[i].titulo);
            printf("Autor: %s\n", acervo[i].autor);
            printf("Area: %s\n", acervo[i].area);
            printf("Ano: %d\n", acervo[i].ano);
            printf("Editora: %s\n", acervo[i].editora);
            encontrado = 1; 
            break; 
        }
        i++;
    }
    
    if (encontrado == 0) {
        printf("\nLivro com codigo %d nao foi encontrado no acervo.\n", codigoBusca);
    }
}

void ordenarLivros(struct Livro acervo[], int totalLivros) {
    // ... (Implementação de ordenarLivros)
    int i, j;
    struct Livro temp; 
    
    if (totalLivros <= 1) {
        printf("\nNao ha livros suficientes para ordenar (total: %d).\n", totalLivros);
        return;
    }
    
    printf("\n--- Ordenando Livros por Ano de Publicacao ---\n");

    for (i = 0; i < totalLivros - 1; i++) {
        for (j = 0; j < totalLivros - i - 1; j++) {
            if (acervo[j].ano > acervo[j + 1].ano) {
                temp = acervo[j];
                acervo[j] = acervo[j + 1];
                acervo[j + 1] = temp;
            }
        }
    }
    
    printf("Ordenacao concluida!\n");
    imprimirLivros(acervo, totalLivros);
}
