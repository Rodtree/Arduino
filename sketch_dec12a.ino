#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS D4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
const int TOTAL_LINEAS = 2;
const int TOTAL_COLUMNAS = 16;
const int PIN_SDA = 4;
const int PIN_SCL = 5;
LiquidCrystal_I2C lcd_i2c(0x27, TOTAL_COLUMNAS, TOTAL_LINEAS);

byte gradoChar[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

float ultimaTemperatura = -127.0;

void mostrarTexto(int columna, int linea, String texto) {
  lcd_i2c.setCursor(columna, linea);
  lcd_i2c.print("                ");
  lcd_i2c.setCursor(columna, linea);
  lcd_i2c.print(texto);
}

void mostrarProgreso() {
  lcd_i2c.setCursor(0, 0);
  lcd_i2c.print("Iniciando.   ");
  delay(500);

  lcd_i2c.setCursor(0, 0);
  lcd_i2c.print("Iniciando..  ");
  delay(500);

  lcd_i2c.setCursor(0, 0);
  lcd_i2c.print("Iniciando... ");
  delay(500);

  mostrarTexto(0, 0, "Inicio correcto");
  mostrarTexto(0, 1, "Tealera presenta");
  delay(1500);

  mostrarTexto(0, 0, "");
  mostrarTexto(0, 1, "");
}

void setup() {
  Serial.begin(9600);
  Wire.begin(PIN_SDA, PIN_SCL);
  lcd_i2c.begin(TOTAL_COLUMNAS, TOTAL_LINEAS);
  lcd_i2c.backlight();
  lcd_i2c.createChar(0, gradoChar);
  mostrarProgreso();
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();
  float temperaturaC = sensors.getTempCByIndex(0);

  if (temperaturaC != DEVICE_DISCONNECTED_C) {
    if (abs(temperaturaC - ultimaTemperatura) >= 0.1) { 
      ultimaTemperatura = temperaturaC;

      String linea1 = "Temperatura";
      int columna1 = (TOTAL_COLUMNAS - linea1.length()) / 2; 
      mostrarTexto(columna1, 0, linea1);

      lcd_i2c.setCursor(4, 1);
      lcd_i2c.print(String(temperaturaC, 1));
      lcd_i2c.write(byte(0));
      lcd_i2c.print("C");
    }
  } else {
    mostrarTexto(0, 0, "Error sensor");
    mostrarTexto(0, 1, "Revisar sistema");
  }

  delay(1000);
}
