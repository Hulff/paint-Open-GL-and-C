#ifndef OPERATIONS_H
#define OPERATIONS_H

void translate(float (*points)[3], int num_points, float xt, float yt);
void rotate(float (*points)[3], int num_points, double angle, float xt, float yt);
void escala(float (*points)[3],float (*originalPoints)[3], int num_points,float xf,float yf,float sx,float sy);
void cisalhamento_h(float (*points)[3], float (*originalPoints)[3], int num_points, float x, float y, float shx);
void cisalhamento_v(float (*points)[3], float (*originalPoints)[3], int num_points, float x, float y, float shy);


#endif
