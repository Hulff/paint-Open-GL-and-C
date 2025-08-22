#include "shape.h"
#include "storage.h"
#include "select.h"
#include <stdlib.h>
#include <stdio.h>

void calcRealCenter(Shape *s, float *cx, float *cy)
{
    int num_points = 0;
    float cx_local = 0, cy_local = 0;
    for (int i = 0; i < s->num_points; i++)
    {
        if (s->points[i][2] == 1)
        {
            cx_local += s->points[i][0];
            cy_local += s->points[i][1];
            num_points++;
        }
    }

    if (num_points > 0)
    {
        cx_local /= num_points;
        cy_local /= num_points;
    }
    else
    {
        cx_local = 0;
        cy_local = 0;
    }

    *cx = cx_local;
    *cy = cy_local;
}

bool verifyAvailability(ShapeStack *storage, Selector *selector)
{
    if (storage->top < 0)
    {
        printf("Nenhuma figura criada\n");
        return false;
    }
    if (selector->selected == NULL)
    {
        printf("Nenhuma figura selecionada\n");
        return false;
    }
    return true;
}
