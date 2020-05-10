//https://circuitdigest.com/microcontroller-projects/arduino-interrupt-tutorial-with-examples

#include <Arduino.h>
// 
const uint8_t led =  13;                      // Hardware Pin for LED
const uint8_t stop_Button =  14;              // Hardware pin for Button

volatile float Actual_Frequency = 0;           // Recaluated Frequency

boolean led_state = 0;                        

void ISR_button();                      // Interrupt Service Routine Decleration for Button

void setup() {
  uint32_t startup_previous_ms = 0;             // hold Previous value for Startup Blinks
  uint32_t startup_current_ms = 0;              // hold Current value for Startup Blinks
  const uint32_t startup_interval_ms = 500;     // Default interval for Startup blinks

  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led,led_state); 
  Serial.println("Starting Flicker Fusion");
  startup_previous_ms = millis();

  for(uint8_t i = 0; i < 4; )           // "i" max value = Number of Initial Blinks x 2
  {
    startup_current_ms =  millis();

    if ((startup_interval_ms) <= (startup_current_ms - startup_previous_ms ))
    {
      startup_previous_ms = startup_current_ms ;
      led_state = !led_state;
      digitalWrite(led,led_state); 
      i++;                              // i Increment For "for Loop" 
    }
  }

  pinMode(stop_Button, INPUT_PULLUP);
  attachInterrupt(stop_Button,ISR_button,FALLING) ;

}

void loop() {
  uint8_t Target_Frequency = 1;
  uint32_t current_us = 0;                              // hold Current value for Change_Freq
  uint32_t Change_Freq_previous_us = micros();           // hold Previous value for Change_Freq
  const uint32_t Change_Freq_interval_us = 3000000;     // Default interval for Change_Freq
  uint32_t Generated_Freq_previous_us = micros();             // hold Previous value for Generated_Freq
  uint32_t Generated_Freq_interval_us = 1000000/ (2 * Target_Frequency);         // interval for Generated_Freq

  while(1)
  { 
    current_us = micros() ;
    if ((Change_Freq_interval_us) <= (current_us - Change_Freq_previous_us))
    { 
      Change_Freq_previous_us = current_us ;
      Target_Frequency++;
      Generated_Freq_interval_us = 1000000/ (2 * Target_Frequency);
    }

    if ((Generated_Freq_interval_us) <= (current_us - Generated_Freq_previous_us))
    {   
      Actual_Frequency = 1000000.0 / (2 * (current_us - Generated_Freq_previous_us)) ;  // Frequency Calculations
      Generated_Freq_previous_us = current_us ;
      led_state = !led_state;
      digitalWrite(led,led_state); 
      Serial.print("F: ");
      Serial.println(Actual_Frequency,4);
      Serial.flush();
    }
  }

}

void ISR_button()             // Interrupt Service Routine Definition for Button
{      
  Serial.print("Your Flicker Fustion Threshold is ");
  Serial.print(Actual_Frequency,4);
  Serial.println(" Hz");
  Serial.println("Plase, Reset the Board to Replay") ;
  while(1)
  {  }                        
}