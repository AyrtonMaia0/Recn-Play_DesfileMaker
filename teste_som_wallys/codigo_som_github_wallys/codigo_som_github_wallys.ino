



//======================================================================EMISSOR========================================================================================


#include "LowPassFilter.c"
//#include "SampleFilter.c"
#include "IRremote.h"

#define micPin 0
#define emissor 3
#define pot A5

IRsend ir(emissor);
float valor;
float valor2;

//The number of samples to buffer before analyzing them
int samplesN = 200;

SampleFilter* filter;

void setup(){
  Serial.begin(115200);
  filter = new SampleFilter();
  SampleFilter_init(filter);
  pinMode(13, OUTPUT);
}

int index = 0;
int maxpeak = 0 ;
int minPeak = 1023;

void loop(){
  // COMEÇANDO A PARTE DO FILTRO
  int peak = 0;

  for(int k=0; k<samplesN; k++){
    int val = analogRead(micPin);
    SampleFilter_put(filter, val);

    int filtered = SampleFilter_get(filter);
    peak = max(peak, filtered);
  }  
  maxpeak = max(maxpeak, peak);
  minPeak = min(minPeak, peak);

  index++;
  if(index == 1000){
    maxpeak = 0;
    minPeak = 1023;
  }
  int lvl = map(peak, minPeak, maxpeak, 0, 1023);
  // TERMINOU A PARTE DO FILTRO
  
  //Mostrando o volume que o microfone tá ouvindo com o filtro (lvl) e o valor que ele tá ouvindo sem o filtro (analogRead(micPin))
  Serial.print(lvl);
  Serial.print(" || ");
  Serial.println(analogRead(micPin));
  
  int valorPot = analogRead(pot); // Lendo o valor que tá no potenciometro e armazenando em valorPot
  
  valor = 0x11111;
  valor2 = 0xAAAAAA;
  
  if (lvl > valorPot) {
    digitalWrite(13, HIGH);
    ir.sendRC5(valor, 4);
    delay(5);
  } else if (lvl > (valorPot - 200)) {
    ir.sendRC5(valor2, 4);
    delay(5);
  } else {
    digitalWrite(13, LOW);
  }

  Serial.println(valorPot); // Mostrando o valor que tá no potenciometro
}





//======================================================================RECEPTOR========================================================================================



// Código receptor + led
#include <IRremote.h>                       // Biblioteca IRemote
#define RECV_PIN 13                       // Arduino pino D11 conectado no Receptor IR
#define LEDAMARELO 5
#define LEDVERMELHO 4

IRrecv irrecv(RECV_PIN);                    // criando a instância
decode_results results;                     // declarando os resultados

void setup()
{
  Serial.begin(9600);                       // Monitor velocidade 9600 bps
  irrecv.enableIRIn();                      // Inicializa a recepção de códigos
  pinMode(LEDAMARELO, OUTPUT);
  pinMode(LEDVERMELHO, OUTPUT);
}

void loop()
{
  if (irrecv.decode(&results))              // se algum código for recebido
  {
    Serial.println(results.value, HEX);     // imprime o HEX Code]
    if (results.value == 0x11) {
      Serial.println(results.value);
      Serial.println("Botão 1 apertado, acendendo led amarelo");
      if (digitalRead(LEDAMARELO) == 0) {
        digitalWrite(LEDAMARELO, HIGH);
      } else {
        digitalWrite(LEDAMARELO, LOW);
      }
      irrecv.resume();
      delay(100);
    }

    if (results.value == 0x1A) {
      Serial.println(results.value);
      Serial.println("Botão 2 apertado, acendendo led vermelho");
      if (digitalRead(LEDVERMELHO) == 0) {
        digitalWrite(LEDVERMELHO, HIGH);
      } else {
        digitalWrite(LEDVERMELHO, LOW);
      }
      irrecv.resume(); 
      delay(100); 
    }
    irrecv.resume(); 
    delay(100); 
  }
}
