// transmitter_basic8266.ino 


/*
Чем этот скетч отличается от самого простого?
В качестве формата нашего сообщения мы используем структуру (message). Это дает нам большую гибкость 
в отношении типов данных, включаемых в сообщение. В этом примере структура состоит из массива символов, 
целого числа и значения с плавающей запятой. 

Для массива символов необходимо указать максимальную ожидаемую длину.
Такие структуры, как message, представляют собой своего рода упрощенные классы. 
message myMessage; создает объект myMessage.
Возвращаемое значение esp_now_init() показывает, завершился ли процесс без ошибок.

Аналогичным образом мы используем возвращаемое значение esp_now_add_peer(). 
Важно: функция не проверяет, действительно ли узел доступен. Но, например, вы получите сообщение об ошибке,
если превышено максимальное количество узлов. Подробнее см. в документации по API.

Чтобы скопировать массив textMsg в массив myMessage.text нужно использовать memcpy(). 
Остальные элементы можно присвоить напрямую.

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

typedef struct messageToBeSent 
{
  char text[64];
  int intVal;
  float floatVal;
} messageToBeSent;

messageToBeSent myMessageToBeSent; 

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
  char textMsg[] = "Hi8266, here's my data for you: ";
  memcpy(&myMessageToBeSent.text, textMsg, sizeof(textMsg));
  myMessageToBeSent.intVal = 8266;
  myMessageToBeSent.floatVal = 82.66;
  esp_now_send(receiverAddress, (uint8_t *) &myMessageToBeSent, sizeof(myMessageToBeSent));
  delay(5000);
}



/* -------------------------------------------------
#include <espnow.h>
#include "ESP8266WiFi.h"

uint8_t receiverAddress[] = {0xA4, 0xCF, 0x12, 0xDF, 0x5D, 0x89};

typedef struct messageToBeSent {
    char text[64];
    int intVal;
    float floatVal;
} messageToBeSent;

typedef struct receivedMessage {
    char text[64];
    long runTime;
} receivedMessage;

messageToBeSent myMessageToBeSent; 
receivedMessage myReceivedMessage; 

void messageSent(uint8_t *macAddr, uint8_t status) {
    Serial.print("Send status: ");
    if(status == 0){
        Serial.println("Success");
    }
    else{
        Serial.println("Error");
    }
}

void messageReceived(uint8_t* macAddr, uint8_t* incomingData, uint8_t len){
    memcpy(&myReceivedMessage, incomingData, sizeof(myReceivedMessage));
    Serial.printf("Incoming Message from: %02X:%02X:%02X:%02X:%02X:%02X \n\r", 
            macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);    
    Serial.print("Message: ");
    Serial.println(myReceivedMessage.text);
    Serial.print("RunTime [s]: ");
    Serial.println(myReceivedMessage.runTime);
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    // delay(1000); // uncomment if your serial monitor is empty
    WiFi.mode(WIFI_STA);
    
    if (esp_now_init() == 0) {
        Serial.println("ESPNow Init success");
    }
    else {
        Serial.println("ESPNow Init fail");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    uint8_t result = esp_now_add_peer(receiverAddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0);
    if(result != 0){
        Serial.println("Failed to add peer");
    }
    
    esp_now_register_send_cb(messageSent);  
    esp_now_register_recv_cb(messageReceived); 
}
 
void loop(){
    char textMsg[] = "Hi, here's my data for you: ";
    memcpy(&myMessageToBeSent.text, textMsg, sizeof(textMsg));
    myMessageToBeSent.intVal = 4242;
    myMessageToBeSent.floatVal = 42.42;
    esp_now_send(receiverAddress, (uint8_t *) &myMessageToBeSent, sizeof(myMessageToBeSent));
    delay(5000);
}
*/
