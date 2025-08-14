// Operações com a pilha de figuras

#include <stdio.h>
#include <stdlib.h>
#include "shape.h"
#include "storage.h"

// Cria pilha de tamanho fixo
ShapeStack *criarPilha(int tamanho)
{
    ShapeStack *pilha = malloc(sizeof(ShapeStack));
    pilha->items = malloc(sizeof(Shape *) * tamanho);
    pilha->top = -1;
    pilha->capacity = tamanho;
    return pilha;
}

// Adiciona figura (push)
void adicionarFigura(ShapeStack *pilha, Shape *figura)
{
    if (pilha->top < pilha->capacity - 1)
    {
        pilha->items[++pilha->top] = figura;
    }
    else
    {
        printf("Erro: pilha cheia! Não é possível adicionar a figura.\n");
    }
}

// Remove figura (pop)
Shape *removerFigura(ShapeStack *pilha)
{
    if (pilha->top == -1)
    {
        printf("Erro: pilha vazia! Nada para remover.\n");
        return NULL;
    }
    return pilha->items[pilha->top--];
}

Shape *buscarFigura(ShapeStack *pilha, int id)
{
    for (int i = 0; i <= pilha->top; i++)
    {
        if (pilha->items[i]->id == id)
        {
            return pilha->items[i];
        }
    }
    return NULL;
}