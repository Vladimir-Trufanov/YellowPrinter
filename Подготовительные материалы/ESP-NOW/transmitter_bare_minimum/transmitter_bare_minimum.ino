// transmitter_bare_minimum.ino
// Wolfgang Ewald [https://wolles-elektronikkiste.de/en/esp-now]

#include <esp_now.h>
#include <WiFi.h>

/*
[7] ESP32-2432S028 (CYD)         MAC-Address: B8:D6:1A:5C:F9:54
                  Formated: {0xB8, 0xD6, 0x1A, 0x5C, 0xF9, 0x54} 
*/
uint8_t receiverAddress[] = {0xB8, 0xD6, 0x1A, 0x5C, 0xF9, 0x54};
esp_now_peer_info_t peerInfo;

void setup()
{
    Serial.begin(115200);
    // delay(1000); // uncomment if your serial monitor is empty
    WiFi.mode(WIFI_STA);
    
    esp_now_init();
    
    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);
}
 
void loop()
{
    char message[] = "Hi, this is a message from the transmitting ESP";
    esp_now_send(receiverAddress, (uint8_t *) message, sizeof(message)-1); // -1 to not send the NULL terminator
    Serial.println(message);
    delay(5000);
}