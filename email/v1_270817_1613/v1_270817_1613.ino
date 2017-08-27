
#include <EtherCard.h>
#include <Ultrasonic.h>
#include <EthernetClient.h>
#define time 1000

//Define os parametros para o sensor ultrasonico HC-SR04
#define PINO_TRIGGER  6 //Porta ligada ao pino Trigger do sensor
#define PINO_ECHO     7 //Porta ligada ao pino Echo do sensor
//Inicializa o sensor ultrasonico
Ultrasonic ultrasonic(PINO_TRIGGER, PINO_ECHO);
//Distancias em centimetros
float distancia_minima = 20;
float distancia_maxima = 70;

// ethernet interface mac address, must be unique on the LAN
static byte mac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte ip[] = { 192,168,0,20 };
char server[] = "mail.smtp2go.com";

byte Ethernet::buffer[100];
BufferFiller bfill;

EthernetClient client;

void setup() {
//INICIO CONEXAO COM A WEB
  if(ether.begin(sizeof Ethernet::buffer, mac) == 0) {
    Serial.println(F("falha ao configurar Ethernet usando DHCP"));
    ether.staticSetup(ip);
  }  
  Serial.begin(9600);
}

void loop() {
 sensorReader();
 
}

void sensorReader(){
  //Variaveis para guardar os valores em 
  //cm (cmSec) e polegadas (inMsec)
  float cmMsec, inMsec;
  
  //Le os valores do sensor ultrasonico
  long microsec = ultrasonic.timing();
  //Atribui os valores em cm ou polegadas as variaveis
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);

  if (cmMsec > distancia_minima && cmMsec < distancia_maxima)
  {
    envia();
  }
  delay(2000);
}

void envia(){
  delay(time);
  Serial.println("conectando...");
  if (client.connect(server, 2525)) {
    Serial.println("connected");
    // Make your API request:
    client.println("Host: api.smtp2go.com");
    client.println("Content-Type: application/json");
    client.println("\n{\n    \"api_key\": \"api-5AD8C9328B7A11E7BD9BF23C91C88F4E\",\n    \"to\": [\"Lucas Ferrari <lucas.ferrari@outlook.com>\"],\n    \"sender\": \"Arduino Alert <ruferrari@hotmail.com>\",\n    \"subject\": \"Alerta de movimento\",\n    \"text_body\": \"Movimento suspeito detectado.\"\n}\n");
    client.println("Connection: close");
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void recebe(){
  while (client.available()){
    char conteudo = client.read();
    Serial.print(conteudo);
  }
}
