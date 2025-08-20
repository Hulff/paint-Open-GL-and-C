#ifndef OPERATIONS_H
#define OPERATIONS_H

void translate(float (*points)[3], int num_points, float xt, float yt);
void rotate(float (*points)[3], int num_points, double angle, float xt, float yt);

#endif
