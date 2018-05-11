/*
 Name:		CarComputer.ino
 Created:	10.05.2018 20:57:41
 Author:	Victor
*/

// the setup function runs once when you press reset or power the board

#include <ThreadController.h>
#include <Thread.h>
#include <StaticThreadController.h>
#include <gfxfont.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Termometer_DHT11.h"
#include "Tachometer.h"

#define Hall_Sensor 2

LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Устанавливаем дисплей
#define LCD_Row_Length 16
bool _display_BackLight = false;
char fullFillRow[LCD_Row_Length];

Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);

Thread tachometerThread = Thread();
#define TachometerRefreshInterval 500

void setup() {	
	Serial.begin(9600);

	pinMode(Hall_Sensor, INPUT);
	attachInterrupt(digitalPinToInterrupt(Hall_Sensor), HallSensorHandler, RISING);

	lcd.init();
	Display_BackLight(true);
	lcd.print("Tachometer, rpm");

	for (size_t i = 0; i < LCD_Row_Length; i++)
	{
		fullFillRow[i] = ' ';
	}

	// инициализация и очистка дисплея
	display.begin();	
	display.clearDisplay();
	display.display();

	display.setContrast(50); // установка контраста
	display.setTextSize(1);  // установка размера шрифта
	display.setTextColor(BLACK); // установка цвета текста
	display.setCursor(0, 0); // установка позиции курсора	

	tachometerThread.onRun(TachometerHandler);
	tachometerThread.setInterval(TachometerRefreshInterval);
}

 //the loop function runs over and over again until power down or reset
void loop() {
	if (tachometerThread.shouldRun())
	{
		tachometerThread.run(); 
	}
}

void HallSensorHandler() {
	Tachometer.HallSensorHandler();
}

void TachometerHandler() 
{
	int turnSpeed = Tachometer.getRPM();
	String toPrint = (String)turnSpeed;
	size_t cursor = LCD_Row_Length - toPrint.length();

	char toPrintArr[LCD_Row_Length];
	for (size_t i = 0; i < LCD_Row_Length; i++)
	{
		toPrintArr[i] = i < cursor ? fullFillRow[i] : toPrint[i - cursor];
	}

	lcd.setCursor(0, 1);
	lcd.print(toPrintArr);

	display.clearDisplay();
	display.setCursor(0, 0);
	display.println("Tachometr, RPM");
	display.println(turnSpeed);
	display.display();
}

void Display_BackLight(bool On) {
	if (On && !_display_BackLight)
	{
		lcd.backlight();
		_display_BackLight = true;
	}
	if (!On && _display_BackLight)
	{
		lcd.noBacklight();
		_display_BackLight = false;
	}
}
