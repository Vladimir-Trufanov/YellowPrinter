// transmitter_basic32.ino

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

#include <esp_now.h>
#include <WiFi.h>

/*
[7] ESP32-2432S028 (CYD)         MAC-Address: B8:D6:1A:5C:F9:54
                  Formated: {0xB8, 0xD6, 0x1A, 0x5C, 0xF9, 0x54} 
*/
uint8_t receiverAddress[] = {0xB8, 0xD6, 0x1A, 0x5C, 0xF9, 0x54};
esp_now_peer_info_t peerInfo;

typedef struct message 
{
    char text[64];
    int intVal;
    float floatVal;
} message;

message myMessage; 

void messageSent(const uint8_t *macAddr, esp_now_send_status_t status) 
{
    Serial.print("Send status: ");
    if(status == ESP_NOW_SEND_SUCCESS)
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
    
    if (esp_now_init() == ESP_OK) 
    {
        Serial.println("ESPNow Init success");
    }
    else 
    {
        Serial.println("ESPNow Init fail");
        return;
    }
    
    esp_now_register_send_cb((esp_now_send_cb_t)messageSent);   

    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) 
    {
        Serial.println("Failed to add peer");
        return;
    }
}
 
void loop()
{
    char textMsg[] = "[32] Hi Receiver, here's my data for you: ";
    memcpy(&myMessage.text, textMsg, sizeof(textMsg));
    myMessage.intVal = 3232;
    myMessage.floatVal = 32.32;
    esp_err_t result = esp_now_send(receiverAddress, (uint8_t *) &myMessage, sizeof(myMessage));
    if (result != ESP_OK) 
    {
        Serial.println("Sending error");
    }
    else
    {
        Serial.println(textMsg);
    }
    delay(4000);
}
