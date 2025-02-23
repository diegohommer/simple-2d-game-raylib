#include <stdio.h>
#include <string.h>
#include "mapa.h"

void arq_salva_jogo(Mapa mapa)
{
    FILE *arquivo;
    int l, c;
    arquivo = fopen("jogo.dat", "w+");

    if(arquivo == NULL)
    {
        printf(" O arquivo nao pode ser aberto!\n");
    }else
    {
        // GUARDA A CONFIGURAÇÃO DO MAPA E JOGADOR ATUAL NO ARQUIVO
        fwrite(&mapa.escada, sizeof(int), 1, arquivo);
        fwrite(&mapa.bau, sizeof(int), 1, arquivo);
        fwrite(&mapa.numeroBaus, sizeof(int), 1, arquivo);
        for(l = 0; l < MAXBAUS; l++)
        {
            fwrite(&mapa.baus[l], sizeof(Bau), 1, arquivo);
        }
        fwrite(&mapa.saida, sizeof(int), 1, arquivo);
        fwrite(&mapa.nivel, sizeof(int), 1, arquivo);
        fwrite(&mapa.baus, sizeof(Bau) * MAXBAUS, 1, arquivo);
        fwrite(&mapa.porta, sizeof(char), 1, arquivo);
        fwrite(&mapa.dimensoes, sizeof(Localizacao), 1, arquivo);
        fwrite(&mapa.posicaoInicialJ, sizeof(Localizacao), 1, arquivo);
        fwrite(&mapa.posicaoSaida, sizeof(Localizacao), 1, arquivo);
        fwrite(&mapa.player, sizeof(Jogador), 1, arquivo);

        for(l = 0; l < mapa.dimensoes.linha; l++)
        {
            for(c = 0; c < mapa.dimensoes.coluna; c++)
            {
                fwrite(&mapa.matriz[l][c], sizeof(char), 1, arquivo);
            }
        }
        fclose(arquivo);
    }
}

void arq_recupera_jogo(Mapa *mapa)
{
    FILE *arquivo;
    int l, c;
    arquivo = fopen("jogo.dat", "r");

    if(arquivo == NULL)
    {
        printf(" O arquivo nao pode ser aberto!\n");
    }else
    {
        // CARREGA O MAPA SALVO NO ARQUIVO JOGO.MAP
        fread(&mapa->escada, sizeof(int), 1, arquivo);
        fread(&mapa->bau, sizeof(int), 1, arquivo);
        fread(&mapa->numeroBaus, sizeof(int), 1, arquivo);
        for(l = 0; l < MAXBAUS; l++)
        {
            fread(&mapa->baus[l], sizeof(Bau), 1, arquivo);
        }
        fread(&mapa->saida, sizeof(int), 1, arquivo);
        fread(&mapa->nivel, sizeof(int), 1, arquivo);
        fread(&mapa->baus, sizeof(Bau) * MAXBAUS, 1, arquivo);
        fread(&mapa->porta, sizeof(char), 1, arquivo);
        fread(&mapa->dimensoes, sizeof(Localizacao), 1, arquivo);
        fread(&mapa->posicaoInicialJ, sizeof(Localizacao), 1, arquivo);
        fread(&mapa->posicaoSaida, sizeof(Localizacao), 1, arquivo);
        fread(&mapa->player, sizeof(Jogador), 1, arquivo);
        for(l = 0; l < mapa->dimensoes.linha; l++)
            for(c = 0; c < mapa->dimensoes.coluna; c++)
                fread(&mapa->matriz[l][c], sizeof(char), 1, arquivo);
    }
}

void arq_carrega_mapa(Mapa *mapa, char *nomeArq)
{
    FILE *arquivo;
    int linhas = 0, colunas = 0;
    char c, stringao[256];

    int i;
    arquivo = fopen(nomeArq, "r");
    if(arquivo)
    {
        // CONTA NÚMERO DE COLUNAS DO MAPA
        do{
            c = fgetc(arquivo); // Pega caracteres 'X' até encontrar outro caracter
            if(c == 'X')        // Soma 1 a colunas a cada coluna que possuir 'X' como primeiro caractere
            {
                colunas++;
            }
        }while(c == 'X');
        rewind(arquivo);  // Rebobina cabeçote para início do arquivo

        // CONTA NÚMERO DE LINHAS DO MAPA
        while (fgets(stringao, sizeof(stringao), arquivo) != NULL) // fgets pega uma linha inteira do arquivo.txt
        {
            linhas++;
        }
        rewind(arquivo);  // Rebobina cabeçote para início do arquivo

        // SALVA DIMENSÕES DO MAPA DENTRO DA ESTRUTURA
        mapa->dimensoes.coluna = colunas;
        mapa->dimensoes.linha = linhas;

        // SALVA MAPA NA MATRIZ DA ESTRUTURA
        for(i = 0; i < linhas; i++)
        {
            fgets(stringao, colunas + 1, arquivo);                   // Pega uma string de tamanho colunas + 1 (1 linha do mapa)
            strcpy(mapa->matriz[i], stringao);                       // Copia essa string para a linha i da matriz
            fgets(stringao, sizeof(stringao) - colunas, arquivo);    // Pega o espaço vazio entre a primeira e segunda linha do mapa
        }
    }else
        printf(" ERRO AO LER O MAPA\n");
    fclose(arquivo);
}
