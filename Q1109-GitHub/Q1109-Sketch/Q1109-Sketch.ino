#include <LiquidCrystal_I2C.h>

#define pinBotoes 27  //A0   //Arduino=Qualquer porta Analógica   ESP=Qualquer porta exceto 0,1,2,3,12,13 e 15

#define TENSAO 3.3           //5=Arduino      3.3=ESP32
#define VALOR_MAXIMO 4095  //1023=Arduino   4095=ESP32

LiquidCrystal_I2C lcd(0x3F, 16, 2);   //SDA=A4  SCL=A5  - Arduino UNO
                                      //SDA=21  SCL=22  - ESP32

//Faixas para Arduino (5V)
/*
int faixas[6][2] = { { 903, 1023 },
                     { 731, 901 },
                     { 585, 729 },
                     { 415, 583 },
                     { 235, 413 },
                     {  78, 233 } };
*/
//Faixas para ESP32 (3,3V)
int faixas[6][2] = { { 3518, 4095 },
                     { 2637, 3506 },
                     { 2072, 2625 }, 
                     { 1433, 2060 }, 
                     {  757, 1421 }, 
                     {  230,  745 } };

int botao = 0;
int botaoAnt = -1;

unsigned long delayDebounce;
unsigned long delayLimpaTela;

void setup() {
  lcd.init();
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Sketch Iniciado.");
}

void loop() {
  //Leitura da Porta
  int leitura = analogRead(pinBotoes);
  lcd.setCursor(10, 1);
  lcd.print("V=");
  lcd.print((float(leitura) / VALOR_MAXIMO) * TENSAO);
  lcd.print("      ");

  //Processa a Leitura dos Botoes
  if ((millis() - delayDebounce) > 200) {
    botao = 0;
    for (int nL = 0; nL < 6; nL++) {
      if ((leitura >= faixas[nL][0]) && (leitura <= faixas[nL][1])) {
        botao = nL + 1;
        delayDebounce = millis();
      }
    }
  }

  //Mostra o botao apertado
  if ((botao != 0) && (botao != botaoAnt)) {
    lcd.setCursor(0, 0);
    lcd.print("Botao: ");
    lcd.print(botao);
    delayLimpaTela = millis();
  }

  //Limpa a tela após botão apertado
  if ((millis() - delayLimpaTela) > 2000) {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    delayLimpaTela = millis();
  }

  botaoAnt = botao;
}