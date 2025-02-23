#include <stdio.h>
#include <string.h>
#include "mapa.h"
#include "arquivo.h"
#include "raylib.h"

#define MAPA_L 30
#define MAPA_C 120
#define PIXELS 70
#define POSICAO_Y 30
#define DESLOCAMENTO_Y 70
#define POSICAO_X 0
#define DESLOCAMENTO_X 70
#define TAM_FONTE 20
#define TAM_PIXEL 70
#define INFOS 6

/*
    Mapa *mapa - Endereço de uma variável do tipo Mapa (função preenche o mapa, guarda as suas dimensões e preenche os flags de escada e porta)
*/
void mapa_carrega(Mapa *mapa, int nivel)
{
    int l, c;
    switch(nivel)
    {
        case 1:
            arq_carrega_mapa(mapa, "mapa1.txt");
            mapa->nivel = 1;
            mapa->escada = 0;
            mapa->bau = 0;
            mapa->saida = 0;
            mapa->porta = ' ';
            break;
        case 2:
            arq_carrega_mapa(mapa, "mapa2.txt");
            mapa->nivel = 2;
            mapa->escada = 0;
            mapa->bau = 0;
            mapa->saida = 0;
            mapa->porta = '1';
            break;
        case 3:
            arq_carrega_mapa(mapa, "mapa3.txt");
            mapa->nivel = 3;
            mapa->escada = 0;
            mapa->bau = 0;
            mapa->saida = 0;
            mapa->porta = ' ';
            break;
        default:
            break;
    }
    mapa->numeroBaus = 0;
    mapa->player.chave = 0;
    for(l = 0; l < mapa->dimensoes.linha; l++)
            {
                for(c = 0; c < mapa->dimensoes.coluna; c++)
                {
                    // CONTAGEM NÚMERO DE BAÚS DO MAPA
                    if(mapa->matriz[l][c] == 'C')
                    {
                        mapa->baus[mapa->numeroBaus].posicaoB = localizacao_cria(l, c);
                        mapa->numeroBaus++;
                    }
                    // INICIALIZAÇÃO DA SAÍDA
                    else if(mapa->matriz[l][c] == 'P')
                    {
                        mapa->matriz[l][c] = ' ';
                        mapa->posicaoSaida = localizacao_cria(l, c);
                    }
                    // INICIALIZAÇÃO POSIÇÃO INICIAL JOGADOR
                    else if(mapa->matriz[l][c] == 'D')
                    {
                        mapa->posicaoInicialJ = localizacao_cria(l, c);
                    }
                }
            }
    inicializa_baus(mapa);
    mapa->player.subindo = 0;
}

void mapa_muda_nivel(Mapa *mapa, int *nivel)
{
    (*nivel)++;
    mapa_carrega(mapa, *nivel);
    if(mapa->nivel < 4)
    {
        arq_salva_jogo(*mapa);
    }
    SetWindowSize((TAM_PIXEL * mapa->dimensoes.coluna), (30 + (TAM_PIXEL * mapa->dimensoes.linha)));
}

void mapa_reseta_player(Mapa *mapa)
{
    // SE JOGADOR ESTÁ SOBRE UMA ESCADA
    if(mapa->escada)
    {
        mapa->matriz[mapa->player.posicaoJ.linha][mapa->player.posicaoJ.coluna] = 'H';
        mapa->escada = 0;
    }else

    // SE JOGADOR ESTÁ SOBRE UM BAÚ
    if(mapa->bau)
    {
        mapa->matriz[mapa->player.posicaoJ.linha][mapa->player.posicaoJ.coluna] = 'C';
        mapa->bau = 0;
    }else

    // SE JOGADOR ESTÁ SOBRE UMA PORTA
    if(mapa->porta != ' ')
    {
        mapa->matriz[mapa->player.posicaoJ.linha][mapa->player.posicaoJ.coluna] = mapa->porta;
        mapa->porta = ' ';
    }else


    // SE JOGADOR ESTÁ SOBRE VAZIO
    mapa->matriz[mapa->player.posicaoJ.linha][mapa->player.posicaoJ.coluna] = ' ';

    // SE POSIÇÃO INICIAL DO JOGADOR FOR UMA ESCADA
    if(mapa->matriz[mapa->posicaoInicialJ.linha][mapa->posicaoInicialJ.coluna] == 'H')
        mapa->escada = 1;

    // SE POSIÇÃO INICIAL DO JOGADOR FOR UM BAÚ
    else if(mapa->matriz[mapa->posicaoInicialJ.linha][mapa->posicaoInicialJ.coluna] == 'C')
        mapa->bau = 1;

    // SE POSIÇÃO INICIAL DO JOGADOR FOR VAZIA, NÃO FAZ NADA
    else if(mapa->matriz[mapa->posicaoInicialJ.linha][mapa->posicaoInicialJ.coluna] == ' ')
        mapa->player.subindo = 0;
    // SE POSIÇÃO INICIAL DO JOGADOR FOR UMA PORTA
    else
        mapa->porta = mapa->matriz[mapa->posicaoInicialJ.linha][mapa->posicaoInicialJ.coluna];

    mapa->matriz[mapa->posicaoInicialJ.linha][mapa->posicaoInicialJ.coluna] = 'D';
}

/*
    Mapa *mapa  - Endereço de uma variável do tipo Mapa (função atualiza a posição do player guardada na variável mapa para ser a posição atual do player)
*/
void mapa_diminui_vida(Mapa *mapa)
{
    mapa->player.vidas--;     // Jogador perde uma vida
    mapa->player.dano = 1;           // Flag de dano recebe 1 (true)
    mapa_reseta_player(mapa); // Coloca o jogador na posição inicial do mapa novamente
}
/*
    Mapa *mapa  - Endereço de uma variável do tipo Mapa (função atualiza a posição do player guardada na variável mapa para ser a posição atual do player)
*/
void mapa_localiza_jogador(Mapa *mapa)
{
    int l, c;
    for(l = 0; l < mapa->dimensoes.linha; l++)
    {
        for(c = 0; c < mapa->dimensoes.coluna; c++)
        {
            if(mapa->matriz[l][c] == 'D')
            {
                mapa->player.posicaoJ.linha = l;
                mapa->player.posicaoJ.coluna = c;
            }
        }
    }
}

/*
    Mapa mapa                - Variável do tipo Mapa (função procura no mapa a porta par da porta na qual o player está sobre)
    char porta               - Caracter que representa a porta na qual o player está sobre
    Localizacao *local_porta - Endereço de uma variável do tipo Localização que receberá a posição do par da porta no mapa
*/
void mapa_busca_porta(Mapa mapa, char porta, Localizacao *local_porta)
{
    int l, c;
    int achou;
    do
    {
        achou = 0;
        for(l = 0; l < mapa.dimensoes.linha; l++)
        {
            for(c = 0; c < mapa.dimensoes.coluna; c++)
            {
                if(mapa.matriz[l][c] == porta)
                {
                    local_porta->linha = l;
                    local_porta->coluna = c;
                    achou = 1;
                }
            }
        }
    }while(achou == 0);
}

/*
    Mapa mapa - Variável do tipo Mapa (função imprime o mapa na tela)
*/
void mapa_imprime(Mapa mapa, Texturas texturas)
{
    /*
    Texture2D bloco_escada = LoadTexture("ladder_mid.png");
    Texture2D bloco_escada_topo = LoadTexture("ladder_top.png");
    Texture2D bloco_bau_aberto = LoadTexture("bau_aberto.png");
    Texture2D bloco_bau_fechado = LoadTexture("bau_fechado.png");
    Texture2D bloco_bau_queimado = LoadTexture("bau_queimado.png");
    Texture2D bloco_porta = LoadTexture("porta.png");
    Texture2D bloco_saida = LoadTexture("signExit.png");
    Texture2D plano_de_fundo = LoadTexture("plano_de_fundo.png");
    Texture2D player_idle = LoadTexture("cavaleiro_idle.png");
    Texture2D player_movimenta_direita1 = LoadTexture("cavaleiro_walk_direita1.png");
    Texture2D player_movimenta_direita2 = LoadTexture("cavaleiro_walk_direita2.png");
    Texture2D player_movimenta_esquerda1 = LoadTexture("cavaleiro_walk_esquerda1.png");
    Texture2D player_movimenta_esquerda2 = LoadTexture("cavaleiro_walk_esquerda2.png");
    Texture2D player_caindo = LoadTexture("cavaleiro_caindo.png");
    Texture2D player_machucado = LoadTexture("cavaleiro_machucado.png");
    Texture2D player_bau = LoadTexture("cavaleiro_bau.png");
    Texture2D player_bau_aberto = LoadTexture("cavaleiro_bau_aberto.png");
    Texture2D player_bau_queimado = LoadTexture("cavaleiro_bau_queimado.png");
    Texture2D player_escada1 = LoadTexture("player_ladder.png");
    Texture2D player_escada2 = LoadTexture("player_ladder2.png");
    Texture2D player_impacto = LoadTexture("cavaleiro_impacto.png");
    Texture2D teto = LoadTexture("teto.png");
    Texture2D teto_direita = LoadTexture("teto_direita.png");
    Texture2D teto_esquerda = LoadTexture("teto_esquerda.png");
    Texture2D chao = LoadTexture("chao.png");
    Texture2D chao_esquerda = LoadTexture("chao_esquerda.png");
    Texture2D chao_direita = LoadTexture("chao_direita.png");
    Texture2D player_porta = LoadTexture("cavaleiro_porta.png");
    Texture2D parede_esquerda = LoadTexture("parede_esquerda.png");
    Texture2D parede_direita = LoadTexture("parede_direita.png");
    Texture2D tijolo = LoadTexture("tijolo.png");
    */
    BeginDrawing();

    int i, j, c;
    mapa_imprime_info(mapa);
    for(i = 0; i < mapa.dimensoes.linha; i++)
    {
        for(j = 0; j < mapa.dimensoes.coluna; j++)
        {
           if(mapa.matriz[i][j] == 'X')
            {
                if(i == 0)
                {
                    if(j == 0)
                    {
                        DrawTexture(texturas.teto_esquerda, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                    else if(j == (mapa.dimensoes.coluna - 1))
                    {
                        DrawTexture(texturas.teto_direita, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                    else
                    {
                        DrawTexture(texturas.teto, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                }
                else if(i == (mapa.dimensoes.linha - 1))
                {
                    if(j == 0)
                    {
                        DrawTexture(texturas.chao_esquerda, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                    else if(j == (mapa.dimensoes.coluna - 1))
                    {
                        DrawTexture(texturas.chao_direita, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                    else
                    {
                        DrawTexture(texturas.chao, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                }
                else if(j == 0)
                {
                    if((i != 0) && (i != (mapa.dimensoes.linha - 1)))
                    {
                        DrawTexture(texturas.parede_esquerda, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                }
                else if(j == (mapa.dimensoes.coluna - 1))
                {
                    if((i != 0) && (i != (mapa.dimensoes.linha - 1)))
                    {
                        DrawTexture(texturas.parede_direita, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                }
                else
                {
                    DrawTexture(texturas.tijolo, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                }
            }

            else if(mapa.matriz[i][j] == 'C')
            {

                for(c = 0; c < mapa.numeroBaus; c++)
                {
                    if(i == mapa.baus[c].posicaoB.linha && j == mapa.baus[c].posicaoB.coluna)
                    {
                        if(mapa.baus[c].conteudo == ' ')
                        {
                            DrawTexture(texturas.bloco_bau_aberto, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                        }
                        else if(mapa.baus[c].conteudo == '*')
                        {
                            DrawTexture(texturas.bloco_bau_queimado, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                        }
                        else
                        {
                            DrawTexture(texturas.bloco_bau_fechado, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                        }
                    }
                }

            }
            else if(mapa.matriz[i][j] == 'H') {
                    if(mapa.matriz[i - 1][j] == ' '){
                        DrawTexture(texturas.bloco_escada_topo, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                    else {
                        DrawTexture(texturas.bloco_escada, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }

            }
            else if(mapa.matriz[i][j] == ' ')
                {
                    DrawTexture(texturas.plano_de_fundo, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                }
            else if(mapa.matriz[i][j] == 'D')
            {
                    if(mapa.porta != ' ')
                    {
                        DrawTexture(texturas.player_porta, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                    else if(mapa.escada)
                    {
                        if(mapa.player.subindo == 0)
                        {
                            DrawTexture(texturas.player_escada1, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                        }
                        else if(mapa.player.subindo == 1)
                        {
                            DrawTexture(texturas.player_escada2, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                        }
                    }
                    else if(mapa.player.impacto == 1)
                    {
                        DrawTexture(texturas.player_impacto, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                    else if(mapa.player.queda > 0)
                    {
                        DrawTexture(texturas.player_caindo, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                    else if(mapa.bau)
                    {
                        for(c = 0; c < mapa.numeroBaus; c++)
                        {
                            if(i == mapa.baus[c].posicaoB.linha && j == mapa.baus[c].posicaoB.coluna)
                            {
                                if(mapa.baus[c].conteudo == ' ')
                                {
                                    DrawTexture(texturas.player_bau_aberto, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                                }
                                else if(mapa.baus[c].conteudo == '*')
                                {
                                    DrawTexture(texturas.player_bau_queimado, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                                }
                                else
                                {
                                    DrawTexture(texturas.player_bau, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                                }
                            }
                        }
                    }
                    else if(mapa.player.dano)
                    {
                        DrawTexture(texturas.player_machucado, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                    else if(mapa.player.sentido == 1)
                    {
                        if(mapa.player.movimento == 2){
                        DrawTexture(texturas.player_movimenta_direita2, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                        }
                        else if(mapa.player.movimento == 1){
                        DrawTexture(texturas.player_movimenta_direita1, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                        }
                        else
                        DrawTexture(texturas.player_idle, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                    else if(mapa.player.sentido == 0)
                    {
                        if(mapa.player.movimento == 2){
                        DrawTexture(texturas.player_movimenta_esquerda2, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                        }
                        else if(mapa.player.movimento == 1){
                        DrawTexture(texturas.player_movimenta_esquerda1, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                        }
                        else
                        DrawTexture(texturas.player_idle, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
                    }
                }
            else if(mapa.matriz[i][j] == 'P')
            {
                DrawTexture(texturas.bloco_saida, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
            }
            else
            {
                DrawTexture(texturas.bloco_porta, POSICAO_X + (DESLOCAMENTO_X * j), POSICAO_Y + (DESLOCAMENTO_Y * i), RAYWHITE);
            }

        }
    }

    ClearBackground(RAYWHITE);
    EndDrawing();
}

void mapa_imprime_info(Mapa mapa)
{
    char TEXT[INFOS + 1][20];
    int deslocamento_texto = 40;
    int i;
    sprintf(TEXT[0], "SCORE:  ");
    sprintf(TEXT[1], "%d", mapa.player.pontuacao);
    sprintf(TEXT[2], "    NIVEL: %d", mapa.nivel);
    sprintf(TEXT[3], "VIDAS: %d", mapa.player.vidas);
    sprintf(TEXT[4], "Chave nao encontrada");
for(i = 0; i < INFOS; i++)
{
    if(i == 1)
        DrawText(TEXT[i], MeasureText(TEXT[0], TAM_FONTE) + 40, 10, TAM_FONTE, GREEN);
    else

    if(i == 4)
    {
        if(mapa.player.chave)
        {
            sprintf(TEXT[4], "Chave encontrada");
            DrawText(TEXT[4], deslocamento_texto, 10, TAM_FONTE, GREEN);
        }else {
            DrawText(TEXT[i], deslocamento_texto, 10, TAM_FONTE, BLACK);
            deslocamento_texto += (40 + MeasureText(TEXT[i], TAM_FONTE));
        }


    }else {
        DrawText(TEXT[i], deslocamento_texto, 10, TAM_FONTE, BLACK);
        deslocamento_texto += (40 + MeasureText(TEXT[i], TAM_FONTE));
    }
}
}

void mapa_muda_movimento(Mapa *mapa, bool movendo)
{
    if(movendo == false)
    {
        mapa->player.movimento = 0;
    }
    else if(movendo == true)
    {
        if(mapa->player.movimento == 1)
        {
            mapa->player.movimento = 2;
        }
        else if(mapa->player.movimento == 0)
        {
            mapa->player.movimento = 1;
        }
        else if(mapa->player.movimento == 2)
        {
            mapa->player.movimento = 1;
        }
    }
}

void mapa_muda_movimento_escada(Mapa *mapa)
{
    if(mapa->player.subindo == 0)
        mapa->player.subindo = 1;
    else if(mapa->player.subindo == 1)
        mapa->player.subindo = 0;
}


void mapa_inicializa_texturas(Texturas *texturas)
{
    texturas->bloco_escada = LoadTexture("ladder_mid.png");
    texturas->bloco_escada_topo = LoadTexture("ladder_top.png");
    texturas->bloco_bau_aberto = LoadTexture("bau_aberto.png");
    texturas->bloco_bau_fechado = LoadTexture("bau_fechado.png");
    texturas->bloco_bau_queimado = LoadTexture("bau_queimado.png");
    texturas->bloco_porta = LoadTexture("porta.png");
    texturas->bloco_saida = LoadTexture("signExit.png");
    texturas->plano_de_fundo = LoadTexture("plano_de_fundo.png");
    texturas->player_idle = LoadTexture("cavaleiro_idle.png");
    texturas->player_movimenta_direita1 = LoadTexture("cavaleiro_walk_direita1.png");
    texturas->player_movimenta_direita2 = LoadTexture("cavaleiro_walk_direita2.png");
    texturas->player_movimenta_esquerda1 = LoadTexture("cavaleiro_walk_esquerda1.png");
    texturas->player_movimenta_esquerda2 = LoadTexture("cavaleiro_walk_esquerda2.png");
    texturas->player_caindo = LoadTexture("cavaleiro_caindo.png");
    texturas->player_machucado = LoadTexture("cavaleiro_machucado.png");
    texturas->player_bau = LoadTexture("cavaleiro_bau.png");
    texturas->player_bau_aberto = LoadTexture("cavaleiro_bau_aberto.png");
    texturas->player_bau_queimado = LoadTexture("cavaleiro_bau_queimado.png");
    texturas->player_escada1 = LoadTexture("player_ladder.png");
    texturas->player_escada2 = LoadTexture("player_ladder2.png");
    texturas->player_impacto = LoadTexture("cavaleiro_impacto.png");
    texturas->teto = LoadTexture("teto.png");
    texturas->teto_direita = LoadTexture("teto_direita.png");
    texturas->teto_esquerda = LoadTexture("teto_esquerda.png");
    texturas->chao = LoadTexture("chao.png");
    texturas->chao_esquerda = LoadTexture("chao_esquerda.png");
    texturas->chao_direita = LoadTexture("chao_direita.png");
    texturas->player_porta = LoadTexture("cavaleiro_porta.png");
    texturas->parede_esquerda = LoadTexture("parede_esquerda.png");
    texturas->parede_direita = LoadTexture("parede_direita.png");
    texturas->tijolo = LoadTexture("tijolo.png");
}

