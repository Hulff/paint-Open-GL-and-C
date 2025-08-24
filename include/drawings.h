// drawings.h
#ifndef DRAWINGS_H
#define DRAWINGS_H

#include "shape.h"
#include "storage.h"
#include "config.h"

void salvarPilhaComTimestamp(ShapeStack *pilha);
void carregarPilhaPorIndice(ShapeStack *pilha, char filenames[maxFiles][256], int indice);
int listarDesenhos(char filenames[maxFiles][256]);
#endif
