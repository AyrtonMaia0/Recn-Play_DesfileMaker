
/*
 * Esses includes ja estavam no codigo deles
#include <Arduino.h>
#include <ESP8266WiFi.h>          //para usar uma rede WiFi
#include <ESP8266WiFiMulti.h>     //usado para uma conexao WiFi mais forte
#include <ESP8266HTTPClient.h>    //utilizar uma API
#include <WiFiClient.h>           //
#include <ArduinoJson.h>          //
*/



//=================================================================NOTACAO==============================================================================
/*
-chamar bibliotecas 
-colocar os defines
-função que serve para conectar com a rede wifi
-função que irá fazer conexão com a API
-anexar as funções de maneira globais
-inicio da função principal (main)
-passar as portas que foram definidas no topo (pinMode)
-colocar o chamado para o Monitor Serial
-Colocar um chamado da função “conecta Wifi” antes de fechar o ‘void setup()’
-’void loop’
 */



//==================================================================INCLUDES & DEFINES==================================================================
// ---IMPORTANDO BIBLIOTECAS E CRIANDO STRUCT
#include <ESP8266WiFi.h>                                                                     //para usar uma rede WiFi
#include <ESP8266HTTPClient.h>                                                               //utilizar uma API
#include <Arduino.h>
#include <ESP8266WiFiMulti.h>                                                                //usado para uma conexao WiFi mais forte
#include <ArduinoJson.h>

#define NWIFIS 3                                                                             // qtd de redes wifi = 3
//#define                                                                                      //http da API que esta a rota para o site



// ---RE-AVALIAR ESSA PARTE. SE FAZ NECESSARIO TER UM HEX OU A API RETORNA UM VALOR PROPRIO
#define REPETE 0xFFFFFFFF                                                                    //caso ocorra de a pessoa segurar o mesmo botao aparecerá esse Hexadecimal
#define BT_COLAR 0x20DF8877                                                                  //valor referente ao botao COLAR que vai estar no site
#define BT_JANELA 0x20DF48B7                                                                 //valor referente ao botao JANELA que vai estar no site
#define BT_OLHOS 0x20DFC837                                                                  //valor referente ao botao OLHOS do CRAB que vai estar no site
#define BT_CRAB 0x20DF28D7                                                                   //valor referente ao botao CRAB que vai estar no site

// ---DEFINICAO DE PORTAS
#define colar 3                                                                              //porta onde esta conectado o colar no lilypad D03 -Digital PWM
#define janela 9                                                                             //porta onde esta conectado o janela no lilypad D09 -Digital PWM
#define olhos 10                                                                             //porta onde esta conectado o olhos no lilypad D10 -Digital PWM
#define crab 11                                                                              //porta onde esta conectado o crab no lilypad D11 -Digital PWM

#define LED 5 //para testes


//========================================================================STRUCT=======================================================================
struct redes{                                                                                //criaçao de struct para os wifi
  const char *ssid;                                                                          //variavel que guardara o nick do roteador
  const char *senha;                                                                         //variavel que guardara a senha do roteador
};

char *redeAtual;                                                                             //variavel que vai receber o wifi que esta conectado

struct redes redeWifi[NWIFIS]={"SENAC-Mesh","09080706",
                               "nickRoteador", "000000",
                               "nickRoteador", "000000"};                                    //struct que é passado os wifis




//========================================================================FUNCOES=======================================================================
// -------------------FUNCAO PARA CONECTAR NO PRIMEIRO WIFI DISPONIVEL
bool conectaWifi()
{
  bool conectou = false;
  int i=0;
  int qtd = 0;

  while( i < NWIFIS ){                                                                      //faz a captura de cada uma das redes
    
      Serial.println("Conectando: wifi");
      Serial.println( redeWifi[i].ssid );
      WiFi.begin(redeWifi[i].ssid, redeWifi[i].senha);                                      //passando os wifis do struct

      qtd = 0;
 
      while(WiFi.status() != WL_CONNECTED){                                                 //se estiver conectado ele vai somar +1 no qtd
        delay(1000);
        Serial.print(".");
        qtd++;
        if(qtd > 4){
          break;
        }
      }
      
      if(WiFi.status() == WL_CONNECTED){                                                    //se ele ainda estiver conectado com o a rede, aparece no monitor serial o IP
        Serial.println("Conectado com sucesso. IP: ");
        Serial.println(WiFi.localIP());      
        Serial.println( redeWifi[i].ssid );
        delay( 2000 );
        conectou = true;
        redeAtual = (char *)redeWifi[i].ssid;
        return conectou;
      }
      i++;
    }
  return conectou;
}



// -----------------------------------FUNCAO GERAL LED

int valor = 1;
float sinVal;
int ledVal;

/*
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+FUNCAO RECEBER INFRAVERMELHO
    
    int ir_receptor()
    {
      if (irrecv.decode(&results))                                                          //se algum código for recebido
      {
        Serial.println(results.value, HEX);                                                 //imprime o RESULTADO = valor, no Monitor Serial
        return results.value;                                                               //retorna esse valor para ser usado no Switch case e chamar a funcao de acendimento de leds
      }else{
        return 0;
      }
    }
*/


    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+FUNCAO GRADATIVAMENTE & ACELERA COM O TEMPO
    void gradativamente(){
      for (int x=0; x<180; x++) {
        // converte graus para radianos e então obtém o valor do seno
        sinVal = (sin(x*(3.1412/180)));
        ledVal = int(sinVal*255);
        analogWrite(LED, ledVal);
        delay(25);
      }
     }



    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+FUNCAO SO ACENDER
     void apenasAcender(){
        digitalWrite(LED, HIGH);                                                            //acender led
        delay(300);                                                                         //após 300milisegundos
     }



    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+FUNCAO PISCAR LED RAPIDO
     void piscarRapido(){
        digitalWrite(LED, HIGH);                                                            //acender led
        delay(120);                                                                         //esperar 120milisegundos
        digitalWrite(LED, LOW);                                                             //apagar led
        delay(120);                                                                         //esperar 120milisegundos 
     }



    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+FUNCAO PISCAR LED LENTO
     void piscarLento(){
        digitalWrite(LED, HIGH);                                                            //acender led
        delay(1050);                                                                        //esperar 1050milisegundos
        digitalWrite(LED, LOW);                                                             //apagar led
        delay(1050);                                                                        //esperar 1050milisegundos 
     }



    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ACENDER E APAGAR DE FORMA GRADUAL
    void gradual(){                                                                         //Lembrar de conectar numa porta PWN para funcinar (a porta com o ~
      for(int i=0; i<=255; i++){                                                            //acendendo o led conforme o number 'i' que vai de 0 - 255
        analogWrite(LED, i);
        delay(3);                                                                           //espera cerca de 3milisigundos para voltar ao inicio do for e acender +1 ao led
      }
      delay(300);                                                                           //tempo do termino de um for(acender) e inicio do for(apagar)
     
      for(int i=255; i>=0; i--){                                                            //apagando o led confotme o number 'i' que vai de 0 - 255
        analogWrite(LED, i);
        delay(3);
      }
      delay(300);
    }



//=================================================================FUNCAO GERAL LED================================================================================
int geralLed(){

  switch(valor){
    case 1:
      gradativamente();                                                                     //chamado da funcao que acende e apagua gradativamente e vai acelerando com o tempo
      break;
    case 2:
      apenasAcender();                                                                      //chamado da funcao que apenas acende
      break;
    case 3:
      piscarRapido();                                                                       //chamado da funcao que faz piscar os leds rapido
      break;
    case 4:
      piscarLento();                                                                        //chamado da funcao que faz piscar os leds lento
      break;
    case 5:
      gradual();                                                                            //chamado da funcao que acende de forma gradual e apaga da mesma maneira
      break;
      /*lembrar de [usar/fazer/modificar] millis*/
  }
  return 0;
}




//==========================================================================MAIN=======================================================================
                   
void setup() {
  conectaWifi();                                                                            //chamada da funcao que verifica qual wifi está pegando
  pinMode(LED, OUTPUT);                                                                     //especificando se é uma porta de entrada ou de saida

}


void loop(){
  //falta colocar para receber o hexadecimal e passa-lo como parametro
  geralLed();                                                                               //funcao geral que chama as funcoes para acendimento dos leds
}
