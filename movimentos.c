#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "mapa.h"

/*
    Mapa *mapa - Endereço de uma variável do tipo Mapa (função atualiza o mapa para derrubar jogador enquanto ele estiver flutuando)
*/
void gravidade(Mapa *mapa)
{
    char* posicaoAtual = &(mapa->matriz[mapa->player.posicaoJ.linha][mapa->player.posicaoJ.coluna]);
    char* posicaoAbaixo = &(mapa->matriz[mapa->player.posicaoJ.linha + 1][mapa->player.posicaoJ.coluna]);

    if(!(mapa->escada))
    {
        // SE JOGADOR ESTÁ ACIMA DE UM BAÚ
        if (*posicaoAbaixo == 'C')
        {
            mapa->bau = 1;
        }
        // SE JOGADOR ESTÁ ACIMA DA SAÍDA
        else if(*posicaoAbaixo == 'P')
        {
            mapa->player.queda = 0;
            mapa->nivel++;
        }
        // SE JOGADOR ESTÁ ACIMA DE UMA PORTA
        else
        {
            mapa->porta = *posicaoAbaixo;
        }

        *posicaoAtual = ' ';
        *posicaoAbaixo = 'D';
        (mapa->player.queda)++;
    }
}


/*
    Mapa *mapa  - Endereço de uma variável do tipo Mapa (função atualiza o mapa para movimentos verticais válidos do jogador)
    int direcao - Inteiro que representa a direcao para qual o usuário deseja movimentar o jogador (-1 para cima, 1 para baixo)
*/
void movimenta_vertical(Mapa *mapa, int direcao)
{
    Localizacao local_porta;       // Inicia variável do tipo localização que representa a posição de uma porta
    char* posicaoAtual = &(mapa->matriz[mapa->player.posicaoJ.linha][mapa->player.posicaoJ.coluna]);
    char* posicaoSeguinte = &(mapa->matriz[mapa->player.posicaoJ.linha + direcao][mapa->player.posicaoJ.coluna]);

    // SE JOGADOR ENTROU EM UMA PORTA
    if(mapa->porta != ' ' && direcao == -1)
    {
        mapa_busca_porta(*mapa, mapa->porta, &local_porta);
        *posicaoAtual = mapa->porta;
        mapa->matriz[local_porta.linha][local_porta.coluna] = 'D';
    }

    // SE JOGADOR ABRIU UM BAÚ
    else if(mapa->bau == 1 && direcao == -1)
    {
        abre_bau(mapa);
    }

    else if(*posicaoSeguinte != 'X')
    {
        // MOVIMENTAÇÃO SE JOGADOR ESTÁ SOBRE UMA ESCADA
        if(mapa->escada)
        {
            // SE POSIÇÃO PARA ONDE JOGADOR DESEJA SE MOVER ESTÁ COM A SAÍDA
            if(*posicaoSeguinte == 'P')
            {
                mapa->nivel++;
            }
            // SE POSIÇÃO PARA ONDE JOGADOR DESEJA SE MOVER NÃO ESTÁ COM UMA ESCADA
            else if(*posicaoSeguinte != 'H')
            {
                // SE POSIÇÃO PARA ONDE JOGADOR DESEJA SE MOVER ESTÁ COM UM BAÚ
                if(*posicaoSeguinte == 'C')
                {
                    mapa->bau = 1;
                }

                // SE POSIÇÃO PARA ONDE JOGADOR DESEJA SE MOVER ESTÁ COM UMA PORTA
                else if(*posicaoSeguinte != ' ')
                {
                    mapa->porta = *posicaoSeguinte;
                }
                mapa->escada = 0;
            }
            *posicaoAtual = 'H';
            *posicaoSeguinte = 'D';
        }
        // MOVIMENTAÇÃO SE O JOGADOR ESTÁ ACIMA DE UMA ESCADA E JOGADOR DESCEU
        else if(direcao == 1 && *posicaoSeguinte == 'H')
        {
            if(mapa->bau)
            {
                *posicaoAtual = 'C';
                mapa->bau = 0;
            }
            else if(mapa->porta != ' ')
            {
                *posicaoAtual = mapa->porta;
                mapa->porta = ' ';
            }
            else
            {
                *posicaoAtual = ' ';
            }
            *posicaoSeguinte = 'D';
            mapa->escada = 1;
        }
    }
}

/*
    Mapa *mapa  - Endereço de uma variável do tipo Mapa (função atualiza o mapa para movimentos horizontais válidos do jogador)
    int direcao - Inteiro que representa a direcao para qual o usuário deseja movimentar o jogador (-1 para esquerda, 1 para direita)
*/
void movimenta_horizontal(Mapa *mapa, int direcao)
{
    char* posicaoAtual = &(mapa->matriz[mapa->player.posicaoJ.linha][mapa->player.posicaoJ.coluna]);
    char* posicaoSeguinte = &(mapa->matriz[mapa->player.posicaoJ.linha][mapa->player.posicaoJ.coluna + direcao]);

    // SE JOGADOR ESTÁ TENTANDO SE DESLOCAR PARA UMA CASA VÁLIDA
    if(*posicaoSeguinte != 'X')
    {
        // SE JOGADOR ESTÁ SOBRE UMA ESCADA
        if(mapa->escada == 1)
        {
            *posicaoAtual = 'H';
            mapa->escada = 0;
        }
        // SE JOGADOR ESTÁ SOBRE UM BAÚ
        else if(mapa->bau == 1)
        {
            *posicaoAtual = 'C';
            mapa->bau = 0;
        }
        // SE JOGADOR ESTÁ SOBRE UMA PORTA
        else if(mapa->porta != ' ')
        {
            *posicaoAtual = mapa->porta;
            mapa->porta = ' ';
        }
        // SE JOGADOR ESTÁ SOBRE VAZIO
        else
        {
            *posicaoAtual = ' ';
        }

        // SE POSIÇÃO PARA ONDE JOGADOR DESEJA SE MOVER É A SAÍDA
        if(*posicaoSeguinte == 'P')
        {
            mapa->nivel++;
        }
        // SE POSIÇÃO PARA QUAL JOGADOR DESEJA SE MOVER ESTÁ COM UMA ESCADA
        else if(*posicaoSeguinte == 'H')
        {
            mapa->escada = 1;
        }
        // SE POSIÇÃO PARA QUAL JOGADOR DESEJA SE MOVER ESTÁ COM UM BAÚ
        else if(*posicaoSeguinte == 'C')
        {
            mapa->bau = 1;
        }
        // SE POSIÇÃO PARA QUAL JOGADOR DESEJA SE MOVER ESTÁ COM UMA PORTA
        else if(*posicaoSeguinte != ' ')
        {
            mapa->porta = *posicaoSeguinte;
        }

        *posicaoSeguinte = 'D';
    }
}
