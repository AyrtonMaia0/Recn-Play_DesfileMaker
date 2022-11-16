#include <IRremote.h>
#define pinLed 13
#define EMI 3

IRsend ir(EMI);

void setup(){
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);
}
void loop(){
  int valorLido = analogRead(A0);
  float valor = 0xAAAAAA;
  
  if ( valorLido > 800 ) {
  digitalWrite(pinLed, HIGH);
   ir.sendRC5(valor, 4);
 Serial.println( valorLido );
  }
}
