#ifndef SELECT_H
#define SELECT_H

#include "shape.h"

typedef enum
{
    ACTION_NONE,
    ACTION_TRANSLATE,
    ACTION_ROTATE,
    ACTION_SHEAR,
    ACTION_REFLECT,
    ACTION_DELETE
} ActionType;

typedef struct
{
    Shape *selected;
    int index;
    int active;        // 1 = modo seleção ativo
    ActionType action; // última ação escolhida
} Selector;

void initSelector(Selector *sel,int maxPos);
void setSelectionMode(Selector *sel, int active);
void selectShape(Selector *sel, Shape **shapes, int shapeCount, float x, float y);
void deleteSelected(Selector *sel, Shape **shapes, int *shapeCount);
void drawSelection(const Selector *sel);

#endif