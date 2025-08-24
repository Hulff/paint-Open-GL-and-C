#ifndef STACK_H
#define STACK_H

#include "shape.h"

typedef struct {
    Shape **items;
    int top;
    int capacity;
} ShapeStack;

ShapeStack *criarPilha(int tamanho);
void adicionarFigura(ShapeStack *pilha, Shape *figura);
Shape *removerFigura(ShapeStack *pilha);
Shape *buscarFigura(ShapeStack *pilha, int id);

#endif