/** YellowPrinter-Esp32-Arduino                               *** WiFiOTA.h ***
 * 
 * Выполнить запуск WiFi и обслужить работу OTA
 * 
 * v2.0.1, 26.08.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 12.08.2026
 * 
 * [Базовое OTA-обновление ESP32 «по воздуху» (OTA) в Arduino IDE]
 * (https://wiki.alashed.kz/esp32/esp32-ota-updates-arduino-ide/index.html)
**/

#pragma once

#include <WiFi.h>
#include <ArduinoOTA.h>

#include "inimem.h"
#include "yp_Trass.h"
#include "yp_ESPNOW.h"

// "OPPO A9 2020"; "TP-Link_B394"; "tve-DESKTOP"; "linksystve"; "linksystve";
// "b277a4ee84e8"; "18009217"    ; "Ue18-647"   ; "x93k6kq6wf"; "X93K6KQ6WF";
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

uint32_t last_ota_time = 0;

void iniWiFi(); 
void iniOTA(); 

void iniWiFi() 
{
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
}

void iniOTA() 
{
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // Password can be set with plain text (will be hashed internally)
  // The authentication uses PBKDF2-HMAC-SHA256 with 10,000 iterations
  // ArduinoOTA.setPassword("admin");

  // Or set password with pre-hashed value (SHA256 hash of "admin")
  // SHA256(admin) = 8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918
  // ArduinoOTA.setPasswordHash("8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918");

  ArduinoOTA
    .onStart([]() 
    {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) 
      {
        type = "sketch";
      } 
      else 
      {  
        // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() 
    {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) 
    {
      if (millis() - last_ota_time > 500) 
      {
        Serial.printf("Flash it: %u%%\n", (progress / (total / 100)));
        last_ota_time = millis();
      }
    })
    .onError([](ota_error_t error) 
    {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) 
      {
        Serial.println("Auth Failed");
      } 
      else if (error == OTA_BEGIN_ERROR) 
      {
        Serial.println("Begin Failed");
      } 
      else if (error == OTA_CONNECT_ERROR) 
      {
        Serial.println("Connect Failed");
      } 
      else if (error == OTA_RECEIVE_ERROR) 
      {
        Serial.println("Receive Failed");
      } 
      else if (error == OTA_END_ERROR) 
      {
        Serial.println("End Failed");
      }
    });

  ArduinoOTA.begin();

  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
  //ipStr = WiFi.localIP().toString();
}

/*
Пишем скетч с поддержкой BasicOTA
---------------------------------
Понадобится библиотека ArduinoOTA. В коде нужно:
Подключить нужные заголовочные файлы: <WiFi.h>, <ArduinoOTA.h>.
Заполнить переменные ssid и password с данными вашей Wi-Fi-сети.
В функции setup() подключить Wi-Fi и вызвать ArduinoOTA.begin(), чтобы активировать OTA.
В loop() добавить ArduinoOTA.handle(), чтобы плата постоянно проверяла наличие обновлений.
Можно добавить обработчики событий (onStart, onEnd, onProgress, onError), чтобы видеть прогресс и ошибки в Мониторе порта.
 
#include <WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "Ваша_SSID";
const char* password = "Ваш_Пароль";

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  ArduinoOTA.onStart(() 
  {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Начинается обновление: " + type);
  }); 

  ArduinoOTA.onEnd(() 
  {
    Serial.println("\пОбновление завершено");
  }); 

  ArduinoOTA.onProgress((unsigned int progress, unsigned int total) 
  {
    Serial.printf("Прогресс: %u%%\r", (progress * 100) / total);
  }); 

  ArduinoOTA.onError((ota_error_t error) 
  {
    Serial.printf("Ошибка [%u]: ", error);
    if (error      == OTA_AUTH_ERROR)    Serial.println("Ошибка аутентификации");
    else if (error == OTA_BEGIN_ERROR)   Serial.println("Ошибка начала");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Ошибка подключения");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Ошибка приёма");
    else if (error == OTA_END_ERROR)     Serial.println("Ошибка завершения");
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

Важное правило: каждое обновление, которое вы загружаете по воздуху, должно 
содержать этот код с OTA-логикой. Если в новом скетче нет вызова ArduinoOTA.begin(), 
устройство перестанет реагировать на OTA-запросы. Поэтому для первого запуска всегда 
загружайте скетч с OTA-логикой по USB, а уже потом обновляйте «по воздуху». 

Первая загрузка (по USB)
------------------------
Подключите ESP32 к компьютеру через USB-кабель, выберите в Arduino IDE правильную 
плату и порт, загрузите скетч. После запуска в мониторе порта вы увидите IP-адрес устройства. 

Настройка разделов памяти
-------------------------
Для работы OTA в ESP32 нужно, чтобы в таблице разделов были два специальных раздела 
(например, ota_0 и ota_1). В Arduino IDE это настраивается так: 
Инструменты → Размер flash-памяти → Выбрать схему разделов.

Ищите вариант, где есть поддержка OTA (например, Minimal SPIFFS или Default 4MB with spiffs). 
Если выбрать схему без OTA, обновление не сработает. 

Загрузка обновлений «по воздуху»
--------------------------------

Теперь можно обновлять прошивку без провода:
В Arduino IDE выберите в Инструменты → Порт сетевой порт с IP-адресом ESP32 
(он появится после первой загрузки). 

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


/*
----------- Почему функции esp_now_register_recv_cb(messagereceived) мешает ota
            -------------------------------------------------------------------
            
На самом деле сама по себе регистрация callback-функции esp_now_register_recv_cb(messageReceived) 
не блокирует OTA напрямую — проблема обычно кроется в том, как в вашем коде выстроены логика ESP-NOW
и процесс обновления прошивки.

Суть в том, что ESP-NOW и OTA решают разные задачи, и конфликт возникает из-за совместного использования ресурсов 
или порядка действий. ESP-NOW работает на уровне Wi-Fi. Когда вы регистрируете esp_now_register_recv_cb, 
вы настраиваете обработчик, который вызывается при получении сообщения по протоколу ESP-NOW. 

Этот модуль напрямую использует Wi-Fi (режим station, SoftAP), и если в коде одновременно 
активно используется Wi-Fi для других задач (например, для подключения к роутеру во время OTA), 
могут возникать конфликты. Например, во время загрузки прошивки через OTA модуль Wi-Fi может 
быть временно отключён или переконфигурирован, и тогда приём через ESP-NOW просто перестаёт срабатывать.

OTA и Wi-Fi — это две разные фазы. Процесс OTA обычно включает отключение Wi-Fi, 
запись нового кода во флеш-память и последующую перезагрузку. Если в этот момент активен код, 
который параллельно использует Wi-Fi (например, слушает ESP-NOW-сообщения), 
это нарушает атомарность операций и приводит к сбоям. 

Типичный сценарий: вы настраиваете ESP-NOW для обмена данными, а в loop() или в функции, 
которая запускает OTA, одновременно включаете Wi-Fi. В этот момент что-то идёт не так — 
приём через ESP-NOW может «зависнуть» или начать работать с ошибками. 

Как это решить?

Не обязательно убирать регистрацию callback-функции, но нужно грамотно выстроить последовательность действий:

Не включайте Wi-Fi и не запускайте OTA одновременно с активным приёмом ESP-NOW. Лучше разделить логику: 
например, сначала завершить приём ESP-NOW, затем отключить Wi-Fi и начать OTA. 

Можно использовать флаг (например, do_OTA), который устанавливается при получении специальной
команды по ESP-NOW. В функции setup() или в цикле loop() проверьте этот флаг: если он установлен — 
выполняем переход на OTA-режим, предварительно отключив Wi-Fi. 

Проверьте, как в вашей библиотеке для OTA (например, ArduinoOTA) взаимодействует с модулем Wi-Fi. 

Ещё один нюанс

Иногда путаница возникает из-за версий библиотек. В разных версиях API ESP-NOW (например, 2.x и 3.x) 
сигнатура функции обратного вызова могла измениться. Если вы мигрируете код с старой версии
на новую, это тоже может вызывать неожиданные сбои в работе. 

Мой совет: начните с того, чтобы вынести код, который активно использует ESP-NOW, в отдельную функцию или блок, 
который вызывается только тогда, когда точно не идёт процесс обновления или не активен Wi-Fi. 
Так вы сразу увидите, в этом ли дело.

Разберу пошагово, как правильно настроить ESP-NOW, чтобы избежать конфликтов (в т. ч. с OTA).

Пошаговая настройка ESP-NOW

Шаг 1. Инициализация WiFi и ESPNOW

Сначала запустите WiFi, затем инициализируйте ESPNOW:

#include <esp_now.h>
#include <WiFi.h>

void setup() 
{
  Serial.begin(115200);

  // Инициализируем WiFi (режим STA или AP)
  WiFi.mode(WIFI_STA);
  // Или WiFi.mode(WIFI_APSTA) для смешанного режима

  // Инициализируем ESPNOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESPNOW");
    return;
  }

  // Регистрируем callback для приёма данных
  esp_now_register_recv_cb(OnDataRecv);
}

Шаг 2. Добавление пиров (устройств)

Перед отправкой данных добавьте пиры в список:

esp_now_peer_info_t peerInfo;
// Заполняем информацию о пире
memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
peerInfo.channel = 0;      // 0 = текущий канал
peerInfo.encrypt = false;  // или true для шифрования
// Добавляем пир
if (esp_now_add_peer(&peerInfo) != ESP_OK) 
{
  Serial.println("Failed to add peer");
  return;
}

Шаг 3. Настройка callback-функций

// Callback для приёма данных:
void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) 
{
  // Обрабатываем данные
  Serial.print("Received data from: ");
  printMacAddress(mac);
  Serial.print("Data: ");
  for (int i = 0; i < len; i++) 
  {
    Serial.print((char)data[i]);
  }
  Serial.println();
}

// Callback для отправки (опционально):
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

Шаг 4. Отправка данных

esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *)&myData, sizeof(myData));
if (result == ESP_OK) 
{
  Serial.println("Send success");
} else {
  Serial.println("Send failed");
}

Ключевые правила для избежания конфликтов

Порядок инициализации:

сначала WiFi.mode();
затем esp_now_init().

Деинициализация перед OTA:

cpp
void startOTA() {
  // Останавливаем ESP‑NOW перед OTA
  esp_now_deinit();
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);

  // Запускаем OTA
  ArduinoOTA.begin();
}
Ограничение длины данных: отправляйте не более 250 байт за раз.

Интервалы между отправкой: не отправляйте данные слишком часто — дождитесь callback от предыдущей отправки.

Избегайте тяжёлых операций в callback‑функциях:

не используйте delay();
не выполняйте длительные вычисления;
передавайте данные в очередь для обработки в основном цикле.
Управление пирами:

добавляйте пиры через esp_now_add_peer();
удаляйте ненужные пиры через esp_now_del_peer().
Обработка ошибок: проверяйте возвращаемые значения функций ESP‑NOW.

Пример полной настройки
cpp
#include <esp_now.h>
#include <WiFi.h>

// MAC-адрес приёмника
uint8_t receiverMacAddress[] = {0x24, 0x6F, 0x28, 0xAB, 0xCD, 0xEF};

// Структура данных
typedef struct struct_message {
  int id;
  float temp;
  char text[32];
} struct_message;
struct_message myData;

void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  memcpy(&myData, data, sizeof(myData));
  Serial.printf("ID: %d, Temp: %.2f, Text: %s\n", myData.id, myData.temp, myData.text);
}

void setup() {
  Serial.begin(115200);

  // Инициализируем Wi‑Fi
  WiFi.mode(WIFI_STA);

  // Инициализируем ESP‑NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP‑NOW init failed");
    return;
  }

  // Регистрируем callback
  esp_now_register_recv_cb(OnDataRecv);

  // Добавляем пир
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Основной код
  delay(1000);
}
Дополнительные рекомендации
Шифрование: для защиты данных используйте esp_now_set_pmk() и esp_now_set_lmk().
Отладка: включайте логирование для отслеживания ошибок ESP‑NOW.
Обновление прошивки: перед OTA полностью деинициализируйте ESP‑NOW и Wi‑Fi.
Тестирование: проверяйте работу в разных режимах Wi‑Fi (STA, AP, AP+STA).
Ресурсы: помните, что ESP‑NOW использует ресурсы Wi‑Fi — не перегружайте систему параллельными задачами.
*/

/*
почему функции esp_now_register_recv_cb(messagereceived) мешает ota
Алиса
Разберу, почему вызов esp_now_register_recv_cb(messagereceived) может мешать OTA‑обновлению на ESP32, и предложу пути решения.

Причины конфликта
Конкуренция за ресурсы Wi‑Fi
ESP‑NOW и OTA используют Wi‑Fi‑модуль ESP32 одновременно:

ESP‑NOW требует постоянного Wi‑Fi‑соединения в режиме станции (STA) для приёма сообщений;
OTA‑обновление через Wi‑Fi (ArduinoOTA или ESP‑OTA) также требует стабильного Wi‑Fi‑подключения к роутеру.
Параллельная работа может привести к:

перегрузке Wi‑Fi‑стека;
потере пакетов из‑за конкуренции за канал;
нестабильности соединения, критичной для OTA.
Приоритет прерываний
Функция обратного вызова (messagereceived), зарегистрированная через esp_now_register_recv_cb, выполняется с высоким приоритетом при получении ESP‑NOW‑пакета. Это может:

временно блокировать другие задачи (в т. ч. обработку OTA‑трафика);
вызвать задержки в сетевом стеке, достаточные для таймаута OTA.
Ограничения протокола ESP‑NOW
работает только в режиме STA (не совместим с AP или AP+STA без тонкой настройки);
использует собственный MAC‑уровень поверх Wi‑Fi, что добавляет накладных расходов;
может конфликтовать с TCP/IP‑стеком, необходимым для OTA (обычно по протоколу HTTP или ArduinoOTA).
Потребление памяти и процессорного времени
обработчик ESP‑NOW занимает ОЗУ и Flash;
частые вызовы messagereceived нагружают CPU;
при OTA‑обновлении система уже испытывает нагрузку (приём и верификация прошивки), дополнительная нагрузка усугубляет ситуацию.
Таймауты и перезагрузка
OTA чувствителен к таймаутам:

если обработчик ESP‑NOW задерживает выполнение критически важных задач OTA дольше допустимого времени, процесс обновления прерывается;
в худшем случае — перезагрузка модуля до завершения OTA.
Практические решения
Вариант 1. Временное отключение ESP‑NOW на время OTA

Перед запуском OTA:

Отмените регистрацию callback‑функции:
cpp
esp_now_unregister_recv_cb();
При необходимости деинициализируйте ESP‑NOW:
cpp
esp_now_deinit();
Запустите OTA‑обновление.
После завершения OTA (успешного или нет) повторно инициализируйте ESP‑NOW и зарегистрируйте callback.
Вариант 2. Разделение режимов работы

Реализуйте логику, которая переключает устройство между режимами:

Режим данных: активен ESP‑NOW, OTA отключён;
Режим обновления: ESP‑NOW отключён, активирован OTA‑сервер.
Пример триггера перехода в режим обновления:

команда по ESP‑NOW с особым флагом;
нажатие кнопки на устройстве;
таймер (например, раз в сутки на 10 минут).
Вариант 3. Использование OTA по Bluetooth или UART

Если Wi‑Fi занят ESP‑NOW, рассмотрите альтернативные каналы OTA:

Bluetooth (BLE OTA): не конфликтует с ESP‑NOW;
UART OTA: требует физического подключения, но надёжен.
Вариант 4. Оптимизация обработчика ESP‑NOW

Сделайте messagereceived максимально легковесным:

не выполняйте в нём тяжёлых вычислений или операций ввода‑вывода;
помещайте полученные данные в очередь (FreeRTOS Queue);
обрабатывайте данные в отдельном низкоприоритетном потоке.
Пример:

cpp
void messagereceived(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  xQueueSendFromISR(esp_now_queue, &data, NULL); // Быстрое копирование в очередь
}
Вариант 5. Настройка приоритетов задач

В FreeRTOS:

задайте низкий приоритет задаче, обрабатывающей ESP‑NOW‑данные;
установите высокий приоритет для задач OTA;
используйте vTaskDelay() в обработчике ESP‑NOW для «уступания» времени другим задачам.
Вариант 6. Проверка и отладка

Добавьте логирование:

выводите в Serial статус ESP‑NOW и OTA;
отслеживайте использование памяти (heap_caps_get_free_size());
замеряйте время выполнения messagereceived.
Проверьте настройки:

убедитесь, что OTA использует отдельный порт (не конфликтующий с ESP‑NOW);
проверьте, что SSID/пароль Wi‑Fi корректны и соединение стабильно.
Краткий итог
Проблема  Решение
Конкуренция за Wi‑Fi  Отключить ESP‑NOW перед OTA
Высокий приоритет callback  Оптимизировать messagereceived, вынести обработку в задачу
Ограничения ESP‑NOW Использовать BLE/UART OTA
Таймауты OTA  Контролировать нагрузку, разделять режимы работы
Рекомендация: начните с Варианта 1 (временное отключение ESP‑NOW). Это самый простой и надёжный способ устранить конфликт. Если требуется непрерывный приём данных, комбинируйте его с Вариантом 4 (оптимизация callback).

Хотите, я раскрою какой‑то из пунктов подробнее или помогу с реализацией конкретного решения?

 */

/*

[Беспроводное управление без роутера: Кроссплатформенный скетч ESP-NOW для ESP32 и ESP8266]
(https://arduino-tex.ru/news/245/besprovodnoe-upravlenie-bez-routera.html)

[ESP-NOW, WIFI и OTA одновременно на передатчике и приемнике]
(https://translated.turbopages.org/proxy_u/en-ru.ru.775fcf0d-6a8864c1-51f20694-74722d776562/https/arduino.stackexchange.com/questions/77344/esp-now-and-wifi-and-ota-at-the-same-time-on-sender-and-receiver)

*/

void launchOTA() 
{
  sayln("Отключаем ESPNOW и переходим в режим OTA");
  deiESPNOW();
  iniWiFi(); 
  iniOTA(); 
}



// ************************************************************** WiFiOTA.h ***
