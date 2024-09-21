#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "forca.h"

char palavrasecreta[TAMANHO_PALAVRA];
char chutes[26];
int chutesdados = 0;

void resetarJogo() {
    chutesdados = 0;  // Reinicia o número de chutes dados
    memset(chutes, 0, sizeof(chutes));  // Limpa o array de chutes
}

int letraexiste(char letra) {
    for (int j = 0; j < strlen(palavrasecreta); j++) {
        if (letra == palavrasecreta[j]) {
            return 1;
        }
    }
    return 0;
}

int chuteserrados() {
    int erros = 0;
    for (int i = 0; i < chutesdados; i++) {
        if (!letraexiste(chutes[i])) {
            erros++;
        }
    }
    return erros;
}

int enforcou() {
    return chuteserrados() >= 5;
}

int ganhou() {
    for (int i = 0; i < strlen(palavrasecreta); i++) {
        if (!jachutou(palavrasecreta[i])) {
            return 0;
        }
    }
    return 1;
}

void abertura() {
    printf("/****************/\n");
    printf("/ Jogo de Forca */\n");
    printf("/****************/\n\n");
}

void chuta() {
    char chute;
    printf("Qual letra? ");
    scanf(" %c", &chute);

    if (letraexiste(chute)) {
        printf("Você acertou: a palavra tem a letra %c\n\n", chute);
    } else {
        printf("\nVocê errou: a palavra NÃO tem a letra %c\n\n", chute);
    }

    chutes[chutesdados] = chute;
    chutesdados++;
}

int jachutou(char letra) {
    for (int j = 0; j < chutesdados; j++) {
        if (chutes[j] == letra) {
            return 1;
        }
    }
    return 0;
}

void desenhaforca() {
    int erros = chuteserrados();

    printf("  _______       \n");
    printf(" |/      |      \n");
    printf(" |      %c%c%c  \n", (erros >= 1 ? '(' : ' '), (erros >= 1 ? '_' : ' '), (erros >= 1 ? ')' : ' '));
    printf(" |      %c%c%c  \n", (erros >= 3 ? '\\' : ' '), (erros >= 2 ? '|' : ' '), (erros >= 3 ? '/' : ' '));
    printf(" |       %c     \n", (erros >= 2 ? '|' : ' '));
    printf(" |      %c %c   \n", (erros >= 4 ? '/' : ' '), (erros >= 4 ? '\\' : ' '));
    printf(" |              \n");
    printf("_|___           \n");
    printf("\n\n");

    for (int i = 0; i < strlen(palavrasecreta); i++) {
        if (jachutou(palavrasecreta[i])) {
            printf("%c ", palavrasecreta[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

void escolhepalavra() {
    FILE *f = fopen("palavras.txt", "r");
    if (f == NULL) {
        printf("Banco de dados de palavras não disponível\n\n");
        exit(1);
    }

    int qtddepalavras;
    fscanf(f, "%d", &qtddepalavras);

    srand(time(0));
    int randomico = rand() % qtddepalavras;

    for (int i = 0; i <= randomico; i++) {
        fscanf(f, "%s", palavrasecreta);
    }

    fclose(f);
}

void adicionapalavra() {
    char quer;

    printf("Você deseja adicionar uma nova palavra no jogo (S/N)? ");
    scanf(" %c", &quer);

    if (quer == 'S') {
        char novapalavra[TAMANHO_PALAVRA];

        printf("Digite a nova palavra, em letras maiúsculas: ");
        scanf("%s", novapalavra);

        FILE *f = fopen("palavras.txt", "r+");
        if (f == NULL) {
            printf("Banco de dados de palavras não disponível\n\n");
            exit(1);
        }

        int qtd;
        fscanf(f, "%d", &qtd);
        qtd++;
        fseek(f, 0, SEEK_SET);
        fprintf(f, "%d", qtd);

        fseek(f, 0, SEEK_END);
        fprintf(f, "\n%s", novapalavra);

        fclose(f);
    }
}

void forca() {
    int repetir;
    do {
        resetarJogo(); // Reinicia o jogo antes de cada nova rodada
        escolhepalavra();

        do {
            desenhaforca();
            chuta();
        } while (!ganhou() && !enforcou());

        if (ganhou()) {
            printf("\nParabéns, você ganhou!\n\n");
            printf("       ___________      \n");
            printf("      '._==_==_=_.'     \n");
            printf("      .-\\:      /-.    \n");
            printf("     | (|:.     |) |    \n");
            printf("      '-|:.     |-'     \n");
            printf("        \\::.    /      \n");
            printf("         '::. .'        \n");
            printf("           ) (          \n");
            printf("         _.' '._        \n");
            printf("        '-------'       \n\n");
        } else {
            printf("\nPuxa, você foi enforcado!\n");
            printf("A palavra era **%s**\n\n", palavrasecreta);
        }

        printf("Repetir? (1) Sim ou (0) Não\n");
        scanf("%d", &repetir);
        
    } while (repetir);
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    abertura();
    forca();
    return 0;
}