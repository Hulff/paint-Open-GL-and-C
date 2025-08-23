// helper.h
#ifndef HELPER_H
#define HELPER_H

#include "shape.h"
#include "storage.h"
#include "select.h"
#include <stdlib.h>
#include <stdio.h>

#include <stdbool.h>

void calcRealCenter(Shape *s, float *cx, float *cy);
int calcRealNumPoints(Shape *s);
bool verifyAvailability(ShapeStack *storage, Selector *selector);

#endif
