#ifndef COORDENADAS_B
#define COORDENADAS_B

/*  TIPO LOCALIZACAO CRIADO:

    int linha  - Representa a coordenada X da posição/localização do mapa
    int coluna - Representa a coordenada Y da posição/localização do mapa

*/

typedef struct
{
    int linha, coluna;
}Localizacao;

Localizacao localizacao_cria(int linha, int coluna);

#endif // COORDENADAS_H
