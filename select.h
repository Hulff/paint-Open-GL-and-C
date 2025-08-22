#ifndef SELECT_H
#define SELECT_H

#include "shape.h"

typedef enum {
    ACTION_NONE,
    ACTION_TRANSLATE,
    ACTION_ROTATE,
    ACTION_SHEAR,
    ACTION_REFLECT,
    ACTION_DELETE
} ActionType;

typedef struct {
    Shape *selected;
    int index;
    int active;         // 1 = modo sele��o ativo
    ActionType action;  // �ltima a��o escolhida
} Selector;

void initSelector(Selector *sel);
void setSelectionMode(Selector *sel, int active);
void selectShape(Selector *sel, Shape **shapes, int shapeCount, float x, float y);
void deleteSelected(Selector *sel, Shape **shapes, int *shapeCount);
void drawSelection(const Selector *sel);

#endif
