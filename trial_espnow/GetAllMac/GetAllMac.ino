// GetAllMac.ino - узнать mac-адреса сетевых интерфейсов ESP32

/*
 AI Thinker ESP32-CAM 6  - MAC Address: F8:B3:B7:A7:E1:18
                         --------------------------------
                         Station MAC:   F8:B3:B7:A7:E1:18
                         SoftAP MAC:    F8:B3:B7:A7:E1:19
                         Bluetooth MAC: F8:B3:B7:A7:E1:1A
                         Ethernet MAC:  F8:B3:B7:A7:E1:1B
                         
 ESP32-2432S028 (CYD) 7  - MAC Address: B8:D6:1A:5C:F9:54
                         --------------------------------
                         Station MAC:   B8:D6:1A:5C:F9:54
                         SoftAP MAC:    B8:D6:1A:5C:F9:55
                         Bluetooth MAC: B8:D6:1A:5C:F9:56
                         Ethernet MAC:  B8:D6:1A:5C:F9:57

 AI Thinker ESP32-CAM 8  - MAC Address: 94:54:C5:A8:21:9C
                         --------------------------------
                         Station MAC:   94:54:C5:A8:21:9C
                         SoftAP MAC:    94:54:C5:A8:21:9D
                         Bluetooth MAC: 94:54:C5:A8:21:9E
                         Ethernet MAC:  94:54:C5:A8:21:9F
*/

#include <WiFi.h>
#include <esp_mac.h>

// Variable to store the MAC address
uint8_t baseMac[6];

void setup()
{
  Serial.begin(115200);
  Serial.println("\n");

  GetOneMac("Station MAC:   ", ESP_MAC_WIFI_STA);
  GetOneMac("SoftAP MAC:    ", ESP_MAC_WIFI_SOFTAP);
  GetOneMac("Bluetooth MAC: ", ESP_MAC_BT);
  GetOneMac("Ethernet MAC:  ", ESP_MAC_ETH);
}

void GetOneMac(char mess[], esp_mac_type_t macind)
{
  esp_read_mac(baseMac, macind);
  Serial.print(mess);
  for (int i = 0; i < 5; i++) 
  {
    Serial.printf("%02X:", baseMac[i]);
  }
  Serial.printf("%02X\n", baseMac[5]);
}
 
void loop()
{
}
