#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include "storage.h"

void startUI(int *option);
void keyBindsUI();
void drawingsUI(ShapeStack *pilha);
void programUI();

#endif
