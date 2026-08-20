/** YellowPrinter-Esp32-Arduino                               *** WiFiOTA.h ***
 * 
 * Обнаружить нажатие на сенсорной панели и зафиксировать данные нажатия
 * 
 * v1.0.1, 12.08.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 12.08.2026
**/

#pragma once

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
