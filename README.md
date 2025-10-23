# Braço Mecânico OpenGL

Este projeto simula um braço mecânico com múltiplos segmentos e a capacidade de pegar e soltar objetos em um ambiente 3D. O código-fonte está contido em `main.cpp`.

## Pré-requisitos

Para compilar e executar este projeto, você precisará de:

1.  **Um Compilador C++:** O mais comum é o `g++` (parte do MinGW no Windows, ou padrão no Linux/macOS).
2.  **Bibliotecas GLUT:** A biblioteca `freeglut` é recomendada.
    * **Linux (Ubuntu/Debian):** `sudo apt-get install freeglut3-dev`
    * **Windows (MinGW):** Certifique-se de que os headers (`.h`) e ficheiros de biblioteca (`.a` ou `.lib`) do `freeglut` estão acessíveis pelo seu compilador.
    * **macOS (com Homebrew):** `brew install freeglut`

## Como Compilar e Executar (Linha de Comando)

A forma mais simples de compilar este projeto é através de um terminal:

1.  Abra um terminal na pasta onde o `main.cpp` está localizado.
2.  Execute o seguinte comando para compilar:

    ```bash
    g++ main.cpp -o main -lfreeglut -lopengl32 -lglu32 -g
    ```

    * `g++ main.cpp`: Compila o seu ficheiro principal.
    * `-o main`: Define o nome do ficheiro executável de saída.
    * `-lfreeglut -lopengl32 -lglu32`: Liga (linka) as bibliotecas necessárias.
    * `-g`: (Opcional) Adiciona informação de debug.

3.  Para executar o programa, digite o nome do executável:

    ```bash
    ./main.exe
    ```
    (No Windows, pode ser apenas `main.exe` ou `.\main.exe`)

## Controles do Programa

* **Rotacionar base:** `A` / `D`
* **Mover braço (ombro):** `W` / `S`
* **Mover antebraço (cotovelo):** `E` / `Q`
* **Abrir garra (Soltar):** `X`
* **Fechar garra (Pegar):** `Z`
* **Menu:** Clique com o botão direito.