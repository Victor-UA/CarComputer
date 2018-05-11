/*
 Name:		CarComputer.ino
 Created:	10.05.2018 20:57:41
 Author:	Victor-UA, Victor_UA@Ukr.net
*/

#include "LCDModes.h"
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

#pragma region Hall
#define Hall_Sensor 2
#pragma endregion
#pragma region LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Устанавливаем дисплей
#define LCD_Row_Length 16
bool _lcd_BackLight = false;
char fullFillRow[LCD_Row_Length];

LCDModes _lcdCurrentMode = LCDModes::Mode_Tachometer;
LCDModes _lcdLastMode = _lcdCurrentMode;
Thread lcdThread = Thread();
#define LCDRefreshInterval 1000
#pragma endregion
#pragma region Nokia
Adafruit_PCD8544 nokiaDisplay = Adafruit_PCD8544(3, 4, 5, 6, 7);
Thread nokiaThread = Thread();
#define NokiaRefreshInterval 100
#pragma endregion
#pragma region Tachometer
Thread tachometerThread = Thread();
#define TachometerRefreshInterval 100
unsigned int tachometerCurrentSpeed = 0;
#pragma endregion

  //Setup
void setup() 
{	
	Serial.begin(9600);
	#pragma region Hall Sensor
		pinMode(Hall_Sensor, INPUT);
		attachInterrupt(digitalPinToInterrupt(Hall_Sensor), HallSensorHandler, RISING);
	#pragma endregion
	#pragma region LCD
		lcd.init();
		LCD_BackLight(true);	
		for (size_t i = 0; i < LCD_Row_Length; i++)
		{
			fullFillRow[i] = ' ';
		}
		lcdThread.onRun(LCD_Handler);
		lcdThread.setInterval(LCDRefreshInterval);
	#pragma endregion
	#pragma region Nokia Display
		nokiaDisplay.begin();	
		nokiaDisplay.clearDisplay();
		nokiaDisplay.display();
		nokiaDisplay.setContrast(50); // установка контраста
		nokiaDisplay.setTextSize(1);  // установка размера шрифта
		nokiaDisplay.setTextColor(BLACK); // установка цвета текста
		nokiaDisplay.setCursor(0, 0); // установка позиции курсора	
		nokiaThread.onRun(Nokia_Handler);
		nokiaThread.setInterval(NokiaRefreshInterval);
	#pragma endregion
	#pragma region Tachometer
		tachometerThread.onRun(TachometerHandler);
		tachometerThread.setInterval(TachometerRefreshInterval);
	#pragma endregion
}

 //the loop function runs over and over again until power down or reset
void loop() 
{
	if (tachometerThread.shouldRun())
	{
		tachometerThread.run();
	}		
}


#pragma region LCD
void LCD_Handler()
{
	switch (_lcdCurrentMode)
	{
		case Mode_Tachometer:
			LCD_Mode_Tachometer();
			break;
		case Mode_Temperature_FrontInterior:
			LCD_Mode_Temperature_FrontInterior_Handler();
			break;
		default:
			break;
	}
}
void LCD_Mode_Tachometer()
{
	if (_lcdLastMode != _lcdCurrentMode)
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Tachometer, rpm");
	}
	String toPrint = (String)tachometerCurrentSpeed;
	size_t cursor = LCD_Row_Length - toPrint.length();

	char toPrintArr[LCD_Row_Length];
	for (size_t i = 0; i < LCD_Row_Length; i++)
	{
		toPrintArr[i] = i < cursor ? fullFillRow[i] : toPrint[i - cursor];
	}

	lcd.setCursor(0, 1);
	lcd.print(toPrintArr);
}
void LCD_Mode_Temperature_FrontInterior_Handler() 
{
	if (_lcdLastMode != _lcdCurrentMode)
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Temperature FI");
	}
}
void LCD_BackLight(bool On) {
	if (On && !_lcd_BackLight)
	{
		lcd.backlight();
		_lcd_BackLight = true;
	}
	if (!On && _lcd_BackLight)
	{
		lcd.noBacklight();
		_lcd_BackLight = false;
	}
}
#pragma endregion

#pragma region Nokia
void Nokia_Handler()
{
	nokiaDisplay.clearDisplay();
	nokiaDisplay.setCursor(0, 0);
	nokiaDisplay.println("Tachometr, RPM");
	nokiaDisplay.println(tachometerCurrentSpeed);
	nokiaDisplay.display();
}
#pragma endregion

#pragma region Tachometer
void HallSensorHandler() {
	tachometer.HallSensorHandler();
}
void TachometerHandler() 
{
	tachometerCurrentSpeed = tachometer.getRPM();	
}
#pragma endregion





