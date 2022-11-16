


//#include <ESP8266WiFi.h>                                                      //para usar rede wifi
//#include <ESP8266WebServer.h>
/*
/* Put your SSID & Password /
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details /
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);
*/
//--------------------------------------------------------------------
/*
#include "ESP8266WiFi.h"
void setup()
{
  Serial.begin(115200);      // configura monitor serial 115200 Bps
  Serial.println();          // imprime uma linha
  WiFi.mode(WIFI_STA);       // configura rede no modo estacao
  WiFi.disconnect();         // desconecta rede WIFI
  delay(100);                // atraso de 100 milisegundos
}
void prinScanResult(int networksFound)
{
  Serial.printf("\n");                                      // imprime uma linha
  Serial.printf("%d redes encontradas\n", networksFound);   // imprime numero de redes encontradas
  for (int i = 0; i < networksFound; i++)                   // contagem das redes encontradas
  {
    Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "aberta" : "");
  }
}
void loop()
{
  WiFi.scanNetworksAsync(prinScanResult);        // imprime o resultado
  delay(500);                                    // atraso de 0,5 segundos
}
*/
//====================================================================================================================================================================================================================================================



#include <ESP8266WiFi.h>

const char* ssid = "********";                                                                                    //nome da rede wifi
const char* password = "********";                                                                                //senha da rede wifi

WiFiServer server(80); //Shield irá receber as requisições das páginas (o padrão WEB é a porta 80)                //a classe que será utilizada


//VARIAVEIS UTILIZADAS PELO CODIGO DO SKETCH
String HTTP_req;                                                                                                  //variaveis utilizadas pelo codigo do Sketch 
String URLValue;                                                                                                  //


//DECLARACAO DE FUNCOES QUE SERAM UTILIZADAS MAIS ABAIXO 
void processaPorta(byte porta, byte posicao, WiFiClient cl);
void lePortaDigital(byte porta, byte posicao, WiFiClient cl);        
void lePortaAnalogica(byte porta, byte posicao, WiFiClient cl);   
String getURLRequest(String *requisicao);
bool mainPageRequest(String *requisicao);

/*
 * GPIO COM BASE NA PORTA DA ESP8266
D00 = GPIO 16
D01 = GPIO 5
D02 = GPIO 4
D03 = GPIO 0
D04 = GPIO 2
D05 = GPIO 14
D06 = GPIO 12
D07 = GPIO 13
D08 = GPIO 15
D09/RX = GPIO 3
D10/TX = GPIO 1
D011/SD2 = GPIO 9
D12/SD3 = GPIO 10
*/



const byte qtdePinosDigitais = 7;                                                                                   //quantidade de pinos digitais que seram utilizadas
byte pinosDigitais[qtdePinosDigitais] = {2           , 4     , 5     , 12     , 13     , 14     , 15     };         //qual é cada porta que quero controlar (GPIO)
byte modoPinos[qtdePinosDigitais]     = {INPUT_PULLUP, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT};             //o modo em que eu quero cada uma dessas portas

const byte qtdePinosAnalogicos = 1;                                                                                 //quantidade de pinos analogicos 
byte pinosAnalogicos[qtdePinosAnalogicos] = {A0};                                                                   //qual é essa porta analogica






void setup()
{         
    Serial.begin(115200);

    //Conexão na rede WiFi
    Serial.println();
    Serial.print("Conectando a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi conectado!");

    // Inicia o servidor WEB
    server.begin();
    Serial.println("Server iniciado");

    // Mostra o endereco IP
    Serial.println(WiFi.localIP());

    //Configura o modo dos pinos
    for (int nP=0; nP < qtdePinosDigitais; nP++) {
        pinMode(pinosDigitais[nP], modoPinos[nP]);
    }
}

void loop()
{

    WiFiClient  client = server.available();

    if (client) { 
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) { 
                char c = client.read(); 
                HTTP_req += c;  
                
                if (c == '\n' && currentLineIsBlank ) { 

                    if ( mainPageRequest(&HTTP_req) ) {
                        URLValue = getURLRequest(&HTTP_req);
                        Serial.println(HTTP_req);
                                                 
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");              //<------ ATENCAO
                        client.println();
                        
                        //Conteudo da Página HTML
                        client.println("<!DOCTYPE html>");
                        client.println("<html>");

                        
                        client.println("<head>");
                        client.println("<title>Arduino via WEB</title>");

                        client.println("<script>");
                        client.println("function LeDadosDoArduino() {");
                        client.println("nocache = \"&nocache=\" + Math.random() * 1000000;");
                        client.println("var request = new XMLHttpRequest();");
                        client.println("var posIni;");
                        client.println("var valPosIni;");
                        client.println("var valPosFim;");
                        client.println("request.onreadystatechange = function() {");
                        client.println("if (this.readyState == 4) {");
                        client.println("if (this.status == 200) {");
                        client.println("if (this.responseText != null) {");

                        for (int nL=0; nL < qtdePinosDigitais; nL++) {                                                    //<-------NOVO
                            client.print("posIni = this.responseText.indexOf(\"PD");
                            client.print(pinosDigitais[nL]);
                            client.println("\");");
                            client.println("if ( posIni > -1) {");
                            client.println("valPosIni = this.responseText.indexOf(\"#\", posIni) + 1;");
                            client.println("valPosFim = this.responseText.indexOf(\"|\", posIni);");
                            client.print("document.getElementById(\"pino");
                            client.print(pinosDigitais[nL]);
                            client.println("\").checked = Number(this.responseText.substring(valPosIni, valPosFim));");
                            client.println("}");
                        }

                        for (int nL=0; nL < qtdePinosAnalogicos; nL++) {                                                    //<-------NOVO
                            
                            client.print("posIni = this.responseText.indexOf(\"PA");
                            client.print(pinosAnalogicos[nL]);
                            client.println("\");"); 
                            client.println("if ( posIni > -1) {");
                            client.println("valPosIni = this.responseText.indexOf(\"#\", posIni) + 1;");
                            client.println("valPosFim = this.responseText.indexOf(\"|\", posIni);");
                            client.print("document.getElementById(\"pino");
                            client.print(pinosAnalogicos[nL]);
                            client.print("\").innerHTML = \"Porta ");
                            client.print(pinosAnalogicos[nL]);
                            client.print(" - Valor: \" + this.responseText.substring(valPosIni, valPosFim);");
                            client.println("}");
                        }
                          
                        client.println("}}}}");
                        client.println("request.open(\"GET\", \"solicitacao_via_ajax\" + nocache, true);");
                        client.println("request.send(null);");
                        client.println("setTimeout('LeDadosDoArduino()', 1000);");
                        client.println("}");
                        client.println("</script>");
                        
                        client.println("</head>");

                        client.println("<body onload=\"LeDadosDoArduino()\">");                      //<------ALTERADO                    
                        client.println("<h1>PORTAS EM FUN&Ccedil;&Atilde;O ANAL&Oacute;GICA</h1>");

                        for (int nL=0; nL < qtdePinosAnalogicos; nL++) {

                            client.print("<div id=\"pino");                         //<----- NOVO
                            client.print(pinosAnalogicos[nL]);
                            client.print("\">"); 
                                                         
                            client.print("Porta ");
                            client.print(pinosAnalogicos[nL]);
                            client.println(" - Valor: ");
                               
                            client.print( analogRead(pinosAnalogicos[nL]) );
                            client.println("</div>");                               //<----- NOVO
                               
                            client.println("<br/>");                             
                        }
                        
                        client.println("<br/>");                        
                        client.println("<h1>PORTAS EM FUN&Ccedil;&Atilde;O DIGITAL</h1>");
                        client.println("<form method=\"get\">");

                        for (int nL=0; nL < qtdePinosDigitais; nL++) {
                            processaPorta(pinosDigitais[nL], nL, client);
                            client.println("<br/>");
                        }
                        
                        client.println("<br/>");
                        client.println("<button type=\"submit\">Envia para o ESP8266</button>");
                        client.println("</form>");                      
                        
                        client.println("</body>");
                        
                        client.println("</html>");

                    
                    } else if (HTTP_req.indexOf("solicitacao_via_ajax") > -1) {     //<----- NOVO

                        Serial.println(HTTP_req);

                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");      
                        client.println();                      

                        for (int nL=0; nL < qtdePinosAnalogicos; nL++) {
                            lePortaAnalogica(pinosAnalogicos[nL], nL, client);                            
                        }
                        for (int nL=0; nL < qtdePinosDigitais; nL++) {
                            lePortaDigital(pinosDigitais[nL], nL, client);
                        }
                            
                    } else {

                        Serial.println(HTTP_req);
                        client.println("HTTP/1.1 200 OK");
                    }
                    HTTP_req = "";    
                    break;
                }
                
                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            }
        } 
        delay(1);     
        client.stop(); 
    } 
}


void processaPorta(byte porta, byte posicao, WiFiClient cl)                                                                     //montar um trecho do HTML com o valor das portas 
{
static boolean LED_status = 0;
String cHTML;

    cHTML = "P";
    cHTML += porta;
    cHTML += "=";
    cHTML += porta;

    if (modoPinos[posicao] == OUTPUT) { 
        
        if (URLValue.indexOf(cHTML) > -1) { 
           LED_status = HIGH;
        } else {
           LED_status = LOW;
        }
        digitalWrite(porta, LED_status);
    } else {

        LED_status = digitalRead(porta);
    }
        
    cl.print("<input type=\"checkbox\" name=\"P");
    cl.print(porta);
    cl.print("\" value=\"");
    cl.print(porta);
    
    cl.print("\"");

    cl.print(" id=\"pino");           //<------NOVO
    cl.print(porta);
    cl.print("\"");
    
    if (LED_status) { 
        cl.print(" checked ");
    }

    if (modoPinos[posicao] != OUTPUT) { 
        cl.print(" disabled ");
    }
    
    cl.print(">Porta ");
    cl.print(porta);

    cl.println();
}




void lePortaDigital(byte porta, byte posicao, WiFiClient cl)                                                                                              //ler os valores das portas digitais, montando um pequeno HTML para compor o site 
{
    if (modoPinos[posicao] != OUTPUT) { 
       cl.print("PD");
       cl.print(porta);
       cl.print("#");
       
       if (digitalRead(porta)) {
          cl.print("1");
       } else {
          cl.print("0");
       }
       cl.println("|");
    }
}








void lePortaAnalogica(byte porta, byte posicao, WiFiClient cl)                                                                                          //ler as portas analogicas , msm coisa html para exibir valor
{
   cl.print("PA");
   cl.print(porta);
   cl.print("#");
   
   cl.print(analogRead(porta));

   //especifico para formatar o valor da porta analogica A0
   if (porta == A0) { 
      cl.print(" (");
      cl.print(map(analogRead(A0),0,1023,0,179)); 
      cl.print("&deg;)");
   }
   
   cl.println("|");   
}


String getURLRequest(String *requisicao) {
int inicio, fim;
String retorno;

  inicio = requisicao->indexOf("GET") + 3;
  fim = requisicao->indexOf("HTTP/") - 1;
  retorno = requisicao->substring(inicio, fim);
  retorno.trim();

  return retorno;
}

bool mainPageRequest(String *requisicao) {
String valor;
bool retorno = false;

  valor = getURLRequest(requisicao);
  valor.toLowerCase();

  if (valor == "/") {
     retorno = true;
  }

  if (valor.substring(0,2) == "/?") {
     retorno = true;
  }  

  if (valor.substring(0,10) == "/index.htm") {
     retorno = true;
  }  

  return retorno;
}
