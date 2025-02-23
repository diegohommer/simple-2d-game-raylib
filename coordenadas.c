#include "coordenadas.h"

/*
    int linha  - Inteiro que representa a coordenada X da posição/localização
    int coluna - Inteiro que representa a coordenada Y da posição/localização
 */
Localizacao localizacao_cria(int linha, int coluna)
{
    Localizacao posicao = {linha, coluna};
    return posicao;
}
