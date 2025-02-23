#include <stdio.h>
#include "raylib.h"
#include "menu.h"
#include "arquivo.h"
#include "ranking.h"

#define TAM_FONTE 80
#define TAM_PIXEL 70

/*
    Esta função recebe um número inteiro de 0 a 3, imprimindo um menu de 4 opções onde
a opção correspondente ao número de entrada possui uma formatação e cor diferente das
demais.
*/
void DesenhaMenu(int opcao)
{

    // Declaração e definição de dados
    int i;
    char TEXT[OPCOES + 1][TAMANHO_TEXTO];     // Matriz com 5 linhas para armazenar as strings das 4 opções do menu e 1 string para uso posterior
    sprintf(TEXT[0],"NOVO JOGO");             // Guarda a string da direita na linha 0 da matriz
    sprintf(TEXT[1],"CARREGAR JOGO");         // Guarda a string da direita na linha 1 da matriz
    sprintf(TEXT[2],"RANKING PONTOS");        // Guarda a string da direita na linha 2 da matriz
    sprintf(TEXT[3],"SAIR");                  // Guarda a string da direita na linha 3 da matriz

    // Execução do comando que desenha as opções do menu na janela do raylib
    for(i = 0; i < OPCOES; i++)
    {
        if(i == opcao)
        {
            sprintf(TEXT[4], "[%s]", TEXT[i]);
            DrawText(TEXT[4], (LARGURA / 1.75) , POSICAO_Y_MENU + (DESLOCAMENTO_Y_MENU * i), TAMANHO_FONTE, RED);
        }
        else
        {
            DrawText(TEXT[i], (LARGURA / 1.75), POSICAO_Y_MENU + (DESLOCAMENTO_Y_MENU * i), TAMANHO_FONTE, WHITE);
        }
    }

    /* A fórmula utilizada para centralizar os textos é (LARGURA - (MeasureText(TEXT[i], TAMANHO_FONTE))) / 2, onde o tamanho da largura da janela
    do raylib subtraido pelo largura do texto dá o valor do espaço em branco. Como queremos deslocar o texto para o centro, dividimos esse espaço em
    branco em 2 "pedaços" iguais. Logo, o valor da largura do espaço em branco dividido por 2 nos devolve o deslocamento para a direita que o texto
    deve ter para ficar centralizado no eixo X. */

    EndDrawing();
}

/*
    Esta função desenha um menu interativo que pode ser navegado com as teclas do teclado.
Setas para cima e baixo deslocam a opção selecionada no menu e ENTER retorna a opção sele-
nada para a função main para executar o comando adequado.
*/

void menuJogo(Mapa *mapa, int *nivel, int *flag, Ranking ranking[], int *posicoes)
{
    int opcao = 0;                        // Inicia opção como zero (menu inicia com a primeira opção selecionada)
    int fim = 1;

    SetWindowSize(LARGURA, ALTURA);
    SetWindowTitle("Lightest Dungeon");
    Texture2D background = LoadTexture("menu.png");

    while (*flag && fim)  // Encerra loop quando X da janela for clicado ou quando flag se tornar falsa
    {

        // Desenha as opções do menu e a string guardada no vetor mensagem (no topo) dentro da janela do raylib
        BeginDrawing();
        DrawTexture(background, 0, 0, RAYWHITE);
        DesenhaMenu(opcao);

        // Modifica o menu conforme o input do usuário
        if(IsKeyPressed(KEY_DOWN))   // Se usuário aperta a tecla da seta para baixo...
        {
            opcao++;                 // Soma 1 a opção (mudando a entrada da função desenho -> desloca a opção selecionada para baixo no menu)
            if(opcao > 3)            // Se opção ultrapassar o valor 3, opção se torna 0 novamente (menu circular)
            {
                opcao = 0;
            }
        }
        if(IsKeyPressed(KEY_UP))     // Se usuário aperta a tecla da seta para cima...
        {
            opcao--;                 // Subtrai 1 da opção (mudando a entrada da função desenho -> desloca a opção selecionada para cima no menu)
            if(opcao < 0)                // Se opção se tornar negativa, opção se torna 3 (menu circular)
            {
                opcao = 3;
            }
        }
        if(IsKeyPressed(KEY_ENTER))  // Se usuário aperta a tecla ENTER
        {
            switch(opcao)
            {
                case 0:
                    fim = 0;
                    mapa_carrega(mapa, *nivel);
                    mapa->player = jog_inicializa(mapa->posicaoInicialJ.linha, mapa->posicaoInicialJ.coluna);
                    SetWindowSize((TAM_PIXEL * mapa->dimensoes.coluna), (30 + (TAM_PIXEL * mapa->dimensoes.linha)));
                    break;
                case 1:
                    fim = 0;
                    arq_recupera_jogo(mapa);
                    *nivel = mapa->nivel;
                    SetWindowSize((TAM_PIXEL * mapa->dimensoes.coluna), (30 + (TAM_PIXEL * mapa->dimensoes.linha)));
                    break;
                case 2:
                    carrega_rankings(ranking, posicoes);
                    imprime_rankings(ranking, *posicoes);
                    SetWindowSize(LARGURA, ALTURA);
                    break;
                case 3:
                    *flag = 0;
                    break;
            }
        }
    }
}

void gameOver(Mapa *mapa, int *menu)
{
    BeginDrawing();

    int altura = ((30 + (TAM_PIXEL * mapa->dimensoes.linha)) - TAM_FONTE) / 2;
    int largura = ((TAM_PIXEL * mapa->dimensoes.coluna) - MeasureText("GAME OVER", TAM_FONTE)) / 2;
    DrawText("GAME OVER", largura, altura, TAM_FONTE, RED);
    DrawText("[1] - Continue?", largura, altura + 80, TAM_FONTE - 40, RED);
    DrawText("[2] - Give Up?", largura, altura + 120, TAM_FONTE - 40, RED);
    if(IsKeyPressed(KEY_ONE))
    {
        arq_recupera_jogo(mapa);
    }
    else if(IsKeyPressed(KEY_TWO))
    {
        *menu = 1;
    }

    ClearBackground(RAYWHITE);
    EndDrawing();
}

void pausa_jogo(Mapa mapa, int *pause)
{
    int altura = ((30 + (TAM_PIXEL * mapa.dimensoes.linha)) - TAM_FONTE) / 2;
    int largura = ((TAM_PIXEL * mapa.dimensoes.coluna) - MeasureText("PAUSED", TAM_FONTE)) / 2;

    while(*pause)
    {
        BeginDrawing();
        DrawText("PAUSED", largura, altura, TAM_FONTE, WHITE);
        EndDrawing();
        if(IsKeyPressed(KEY_ENTER))
        {
            *pause = 0;
        }
    }
}
