// transmitter_8266_32.ino 


/*
Чем этот скетч отличается от самого простого?

Возвращаемое значение esp_now_init() показывает, завершился ли процесс без ошибок.
Аналогичным образом мы используем возвращаемое значение esp_now_add_peer(). 
Важно: функция не проверяет, действительно ли узел доступен. Но, например, вы получите сообщение об ошибке,
если превышено максимальное количество узлов. Подробнее см. в документации по API.

С помощью esp_now_register_send_cb((esp_now_send_cb_t)messageSent); мы регистрируем функцию messageSent, 
которая вызывается после отправки сообщения. Параметры предопределены.
Проверка status в функции messageSent и проверка result в качестве возвращаемого значения esp_now_send()
проверяют разные критерии. Подробности — в документации по API. 
*/

#include <espnow.h>
#include "ESP8266WiFi.h"
/*
[7] ESP32-2432S028 (CYD)         MAC-Address: B8:D6:1A:5C:F9:54
                  Formated: {0xB8, 0xD6, 0x1A, 0x5C, 0xF9, 0x54} 
*/
uint8_t receiverAddress[] = {0xB8, 0xD6, 0x1A, 0x5C, 0xF9, 0x54};

void messageSent(uint8_t *macAddr, uint8_t status) 
{
  Serial.print("Send status: ");
  if(status == 0)
  {
    Serial.println("Success");
  }
  else
  {
    Serial.println("Error");
  }
}

void setup()
{
  Serial.begin(115200);
  // delay(1000); // uncomment if your serial monitor is empty
  WiFi.mode(WIFI_STA);
  if (esp_now_init() == 0) 
  {
    Serial.println("ESPNow Init success");
  }
  else 
  {
    Serial.println("ESPNow Init fail");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  // Добавляем устройство в список сопряжённыз для протокола ESP-NOW
  // до отправки данных на это устройство. Без добавления в список 
  // система не позволит отправить данные на указанный MAC-адрес.
  // Отправка готовится по 0 (текущему) каналу без шифрования (NULL,0) 
  uint8_t result = esp_now_add_peer(receiverAddress, ESP_NOW_ROLE_CONTROLLER, 0, NULL, 0);
  if(result != 0)
  {
    Serial.println("Failed to add peer");
  }
  // Регистрируем callback-функцию, которая будет вызываться каждый раз после того, 
  // как данные успешно отправлены с одного устройства ESP-NOW на другое.
  esp_now_register_send_cb(messageSent);  
}

 
void loop()
{
  char message[] = "Hi 8266, this is a message from the transmitting ESP";
  esp_now_send(receiverAddress, (uint8_t *) message, sizeof(message)-1); // -1 to not send the NULL terminator
  Serial.println(message);
  delay(5000);
}
