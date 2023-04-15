// библиотека для работы I²C
#include <Wire.h> 
// подключаем библиотеку LiquidCrystal_I2C - монитор
#include <LiquidCrystal_I2C.h>
 // библиотека для работы с часами реального времени
#include "TroykaRTC.h"

// подключаем библиотеку для bh1750 - датчик освещённости
#include <BH1750.h>

// библиотека для работы с протоколом 1-Wire
#include <OneWire.h>
// библиотека для работы с датчиком DS18B20 - датчик температуры
#include <DallasTemperature.h>
 
// сигнальный провод датчика
#define ONE_WIRE_BUS 10

// создаём объект для работы с библиотекой OneWire
OneWire oneWire(ONE_WIRE_BUS);
 
// создадим объект для работы с библиотекой DallasTemperature
DallasTemperature sensor(&oneWire);

// Объявляем объект lightMeter
BH1750 lightMeter;

// создаем объект-экран, передаём используемый адрес 
// и разрешение экрана:
LiquidCrystal_I2C lcd(0x38, 20, 4);

// размер массива для времени
#define LEN_TIME 12
// размер массива для даты
#define LEN_DATE 12
// размер массива для дня недели
#define LEN_DOW 12
 
// создаём объект для работы с часами реального времени
RTC clock;
 
// массив для хранения текущего времени
char time[LEN_TIME];
// массив для хранения текущей даты
char date[LEN_DATE];
// массив для хранения текущего дня недели
char weekDay[LEN_DOW];

// Укажем, что к каким пинам подключено
int trigPin = 10; 
int echoPin = 11; 

void setup() {
  Serial.begin (9600); 
  //pinMode(trigPin, OUTPUT); 
  //pinMode(echoPin, INPUT); 
  
  // инициализация часов
  clock.begin();
  // метод установки времени и даты в модуль вручную
  // clock.set(10,25,45,27,07,2005,THURSDAY);    
  // метод установки времени и даты автоматически при компиляции
  // clock.set(__TIMESTAMP__);
  // что бы время менялось при прошивки или сбросе питания
  // закоментируйте оба метода clock.set();
  
  // инициализируем экран
  lcd.init();
  // включаем подсветку
  lcd.backlight();

  // Инициализируем и запускаем BH1750
  lightMeter.begin();

  // начинаем работу с датчиком
  sensor.begin();
  // устанавливаем разрешение датчика от 9 до 12 бит
  sensor.setResolution(12);

  Serial.println(F("BH1750 тест"));  
}
 
void loop() {
  // запрашиваем данные с часов
  clock.read();
  // сохраняем текущее время, дату и день недели в переменные
  clock.getTimeStamp(time, date, weekDay);
  // выводим в serial порт текущее время, дату и день недели
  // Serial.print(time);
  // Serial.print("\t");
  // Serial.print(date);
  // Serial.print("\t");
  // Serial.println(weekDay);
  
  lcd.clear();  
  // устанавливаем курсор в колонку 0, строку 0
  lcd.setCursor(0, 0);
  // печатаем первую строку
  lcd.print(time);
  // устанавливаем курсор в колонку 0, строку 1
  // на самом деле это вторая строка, т.к. нумерация начинается с нуля
  lcd.setCursor(0, 1);
  // печатаем вторую строку
  lcd.print(date);
  // устанавливаем курсор в колонку 0, строку 2 
  //lcd.setCursor(0, 2);
  // печатаем третью строку
  //lcd.print(weekDay);
  // устанавливаем курсор в колонку 0, строку 3
  //lcd.setCursor(0, 3);
  // печатаем четвёртую строку
  //lcd.print("www.Amperka.ru");
  
  //int duration, distance;
  // для большей точности установим значение LOW на пине Trig
  //digitalWrite(trigPin, LOW); 
  //delayMicroseconds(2); 
  // Теперь установим высокий уровень на пине Trig
  //digitalWrite(trigPin, HIGH);
  // Подождем 10 μs 
  //delayMicroseconds(10); 
  //digitalWrite(trigPin, LOW); 
  // Узнаем длительность высокого сигнала на пине Echo
  //duration = pulseIn(echoPin, HIGH); 
  // Рассчитаем расстояние
  //distance = duration / 58;
  // Выведем значение в Serial Monitor
  // Serial.print(distance); 
  // Serial.println(" cm"); 
  //String dist(distance);
  //dist += " cm";
  //lcd.setCursor(0, 3);
  // печатаем четвёртую строку
  //lcd.print(dist);

 
 // Считываем показания с BH1750
  float lux = lightMeter.readLightLevel();
 // Отправляем значение освещенности в последовательный порт
 // Serial.print("Light: ");
 // Serial.print(lux);
 // Serial.println(" lx"); 
  
  String dist("Light: ");
  dist += lux;
  dist += " lx";
  lcd.setCursor(0, 3);
  //печатаем четвёртую строку
  lcd.print(dist);

  // переменная для хранения температуры
  float temperature;
  // отправляем запрос на измерение температуры
  sensor.requestTemperatures();
  // считываем данные из регистра датчика
  temperature = sensor.getTempCByIndex(0);
  // устанавливаем курсор в колонку 0, строку 2 
  lcd.setCursor(0, 2);
  // выводим температуру
  String temp("Temp C: ");
  temp += temperature;
  lcd.print(temperature);
  // ждём одну секунду
  delay(1000);
}