#ifndef STORAGE_H
#define STORAGE_H
#include "shape.h"

typedef struct
{
    Shape **items; // array de ponteiros para Shape
    int top;       // índice do topo
    int capacity;  // capacidade máxima
} ShapeStack;

ShapeStack *criarPilha(int tamanho);
void adicionarFigura(ShapeStack *pilha, Shape *figura);
Shape *removerFigura(ShapeStack *pilha);
Shape *removerFiguraPorIndice(ShapeStack *pilha, int indice);
Shape *buscarFigura(ShapeStack *pilha, int id);

#endif
