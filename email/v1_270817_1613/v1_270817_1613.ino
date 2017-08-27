#include <EtherCard.h>
#include <Ultrasonic.h>

//Define os parametros para o sensor ultrasonico HC-SR04
#define PINO_TRIGGER  6 //Porta ligada ao pino Trigger do sensor
#define PINO_ECHO     7 //Porta ligada ao pino Echo do sensor
//Inicializa o sensor ultrasonico
Ultrasonic ultrasonic(PINO_TRIGGER, PINO_ECHO);
//Distancias em centimetros
float distancia_minima = 20;
float distancia_maxima = 60;

// ethernet interface mac address, must be unique on the LAN
static byte mac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte ip[] = { 192,168,0,20 };

byte Ethernet::buffer[500];
BufferFiller bfill;

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
  delay(1000);
}

void envia(){
  Serial.println("Pronto para Enviar");
}
