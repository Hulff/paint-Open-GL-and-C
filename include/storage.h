#ifndef STORAGE_H
#define STORAGE_H

ShapeStack *criarPilha(int tamanho);
void adicionarFigura(ShapeStack *pilha, Shape *figura);
Shape *removerFigura(ShapeStack *pilha);
Shape *buscarFigura(ShapeStack *pilha, int id);

#endif
