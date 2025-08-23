#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

#include "storage.h"
#include "shape.h"

#include "config.h"

// Salva a pilha em um arquivo com timestamp
void salvarPilhaComTimestamp(ShapeStack *pilha)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char filename[256];
    snprintf(filename, sizeof(filename),
             "drawings/desenho_%02d_%02d_%04d_%02d_%02d_%02d.txt", // registra o desenho com dia mes ano hora min seg
             tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
             tm.tm_hour, tm.tm_min, tm.tm_sec);

    FILE *f = fopen(filename, "w");
    if (!f)
    {
        perror("Erro ao criar arquivo de desenho");
        return;
    }

    for (int i = 0; i <= pilha->top; i++) // converte cada figura para texto no arquivo
    {
        Shape *s = pilha->items[i];
        fprintf(f, "SHAPE %d %d %.2f %.2f %.2f %d\n",
                s->id, s->type, s->r, s->g, s->b, s->num_points);

        for (int j = 0; j < s->num_points; j++)
            fprintf(f, "%.2f %.2f %.2f\n", s->points[j][0], s->points[j][1], s->points[j][2]);

        fprintf(f, "\n");
    }

    fclose(f);
    printf("Desenho salvo em: %s\n", filename);
}

// Carrega um arquivo baseado no índice da enumeração
void carregarPilhaPorIndice(ShapeStack *pilha, char filenames[maxFiles][256], int indice)
{
    if (indice < 1)
        return;

    FILE *f = fopen(filenames[indice - 1], "r");
    if (!f)
    {
        perror("Erro ao abrir arquivo");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), f))
    {
        int id, type, num_points;
        float r, g, b;

        // Só processa linhas que começam com "SHAPE"
        // gera as figuras e as adiciona na pilha

        if (sscanf(line, "SHAPE %d %d %f %f %f %d",
                   &id, &type, &r, &g, &b, &num_points) == 6)
        {
            Shape *s = createShape(num_points, (ShapeType)type);
            s->id = id;
            s->r = r;
            s->g = g;
            s->b = b;

            for (int j = 0; j < num_points; j++)
            {
                if (fscanf(f, "%f %f %f", &s->points[j][0], &s->points[j][1], &s->points[j][2]) != 3)
                {
                    printf("Erro ao ler ponto %d do shape %d\n", j, id);
                    break;
                }
            }

            adicionarFigura(pilha, s);
        }
    }

    fclose(f);
}

// Lista arquivos da pasta drawings e armazena seus nomes em filenames
int listarDesenhos(char filenames[maxFiles][256])
{
    DIR *d = opendir("drawings");
    if (!d)
        return 0;

    struct dirent *dir;
    int count = 0;
    while ((dir = readdir(d)) != NULL)
    {
        if (dir->d_type == DT_REG) // arquivo regular
        {
            const char *ext = strrchr(dir->d_name, '.'); // procura última ocorrência de '.'
            if (ext && strcmp(ext, ".txt") == 0)         // verifica se é ".txt"
            {
                snprintf(filenames[count], 256, "drawings/%s", dir->d_name);
                count++;
                if (count >= maxFiles)
                    break;
            }
        }
    }
    closedir(d);

    for (int i = 0; i < count; i++)
        printf("%d: %s\n", i + 1, filenames[i]); // lista os elementos de forma enumerada

    return count; // total de arquivos listados
}
