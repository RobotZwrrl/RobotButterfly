// print why it woke up on startup
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}


void readMacAddress() {
  uint8_t baseMac[6];
  //wifi_interface_t ret = esp_netif_get_mac(WIFI_IF_STA, baseMac);
  //esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  esp_err_t ret = esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  if(ret == ESP_OK) {
    Serial << "MAC address: ";
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
    Serial << endl;
  } else {
    Serial << "Failed to read MAC address" << endl;
  }
}

