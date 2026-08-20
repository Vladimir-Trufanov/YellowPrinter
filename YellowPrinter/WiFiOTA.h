/** YellowPrinter-Esp32-Arduino                               *** WiFiOTA.h ***
 * 
 * Обнаружить нажатие на сенсорной панели и зафиксировать данные нажатия
 * 
 * v1.0.1, 12.08.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 12.08.2026
**/

#pragma once

/*

Шаг 2. Напишите скетч с поддержкой OTA
Вам понадобится библиотека ArduinoOTA. В коде нужно:
Подключить нужные заголовочные файлы: <WiFi.h>, <ArduinoOTA.h>.
Заполнить переменные ssid и password с данными вашей Wi-Fi-сети.
В функции setup() подключить Wi-Fi и вызвать ArduinoOTA.begin(), чтобы активировать OTA.
В loop() добавить ArduinoOTA.handle(), чтобы плата постоянно проверяла наличие обновлений.
Можно добавить обработчики событий (onStart, onEnd, onProgress, onError), чтобы видеть прогресс и ошибки в Мониторе порта.
 
github.com
programmingelectronics.com
Пример скеча:
#include <WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "Ваша_SSID";
const char* password = "Ваш_Пароль";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  ArduinoOTA.onStart(() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Начинается обновление: " + type);
  }); 

  ArduinoOTA.onEnd(() {
    Serial.println("\пОбновление завершено");
  }); 

  ArduinoOTA.onProgress((unsigned int progress, unsigned int total) {
    Serial.printf("Прогресс: %u%%\r", (progress * 100) / total);
  }); 

  ArduinoOTA.onError((ota_error_t error) {
    Serial.printf("Ошибка [%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Ошибка аутентификации");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Ошибка начала");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Ошибка подключения");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Ошибка приёма");
    else if (error == OTA_END_ERROR) Serial.println("Ошибка завершения");
  }); 

  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP-адрес: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{
  ArduinoOTA.handle();
}

Важное правило: каждое обновление, которое вы загружаете по воздуху, должно содержать этот код с OTA-логикой. Если в новом скетче нет вызова ArduinoOTA.begin(), устройство перестанет реагировать на OTA-запросы. Поэтому для первого запуска всегда загружайте скетч с OTA-логикой по USB, а уже потом обновляйте «по воздуху». 
programmingelectronics.com
github.com
v3trov.pro
Шаг 3. Первая загрузка (по USB)
Подключите ESP32 к компьютеру через USB-кабель, выберите в Arduino IDE правильную плату и порт, загрузите скетч. В Мониторе порта вы увидите IP-адрес устройства. 
github.com
programmingboss.com
Шаг 4. Настройка разделов памяти
Для работы OTA в ESP32 нужно, чтобы в таблице разделов были два специальных раздела (например, ota_0 и ota_1). В Arduino IDE это настраивается так: Инструменты → Размер flash-памяти → Выбрать схему разделов. Ищите вариант, где есть поддержка OTA (например, Minimal SPIFFS или Default 4MB with spiffs). Если выбрать схему без OTA, обновление не сработает. 
lesson.iarduino.ru
kotyara12.ru
github.com
Шаг 5. Загрузка обновлений «по воздуху»
Теперь можно обновлять прошивку без провода:
В Arduino IDE выберите в Инструменты → Порт сетевой порт с IP-адресом ESP32 (он появится после первой загрузки). 
github.com
programmingboss.com
Внесите изменения в скетч (добавьте логику, исправьте ошибки). 

*/

/*
Какой тип возвращает WiFi.localIP()
-----------------------------------
Функция WiFi.localIP() в Arduino (в частности, для модулей ESP8266 и ESP32) возвращает значение типа IPAddress. 
Этот тип определён в библиотеке WiFi.h. Сам метод localIP() предназначен для получения локального IP-адреса интерфейса Wi-Fi-платы (станции). 

Как работать с результатом
--------------------------
Чтобы вывести полученный IP-адрес в последовательный порт, можно напрямую передать переменную типа IPAddress в Serial.println(): 

if (WiFi.status() == WL_CONNECTED) 
{
  Serial.println(WiFi.localIP());
}

Для других задач с этим типом доступны и другие методы, например subnetMask(), gatewayIP() и dnsIP(). 
arduino-esp8266.readthedocs.io +1

Как проверить правильность возврата IP-адреса?
----------------------------------------------

Основной способ: проверка статуса подключения
Перед вызовом WiFi.localIP() обязательно убедитесь, что устройство подключено к сети — используйте WiFi.status():

#include <WiFi.h>
const char* ssid = "Your_SSID";
const char* password = "Your_Password";

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Ждём подключения
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Теперь IP гарантированно корректен
}

void loop() {}

Дополнительные проверки
-----------------------

1. Проверка на «некорректный» IP
Некоторые версии библиотек могут возвращать 255.255.255.255 при ошибке. Добавьте проверку:

IPAddress ip = WiFi.localIP();
if (ip[0] == 255 && ip[1] == 255 && ip[2] == 255 && ip[3] == 255) 
{
  Serial.println("Error: Invalid IP address received");
} 
else 
{
  Serial.println("Valid IP: " + ip.toString());
}

2. Проверка диапазона локальных IP
Убедитесь, что IP принадлежит стандартным локальным диапазонам:

bool isValidLocalIP(IPAddress ip) 
{
  // 192.168.x.x
  if (ip[0] == 192 && ip[1] == 168) return true;
  // 10.x.x.x
  if (ip[0] == 10) return true;
  // 172.16.x.x – 172.31.x.x
  if (ip[0] == 172 && ip[1] >= 16 && ip[1] <= 31) return true;
  return false;
}

// Использование:
IPAddress ip = WiFi.localIP();
if (isValidLocalIP(ip)) 
{
  Serial.println("Local IP is valid: " + ip.toString());
} 
else 
{
  Serial.println("IP is not in local range: " + ip.toString());
}

3. Проверка через строковое представление
Преобразуйте IP в строку и проверьте формат:

String ipStr = WiFi.localIP().toString();
if (ipStr != "0.0.0.0" && ipStr != "255.255.255.255") 
{
  Serial.println("Valid IP string: " + ipStr);
} 
else 
{
  Serial.println("Invalid IP string: " + ipStr);
}

4. Комплексная функция проверки
Объедините все проверки в одну функцию:

bool checkIP() 
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("WiFi not connected");
    return false;
  }
  IPAddress ip = WiFi.localIP();
  // Проверка на 0.0.0.0 (не назначен)
  if (ip == IPAddress(0, 0, 0, 0)) 
  {
    Serial.println("IP not assigned yet");
    return false;
  }
  // Проверка на 255.255.255.255 (ошибка)
  if (ip == IPAddress(255, 255, 255, 255)) 
  {
    Serial.println("Invalid IP received");
    return false;
  }
  // Проверка диапазона
  if (!isValidLocalIP(ip)) 
  {
    Serial.println("IP not in local network range");
    return false;
  }
  Serial.println("IP validated: " + ip.toString());
  return true;
}

Практические советы
-------------------
Используйте delay() после подключения — иногда IP назначается не сразу:
WiFi.begin(ssid, password);
delay(5000); // Ждём 5 секунд
if (WiFi.status() == WL_CONNECTED) 
{
  Serial.println(WiFi.localIP());
}
Проверяйте настройки роутера — убедитесь, что DHCP включён и есть свободные адреса.
Добавьте отладочные сообщения:

Serial.print("SSID: "); Serial.println(WiFi.SSID());
Serial.print("Signal: "); Serial.println(WiFi.RSSI());
Serial.print("IP: "); Serial.println(WiFi.localIP());

Переподключение при ошибке — если IP некорректен, попробуйте переподключиться:
if (!checkIP()) 
{
  WiFi.disconnect();
  WiFi.reconnect();
}

Краткий итог
Чтобы гарантированно получить корректный IP:
  - Дождитесь статуса WL_CONNECTED.
  - Проверьте IP на значения 0.0.0.0 и 255.255.255.255.
  - Убедитесь, что адрес принадлежит локальной сети.
  - При ошибке — переподключитесь.
*/

// ************************************************************** WiFiOTA.h ***
