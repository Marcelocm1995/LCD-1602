# LCD-1602
Tutorial para utilização do controlador LCD HD44780 com mBed e Keil uVision

# Objetivo
Ensinar a importar a biblioteca em ambos compiladores (Keil uVision e mBed), e então escrever caracteres alfanuméricos no display de cristal líquido (LCD HD44780)

# Importação - mBed
Há duas maneiras de importar no mBed:
* Importando o código disponível no repositório do mBed neste link https://os.mbed.com/users/Marcelocostanzo/code/LCD_1602/ 
* Dentro de seu projeto, criar 2 arquivos com a mesma estrutura presente na pasta "mbed-> biblioteca" deste repositório

# Importação - Keil
Crie normalmente seu projeto no cubeMX seguindo a pinagem escolhida para conexão do display. No diretório do projeto vá até a pasta Inc e copie o arquivo LCD_1602.h disponivel na pasta "Keil-> biblioteca" deste repositório, faça o mesmo com o arquivo LCD_1602.c porem cole este na pasta Src do projeto.
Abra o projeto no Keil, na arvore do projeto clique direito com o mouse na pasta "Application/User" e então "Add Existing Files to Group Application/User", escolha o arquivo LCD_1602.c copiado anteriormente na pasta Src.

# Programando
Para utilizar a biblioteca devemos incluir ela em nosso arquivo principal, o main. Para isso devemos escrever #include "LCD_1602.h", com isso o código já estará apto a utilizar as funções da biblioteca. As funções são as seguintes:

```javascript
void lcd_init (void)
```
- Inicializa o display, enviando uma sequência de dados necessários 

```javascript
void lcd_send_cmd (char cmd)
```
- Envia comandos ao display, não usaremos essa função em nosso código principal, ela é utilizada internamente pela biblioteca

```javascript
void lcd_send_char (char data)
```
- Escreve um único caracter no display

```javascript
void lcd_send_string (char *str)
```
- Escreve uma string no display (ou um vetor de caracteres)  

```javascript
void lcd_put_cur(int row, int col)
```
- Seleciona a posição do cursor em linha, coluna

```javascript
void lcd_clear (void)  
```
- Limpa o display
    
```javascript
void lcd_cursor_mode(char cursor, char blink)
```
- Configura o modo do cursor. O primeiro parâmetro exibe ou não o cursor (0 sem cursor, 1 com cursor), o segundo parâmetro piscará ou não o cursor (0 não piscante, 1 piscante)
