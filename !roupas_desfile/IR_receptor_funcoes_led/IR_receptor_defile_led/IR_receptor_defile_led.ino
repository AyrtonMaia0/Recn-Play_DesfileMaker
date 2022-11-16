

// codigo geral do dia
//1- recebe o hex da central ,o som
//2- if else 
//3- if {chama a funcao que acende com as batidas do grave}
//4- else {}


//=================================================================INCLUDES BIBLIOTECAS================================================================================
#include <IRremote.h>                                                                       // Biblioteca IRemote
//#include <IRremote.hpp>


//======================================================================DEFINES========================================================================================

#define RECV_PIN 13                                                                         // Arduino pino D11 conectado no Receptor IR
#define LEDAMARELO 5
#define LEDVERMELHO 4

#define LED1 9 //para testes
#define LED2 10
#define LED3 11

IRrecv irrecv(RECV_PIN);                                                                    // criando a instância
decode_results results;                                                                     // declarando os resultados



//====================================================================INFRAVERMELHO====================================================================================

// Código receptor + led
//int hexa;

/*
int receptor()
{
  if (irrecv.decode(&results))                                                              // se algum código for recebido
  {
    Serial.println(results.value, HEX);                                                     // imprime o HEX no Monitor Serial
    
    
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
  hexa = results.value;                                                                     // atribui o resultado HEX na variavel 'hexa'
  return hexa;                                                                              // retorna a variavel 'hexa' com o dado HEX nela
}
*/

//=================================================================FUNCOES PARA ACENDER LED================================================================================

int hexa;
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+FUNCAO RECEBER INFRAVERMELHO
    
    int ir_receptor()
    {
      if (irrecv.decode(&results))                                                          // se algum código for recebido
      {
        Serial.println(results.value, HEX);                                                 // imprime o RESULTADO = valor, no Monitor Serial
        hexa = results.value;                                                               // atribui o resultado HEX na variavel 'hexa'
        return results.value;                                                               // retorna a variavel 'hexa' com o HEX para chamar a funcao de acendimento de leds
      }
    }



int valor = hexa;                                                                           // 'valor' vai receber o resultado do hexadecimal que esta na variavel 'hexa'
float sinVal;
int ledVal;




    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+FUNCAO GRADATIVAMENTE & ACELERA COM O TEMPO
    void gradativamente(){
      for (int x=0; x<180; x++) {
        // converte graus para radianos e então obtém o valor do seno
        sinVal = (sin(x*(3.1412/180)));
        ledVal = int(sinVal*255);
        analogWrite(LED, ledVal);
        delay(25);
      }
     }



    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+FUNCAO SO ACENDER
     void apenasAcender(){
        digitalWrite(LED, HIGH);                                                            // acender led
        delay(300);                                                                         // após 300milisegundos
     }



    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+FUNCAO PISCAR LED RAPIDO
     void piscarRapido(){
        digitalWrite(LED, HIGH);                                                            // acender led
        delay(200);                                                                         // esperar 120milisegundos
        digitalWrite(LED, LOW);                                                             // apagar led
        delay(200);                                                                         // esperar 120milisegundos 
     }



    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+FUNCAO PISCAR LED LENTO
     void piscarLento(){
        digitalWrite(LED, HIGH);                                                            // acender led
        delay(1050);                                                                        // esperar 1050milisegundos
        digitalWrite(LED, LOW);                                                             // apagar led
        delay(1050);                                                                        // esperar 1050milisegundos 
     }



    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ACENDER E APAGAR DE FORMA GRADUAL
    void gradual(){                                                                         // Lembrar de conectar numa porta PWN para funcinar (a porta é com ~)
      for(int i=0; i<=255; i++){                                                            // acendendo o led conforme o number 'i' que vai de 0 - 255
        analogWrite(LED, i);
        delay(3);                                                                           // espera cerca de 3milisigundos para voltar ao inicio do for e acender +1 ao led
      }
      delay(300);                                                                           // tempo do termino de um for(acender) e inicio do for(apagar)
     
      for(int i=255; i>=0; i--){                                                            // apagando o led confotme o number 'i' que vai de 0 - 255
        analogWrite(LED, i);
        delay(3);
      }
      delay(300);
    }



//=================================================================FUNCAO GERAL LED================================================================================
void geralLed(){
/*
  switch(valor){                                                                            //poderia colocar diretamente a variavel 'hexa' mas preferir usar a 'valor'
    case 0x11:
      gradativamente();                                                                     // chamado da funcao que acende e apagua gradativamente e vai acelerando com o tempo
      break;
    case 0x22:
      apenasAcender();                                                                      // chamado da funcao que apenas acende
      break;
    case 0x33:
      piscarRapido();                                                                       // chamado da funcao que faz piscar os leds rapido
      break;
    case 0x44:
      piscarLento();                                                                        // chamado da funcao que faz piscar os leds lento
      break;
    case 0x55:
      gradual();                                                                            // chamado da funcao que acende de forma gradual e apaga da mesma maneira
      break;
      /*lembrar de [usar/fazer/modificar] millis
  }
}*/
/*OU ENTAO , NO LUGAR DE COLOCAR O SWITCH CASE , FAZER UM SISTEMA DE IF ELSE*/

  if(valor == 0x11){
    gradual();
    
  }
  else{
    piscarRapido();
  }
}


 

//==========================================================================MAIN=======================================================================
void setup() 
{
  Serial.begin(9600);                                                                       // Monitor velocidade 9600 bps
  irrecv.enableIRIn();                                                                      // Inicializa a recepção de códigos
  pinMode(LEDAMARELO, OUTPUT);  
  pinMode(LEDVERMELHO, OUTPUT);

  pinMode(LED, OUTPUT);
}

void loop() 
{
  ir_receptor();
  geralLed();
  /*lembrar de [usar/fazer/modificar] millis*/
}
