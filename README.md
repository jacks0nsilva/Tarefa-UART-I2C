# Atividade Unidade 4 Capítulo 6 - Comunicação Serial 🚀

Tarefa prática do programa de residência EmbarcaTech.
Este projeto demonstra a comunicação UART para controlar um display OLED (SSD1306) e uma matriz de LEDs 5x5. O projeto também inclui interrupções para lidar com a recepção de dados de forma eficiente.

## Visão Geral 🌐

O projeto é composto por:

- **Comunicação UART:** Recebe comandos e dados via UART.
- **Display OLED (SSD1306):** Exibe texto e informações recebidas via UART.
- **Matriz de LEDs 5x5:** Exibe números recebidos via UART.
- **Interrupções:** Gerenciam a recepção de dados UART de forma eficiente.

## Vídeo de demonstração 📹

[Vídeo de apresentação do projeto](https://drive.google.com/file/d/1p33Xp1W8i2tf9JfYoDewDHImfJSwaRyE/view?usp=drive_link)

## Hardware 🛠️

1. Placa de desenvolvimento BitDogLab
2. Display OLED SSD1306 (com comunicação I2C)
   - GPIO 14 (SDA)
   - GPIO 15 (SCL)
3. Matriz de LEDs 5x5 (controlada via PIO)
   - GPIO 7
4. LED RGB
   - GPIO 11 (verde)
   - GPIO 12 (azul)
   - GPIO 13 (vermelho)
5. Botões A e B

   - GPIO 5 (A)
   - GPIO 6 (B)

6. Resistores

7. Conexão UART (para comunicação com um computador ou outro dispositivo)

## Software 💻

- **SDK do Raspberry Pi Pico:** Utilizado para desenvolvimento.
- **Bibliotecas:**
  - `ssd1306.h` e `ssd1306.c`: Para controlar o display OLED.
  - `pio_config.h`: Configuração e controle da matriz de LEDs via PIO.
  - `hardware/i2c.h`: Para comunicação I2C com o display OLED.
  - `pico/stdlib.h`: Funções padrão do Pico.
  - `hardware/uart.h`: Para comunicação UART.
  - `hardware/pio.h`: Para configurar e usar o PIO.
- **Arquivo principal (`Tarefa-UART-I2C.c`):** Contém a lógica principal do programa.

## Detalhes do Projeto 🔍

### 1. Comunicação UART

- **Configuração:** A comunicação UART é configurada para receber dados de um dispositivo externo (por exemplo, um computador).
- **Recepção de Dados:** Dados são recebidos via UART e interpretados como comandos e dados para controlar o display OLED e a matriz de LEDs.

### 2. Display OLED (SSD1306)

- **Inicialização:** O display OLED é inicializado usando a função `ssd1306_init()`. Isso inclui a configuração da interface I2C, a definição da largura e altura do display, e a inicialização do buffer de memória.
- **Exibição de Texto:**
  - A função `ssd1306_draw_char()` desenha um caractere individual no display. Ela utiliza um array de fontes (`font[]`) para obter os dados de pixel para cada caractere.
  - A função `ssd1306_draw_string()` exibe uma string de texto no display, chamando `ssd1306_draw_char()` para cada caractere da string.
  - Quando um comando para exibir texto é recebido via UART, o texto é exibido no display usando essas funções.
- **Exibição de Números:** Números podem ser exibidos no display OLED usando a mesma abordagem de exibição de texto.

### 3. Matriz de LEDs 5x5

- **Configuração PIO:** A matriz de LEDs é controlada usando o Peripheral Input/Output (PIO) do Raspberry Pi Pico. O PIO permite a criação de máquinas de estado personalizadas para controlar periféricos.
- **Exibição de Números:**
  - Quando um comando para exibir um número na matriz de LEDs é recebido via UART, o programa converte o número em um padrão de LEDs.

### 4. Interrupções ⚡

- **Detecção do Evento:**  
   Quando o usuário interage com os botões, o hardware gera um evento que aciona a interrupção.

- **Chamado do Handler:**  
   O handler de interrupção é chamado para tratar o evento gerado pelo botão pressionado.

- **Identificação do Botão:**  
   O handler de interrupção identifica qual botão foi pressionado (A ou B) e executa a ação correspondente.

- **Processamento do Evento:**  
   Dependendo do botão que disparou a interrupção:

  - **Botão A:**
    - Altera o estado do LED RGB verde.
    - Exibe uma mensagem no display informando que o LED verde foi alterado.
  - **Botão B:**
    - Altera o estado do LED RGB azul.
    - Exibe uma mensagem no display informando que o LED azul foi alterado.

## Arquivos Importantes 📂

- `Tarefa-UART-I2C.c`: Arquivo principal, contendo a lógica do programa, incluindo a inicialização, o tratamento de interrupções UART e o controle do display OLED e da matriz de LEDs.
- `libs/include/ssd1306.h`: Definições e protótipos de funções para o display OLED.
- `libs/src/ssd1306.c`: Implementação das funções para o display OLED.
- `libs/include/pio_config.h`: Configuração e funções para controlar a matriz de LEDs via PIO.
- `libs/include/ws2812.pio`: Programa PIO para controlar os LEDs (geralmente incluído no projeto).
- `libs/include/font.h`: Dados da fonte para o display OLED.

## Como Executar o Projeto 🚀

1.  **Configuração do Ambiente:**
    - Certifique-se de ter o SDK do Raspberry Pi Pico instalado e configurado corretamente.
    - Configure as ferramentas de compilação (CMake, etc.).
2.  **Conexões de Hardware:**

    - Conecte o display OLED à placa Raspberry Pi Pico usando a interface I2C. Verifique os pinos SDA e SCL corretos.
    - Conecte a matriz de LEDs aos pinos GPIO configurados para o PIO.
    - Conecte a UART da placa Pico a um dispositivo externo (por exemplo, um computador) usando um conversor USB-UART.

3.  **Clone o repositório**
    ```
    git clone https://github.com/jacks0nsilva/Tarefa-UART-I2C
    ```
4.  **Instale a extensão do Raspberry Pi Pico no seu VsCode**
5.  **Usando a extensão do Raspberry Pi Pico, siga os passos:**
6.  **Clean CMake: Para garantir que o projeto será compilado do zero**

7.  **Compile Project: Compilação dos binários**

8.  **Run Project [USB]: Compila e copia o firmware para a placa automaticamente**
