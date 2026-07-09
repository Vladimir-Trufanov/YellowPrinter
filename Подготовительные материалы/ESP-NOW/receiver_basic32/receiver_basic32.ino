// receiver_basic32.ino 

#include <esp_now.h>
#include <WiFi.h>

typedef struct message 
{
    char text[64];
    int intVal;
    float floatVal;
} message;

message myMessage;

void messageReceived(const esp_now_recv_info *info, const uint8_t* incomingData, int len)
{
    memcpy(&myMessage, incomingData, sizeof(myMessage));
    Serial.printf("Transmitter MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n\r", 
            info->src_addr[0], info->src_addr[1], info->src_addr[2], info->src_addr[3], info->src_addr[4], info->src_addr[5]);    
    Serial.print("Message: ");
    Serial.println(myMessage.text);
    Serial.print("Integer Value: ");
    Serial.println(myMessage.intVal);
    Serial.print("Float Value: ");
    Serial.println(myMessage.floatVal);
    Serial.println();
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

    esp_now_register_recv_cb(messageReceived);
}
 
void loop(){}
