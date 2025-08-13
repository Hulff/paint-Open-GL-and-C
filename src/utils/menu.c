// Gera a UI de inicio

#include <stdio.h>
#include <stdlib.h>

// Gera a UI
void startUI(int *option)
{
    int temp;
    system("clear"); // Linux / macOS
    printf("Seja bem-vindo ao Paint 2025 atualizado Premium!\n");
    printf("############################################\n");
    printf("Pressione '1' iniciar o modo de desenho livre\n");
    printf("Pressione '2' para carregar algum desenho salvo\n");
    printf("Pressione '3' para sair.\n");
    printf("Pressione '4' para as dicas de atalhos.\n");
    printf("############################################\n");
    while (1)
    {
        printf("Escolha uma opção: ");
        if (scanf("%d", &temp) == 1 && temp >= 1 && temp <= 4) // scanf("%d", &temp) retorna 1 se a entrada foi válida
        {
            *option = temp;
            break;
        }
        else
        {
            printf("Opção inválida. Tente novamente.\n");
            while (getchar() != '\n')
                ;
        }
    }
}
void keyBindsUI()
{
    char temp;
    system("clear"); // Linux / macOS
    printf("Controles dentro do programa:\n");
    printf("############################################\n");
    printf("Pressione '' para criar um ponto.\n");
    printf("Pressione '' para criar um segmento de reta.\n");
    printf("Pressione '' para criar um quadrado.\n");
    printf("Pressione '' para criar um triangulo.\n");
    printf("Pressione '' para utilizar a criação livre.\n");
    printf("Pressione '' para apagar a última forma guardada na memoria.\n"); // tentar implementar
    printf("############################################\n");
    printf("Pressione 'q' voltar ao inicio.\n");
    printf("############################################\n");

    while (1)
    {
        printf("Escolha uma opção: ");
        if (scanf(" %c", &temp) == 1 && temp == 'q')
        {
            break;
        }
        else
        {
            printf("Opção inválida. Tente novamente.\n");
            while (getchar() != '\n')
                ;
        }
    }
};