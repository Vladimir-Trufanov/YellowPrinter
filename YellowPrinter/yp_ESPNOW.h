/** YellowPrinter-Esp32-Arduino                             *** yp_ESPNOW.h ***
 * 
 * Обcлужить работу с ESPNOW
 * 
 * v2.0.10, 22.08.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 12.08.2026
**/

#pragma once

#include <esp_now.h>

#include "inimem.h"

void messageReceived(const esp_now_recv_info *info, const uint8_t* incomingData, int len);
void iniESPNOW(); 
void deiESPNOW(); 

void iniESPNOW() 
{
  if (esp_now_init() == ESP_OK) 
  {
    Serial.println("ESPNow Init success");
    //esp_now_register_recv_cb(messageReceived);
    if (esp_now_register_recv_cb(messageReceived) != ESP_OK) 
      Serial.println("ESPNow Register Message fail");
  }
  else 
  {
    Serial.println("ESPNow Init fail");
    return;
  }
}

void deiESPNOW() 
{
  if (esp_now_unregister_recv_cb() != ESP_OK) 
    Serial.println("ESPNow UnRegister Message fail");
  else
  if (esp_now_deinit() == ESP_OK) 
  {
    Serial.println("ESPNow DeINIT success");
  }
  else 
  {
    Serial.println("ESPNow DeINIT fail");
    return;
  }
}

// ****************************************************************************
// *            Принять поступающее сообщение в захвате мьютекса              *
// ****************************************************************************
void messageReceived(const esp_now_recv_info *info, const uint8_t* incomingData, int len)
{
  messBool=true;  
  while (messBool) 
  {
    // Как только захватили мьютекс, выполняем свою работу
    if (xSemaphoreTake (messMutex, portMAX_DELAY)) 
    {
      memset(CtrlMessage.line,'\0',smLINESIZE); 
      memcpy(&CtrlMessage, incomingData, len);
      messCalc++;
      messBool=false;
      /*
      Serial.printf("\nTransmitter MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n", 
        info->src_addr[0], info->src_addr[1], info->src_addr[2], info->src_addr[3], info->src_addr[4], info->src_addr[5]);    
      */
      Serial.print("CtrlMessage.line: "); Serial.println(CtrlMessage.line);
      /*
      Serial.printf("Длительность messageReceived(): %d ms\n\n", duration * portTICK_PERIOD_MS);
      */
      xSemaphoreGive (messMutex);  
    }
    vTaskDelay(64);
  }
}

// ************************************************************ yp_ESPNOW.h ***
