#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"
#include "mapa.h"
#include "raylib.h"
#include "ranking.h"


#define ARQUIVO_RANKING "ranking.txt"
#define ARQUIVO_SAVE "jogo.dat"
#define TAM_NOME 10
#define POS_Y_TEXTO 100
#define DESLOCA_Y_TEXTO 50

/*
    char nome[NOMEMAX] - Nome de um jogador que será inserido numa estrutura do tipo Ranking
    int pontuacao      - Pontuacao de um jogador que será inserida numa estrutura do tipo Ranking
*/
Ranking cria_ranking(char nome[NOMEMAX], int pontuacao)
{
    Ranking ranking;
    strcpy(ranking.nome, nome);
    ranking.pontuacao = pontuacao;
    return ranking;
}

/*
    Ranking entradas[VETORMAX] - Vetor de rankings responsável por guardar o ranking de no máximo 5 jogadores
    int ocupacao - Número de endereços ocupados no vetor de rankings
*/
void salva_rankings(Ranking entradas[VETORMAX], int ocupacao)
{
    FILE *arquivo;
    int i = 0;
    arquivo = fopen(ARQUIVO_RANKING, "w");
    if(arquivo)
    {
        while(i < ocupacao) // Enquanto i for menor que ocupação...
        {
            // Guarda o nome e a pontuação do jogador do ranking[i] no arquivo.txt
            fputs(entradas[i].nome, arquivo);
            fprintf(arquivo, " ");
            fprintf(arquivo, "%d", entradas[i].pontuacao);
            fprintf(arquivo, "\n");
            i++;
        }
    }else
        printf(" Arquivo nao pode ser aberto!\n");
    fclose(arquivo);
}

/*
    Ranking entradas[VETORMAX] - Vetor de rankings responsável por guardar o ranking de no máximo 5 jogadores
    int *ocupacao - Endereço da variável que representa o número de endereços ocupados no vetor de rankings
*/
void carrega_rankings(Ranking entradas[VETORMAX], int *ocupacao)
{
    FILE *arquivo;
    int i = 0;         // Contador para preencher vetor inicia com 0.
    *ocupacao = 0;     // Conteúdo de ocupação inicia com 0 (será incrementado conforme vetor é preenchido)
    int valores_lidos;
    char nome[NOMEMAX];
    int pontuacao;
    arquivo = fopen("ranking.txt", "r");
    if(arquivo)
    {
        while(!feof(arquivo))  // Enquanto não chegou ao fim do arquivo
        {
            // Valores_lidos recebe o número de informações lidas pelo comando fscanf
            valores_lidos = fscanf(arquivo, "%s %d", nome, &pontuacao);

            // Se o fscanf leu pelo menos uma informação... (arquivo não está no fim)
            if(valores_lidos > 0)
            {
                strcpy(entradas[i].nome, nome);     // Copia o nome lido do arquivo para o nome do ranking[i] do vetor
                entradas[i].pontuacao = pontuacao;  // Copia a pontuação lida para a pontuação do ranking[i] do vetor
                (*ocupacao)++;                      // Incrementa 1 à ocupação
                i++;                                // Incrementa 1 ao contador
            }
        }
    }else
        printf(" Arquivo nao pode ser aberto!\n");
    fclose(arquivo);
}

/*
    Ranking entradas[VETORMAX] - Vetor de rankings responsável por guardar o ranking de no máximo 5 jogadores
    int ocupacao - Variável que representa o número de endereços ocupados no vetor de rankings
*/
void imprime_rankings(Ranking entradas[VETORMAX], int ocupacao)
{
    const int screenWidth = 550;
    const int screenHeight = 450;
    int i;
    char nome[NOMEMAX];
    int fecha = 1;
    SetWindowSize(screenWidth, screenHeight);

    while (fecha)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenha as posições e os campos de nome e pontuação
        DrawText("1ST", 100, 100, 20, BLACK);
        DrawText("2ND", 100, 150, 20, BLACK);
        DrawText("3RD", 100, 200, 20, BLACK);
        DrawText("4TH", 100, 250, 20, BLACK);
        DrawText("5TH", 100, 300, 20, BLACK);
        DrawText("Name", 200, 50, 20, BLACK);
        DrawText("Score", 350, 50, 20, BLACK);

        for(i = 0; i < ocupacao; i++)
        {
            DrawText(entradas[i].nome, 200, POS_Y_TEXTO + (i * DESLOCA_Y_TEXTO), 20, BLACK);
            DrawText(itoa(entradas[i].pontuacao, nome, 10), 350, POS_Y_TEXTO + (i * DESLOCA_Y_TEXTO), 20, BLACK);
        }

        if(IsKeyPressed(KEY_SPACE))
        {
            fecha = 0;
        }

        EndDrawing();
    }
}

/*
    Ranking entradas[VETORMAX] - Vetor de rankings responsável por guardar o ranking de no máximo 5 jogadores
    int *ocupacao - Endereço da variável que representa o número de endereços ocupados no vetor de rankings
    Ranking novo_jogador - Variável do tipo Ranking que será adicionada ao vetor de rankings adequadamente
*/
void adiciona_ranking(Ranking entradas[VETORMAX], int *ocupacao, Ranking novo_jogador)
{
    Ranking aux, aux2; // Variáveis auxiliares do tipo ranking para fazer as substituições
    int i, j;
    int fim = 1;
    if(*ocupacao == 0) // Se vetor de rankings não está com nenhum ranking...
    {
        entradas[*ocupacao] = novo_jogador; // Adiciona novo_jogador na primeira posição
        (*ocupacao)++;                      // Incrementa 1 à ocupação
    }else              // Senão...
    {
        for(i = 0; i < (*ocupacao) && fim; i++) // Iteração que repete enquanto i for menor que a ocupação do vetor e fim for verdadeiro
        {
            // Se a pontuação da posição i é menor ou igual a pontuação do novo_jogador...
            if(entradas[i].pontuacao <= novo_jogador.pontuacao)
            {
                aux = entradas[i];                     // aux guarda o ranking da posição i
                entradas[i] = novo_jogador;            // novo_jogador é colocado nessa posição
                for(j = i + 1; j < (*ocupacao); j++)  // Iteração para "deslocar para a direita" o resto do vetor
                {
                    aux2 = entradas[j];
                    entradas[j] = aux;
                    aux = aux2;
                }
                fim = 0;                               // flag de fim se torna falsa
                if(*ocupacao < 5)                      // Se ocupação é menor que 5 e novo_jogador foi inserido...
                    (*ocupacao)++;                     // incrementa 1 à ocupação

            }
        }
        // Se fim é verdadeiro (não foi encontrado um ranking com pontuação menor ou igual ao novo_jogador) e ocupação é menor que 5
        if(fim && *ocupacao < 5)
        {
            entradas[i] = novo_jogador; // Coloca jogador na primeira posição não ocupada do vetor
            (*ocupacao)++;              // Incrementa 1 à ocupação
        }
    }
}

void recebe_nome(Mapa *mapa)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetWindowSize(screenWidth, screenHeight);

    char nome[TAM_NOME + 1] = "\0";
    int contador = 0;
    int flag = 1;

    bool mouseTexto = false;

    Rectangle caixa_nome = { screenWidth/2.0f - 100, 180, 225, 50 };

    while (flag)
    {
        if (CheckCollisionPointRec(GetMousePosition(), caixa_nome))
        {
            mouseTexto = true;
        }

        else
        {
            mouseTexto = false;
        }
        if(mouseTexto)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int caracter = GetCharPressed();

                while (caracter > 0)
                {
                    if ((caracter >= 32) && (caracter <= 125) && (contador < TAM_NOME))
                    {
                        nome[contador] = (char)caracter;
                        nome[contador + 1] = '\0';
                        contador++;
                    }

                    caracter = GetCharPressed();
                }

                if(IsKeyPressed(KEY_BACKSPACE))
                {
                    contador--;
                    if(contador < 0)
                        contador = 0;
                    nome[contador] = '\0';
                }

            if(IsKeyPressed(KEY_ENTER))
            {
                strcpy(mapa->player.nome, nome);
                flag = 0;
            }

        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


        if(flag)
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("DIGITE O NOME DO JOGADOR", 260, 140, 20, DARKGREEN);
            DrawRectangleRec(caixa_nome, LIGHTGRAY);
            if (mouseTexto)
            {
                DrawRectangleLines((int)caixa_nome.x, (int)caixa_nome.y, (int)caixa_nome.width, (int)caixa_nome.height, GREEN);
            }
            else
            {
                DrawRectangleLines((int)caixa_nome.x, (int)caixa_nome.y, (int)caixa_nome.width, (int)caixa_nome.height, DARKGRAY);
            }
            DrawText(nome, (int)caixa_nome.x + 5, (int)caixa_nome.y + 8, 40, DARKBLUE);

            EndDrawing();
        }

    }
}




