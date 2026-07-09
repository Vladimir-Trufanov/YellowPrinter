// receiver_bare_minimum.ino
// Wolfgang Ewald [https://wolles-elektronikkiste.de/en/esp-now]

#include <esp_now.h>
#include <WiFi.h>

void messageReceived(const esp_now_recv_info *info, const uint8_t* incomingData, int len)
{
    Serial.println("\nПрием: ");
    for(int i=0; i<len; i++)
    {
        Serial.print((char)incomingData[i]);
    }
    Serial.println();
}


void setup()
{
    Serial.begin(115200);
    // delay(1000); // uncomment if your serial monitor is empty
    WiFi.mode(WIFI_STA);
    esp_now_init();
    esp_now_register_recv_cb(messageReceived);
}

void loop(){}