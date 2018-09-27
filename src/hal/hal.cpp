
#include <Arduino.h>
#include "hal.h"

extern "C" void hal_init_cpp(void){
  Serial.begin(115200);
  delay(100);
  Serial.println("Serial is initialized by mruby/c HAL");
}

extern "C" void hal_write_string(char* text){
  Serial.print(text);
}

