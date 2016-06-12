/* Algoritmo de reprodução de Audio no arduino
  Desenvolvido por Felipe Ferreira.
  Componentes utilizadados: Arduino
                            Shield Ethernet = para utilizar o cartão SD
                            1 Led
                            2 Botões
                            Saida de Audio
                            2 Resistores de 10k
                            1 de 220k -> LED

*/

#include <SdFat.h>
#define SaidaAudio 6
const int chipSelect = 4;

SdFat sd;
SdFile ArquivoMusica;

const int ButtonStart = 2;
const int ButtonPause = 9;
const int Led = 8;
int buttonState = 0;
int buttonState2 = 0;

unsigned char data; // Declara uma variável para armazenar os dados do arquivo

void setup() {

  Serial.begin(9600);
  pinMode(SaidaAudio, OUTPUT); //Saida do Audio
  pinMode(ButtonStart, INPUT); //Entrada digital do botão Start.
  pinMode(ButtonPause, INPUT); //Entrada digital do botão Pause.
  pinMode(Led, OUTPUT); //Saida digital Led.
  TCCR0B = 0x01;     // Configura a frequência do PWM nos pinos 5 e 6 para 64KHz.
}

void loop() {
  buttonState = digitalRead(ButtonStart); // lê a entrada
  if (buttonState == HIGH) { // se o botao start for ligado toca o audio
    digitalWrite(Led, HIGH); // Liga o LED
    if (!sd.begin(SPI_FULL_SPEED, chipSelect)) sd.initErrorHalt();     // Inicia SdFat com máxima velocidade ou imprime mensagem de erro no "Serial Monitor".
    if (!ArquivoMusica.open("Savior4.wav", O_READ)) {     // Abre o arquivo chamado de audio
      sd.errorHalt("Erro ao abrir o codigo");
    }
    for (int count = 0; count < 42; count++) { // Pula o cabeçalho do arquivopara acessar os dados(depois de 129 leituras). Este número pode variar para cada arquivo.
      data = ArquivoMusica.read();     // Lê byte a byte até o final do cabeçalho.
    }
    while (data >= 0) {    // Se o dado a ser lido for maior ou igual a zero (não nulo),
      buttonState2 = digitalRead(ButtonPause);
      if (buttonState2 == HIGH) { // Da pause
        ArquivoMusica.close();
        break;
      }
      data = ArquivoMusica.read();     // Lê um byte do arquivo e armazena-o em "data".
      analogWrite(SaidaAudio, data);     // Envia os dados para o pino 6 (pino ligado ao alto falante).
      delayMicroseconds(40);     // Espera um certo intervalo (em microssegundos) para a próxima amostragem (de acordo com a do arquivo .wav).
    } // desliga o led
    buttonState = LOW; 
  }
  digitalWrite(Led, LOW);
}
