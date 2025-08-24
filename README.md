
---
# 🎨 Paint em OpenGL (Trabalho de Computação Gráfica - UFCA)

Projeto desenvolvido para a disciplina de **Computação Gráfica** da **Universidade Federal do Cariri**, ministrada pela professora **Luana Batista da Cruz**.

O objetivo é criar uma aplicação em **C com OpenGL** que permita desenhar, manipular e animar objetos geométricos primários, simulando um "Paint" personalizado.

---
## windows comando puro
 gcc -Wall -Wextra -Iinclude (Get-ChildItem -Recurse -Filter *.c | ForEach-Object { $_.FullName }) -o paint.exe -lopengl32 -lglu32 -lfreeglut -mconsole -lm
## Como rodar   no windows  com cmake
make → compila o projeto.
make run → compila e executa.
make clean → apaga arquivos temporários para recompilar do zero.
## Como rodar no linux
make -f Makefile.linux → compila o projeto.
make -f Makefile.linux run → compila e executa.
make -f Makefile.linux clean → apaga arquivos temporários para recompilar do zero.


## 📋 Funcionalidades obrigatórias

### 1. Criação de Objetos Primários (5,0 pontos)

* [X] Criar **ponto**
* [X] Criar **segmento de reta**
* [X] Criar **polígono**
* [X] Permitir múltiplas criações de cada tipo

### 2. Seleção e Exclusão de Objetos

* [X] Selecionar objetos individualmente (ponto, segmento ou polígono)
* [X] Excluir objeto selecionado

### 3. Transformações Geométricas (com matrizes de transformação/compostas)

* [X] Arrastar e soltar (**translação**)
* [X] Rotacionar em relação ao centro do objeto

  * Ponto: em relação à origem
* [X] Escalar em relação ao centro do objeto

  * Não se aplica a pontos
* [ ] Reflexão
* [X] Cisalhamento

### 4. Interação com o Usuário

* [X] Implementar eventos com:

  * [X] Clique do mouse
  * [X] Botão de rolagem do mouse
  * [X] Teclado

---

## 📂 Funcionalidades adicionais (2,0 pontos)

* [ ] **Salvar** objetos geométricos em arquivo
* [ ] **Carregar** objetos geométricos de arquivo

---

## 🎬 Animação (1,0 ponto)

* [ ] Criar animação com objetos primários (tema livre)

---

## 📑 Apresentação

Na apresentação, será necessário:

* [ ] Explicar a **estrutura de dados** usada para armazenar e manipular os objetos e justificar a escolha
* [ ] Demonstrar o funcionamento (criação, seleção, transformação etc.)
* [ ] Apresentar o algoritmo de seleção de objetos

---

## 🛠 Tecnologias

* Linguagem **C**
* **OpenGL**
* Bibliotecas auxiliares: *\[definir após implementação]*

---

## 📅 Prazos

* **Entrega:** 24/08
* **Apresentações:** A partir de 26/08
  ⏳ Tempo de apresentação: **20 a 30 minutos**
---
## Estrutura (exemplo depois ajustar para ficar igual com o projeto)
paint-opengl/
│── src/
│   ├── main.c                 # Função principal (loop do programa, inicialização)
│   ├── shapes/                # Objetos geométricos
│   │   ├── point.c             # Funções para ponto
│   │   ├── line.c              # Funções para linha
│   │   └── polygon.c           # Funções para polígono
│   ├── utils/                 # Funções auxiliares
│   │   ├── matrix.c            # Operações com matrizes
│   │   ├── vector.c            # Operações com vetores
│   │   └── color.c             # Manipulação de cores
│
├── include/                   # Cabeçalhos (.h)
│   ├── draw.h
│   ├── events.h
│   ├── transform.h
│   ├── animation.h
│   ├── file_io.h
│   ├── point.h
│   ├── line.h
│   ├── polygon.h
│   ├── matrix.h
│   ├── vector.h
│   └── color.h
│
├── drawings/  # arquivos gerados pelo progama para salvar o desenho
│
├── assets/                    # Imagens, fontes ou outros recursos (se necessário)
│
├── Makefile                   # Compilação e execução
├── README.md                  # Descrição do projeto
└── .gitignore                 # Ignorar arquivos desnecessários
