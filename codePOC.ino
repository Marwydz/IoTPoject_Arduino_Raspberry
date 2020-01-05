
#include <Bridge.h>
#include <HttpClient.h>
const int sensorPin=A2;
double sensorValue=0;
const int AOUTpin=3;//the AOUT pin of the CO sensor goes into analog pin A3 of the arduino
const int DOUTpin=8;//the DOUT pin of the CO sensor goes into digital pin D8 of the arduino
const int ledPin=11;//the anode of the LED connects to digital pin D13 of the arduino

int limit;
int value;


void setup() {

  Serial.begin(9600);
  pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
  pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino

  pinMode(sensorPin,INPUT);
  // Bridge takes about two seconds to start up
  // it can be helpful to use the on-board LED
  // as an indicator for when it has initialized
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  SerialUSB.begin(9600);
  while (!SerialUSB); // wait for a serial connection
}

void loop() {
  sensorValue = analogRead (sensorPin);
  sensorValue = sensorValue;
  sensorValue=((sensorValue*10)-500)/45;
  Serial.println(sensorValue);
  delay(1000); 
  
  value= analogRead(AOUTpin);//reads the analaog value from the CO sensor's AOUT pin
  limit= digitalRead(DOUTpin);//reads the digital value from the CO sensor's DOUT pin
  Serial.print("CO value: ");
  Serial.println(value);//prints the CO value
  Serial.print("Limit: ");
  Serial.print(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
  delay(100);
  digitalWrite(ledPin, LOW);
  if (value >= 200){
  digitalWrite(ledPin, HIGH);//if limit has been reached, LED turns on as status indicator
  }

  //prise de decision
  int LED; // va renvoyer la decision prise 
  double tempmin= 22;
  double fummin = 185;
  //les valeurs sont recuperees par les capteurs precedemment
  if(sensorValue <= tempmin){
    LED = 0;
  }
  else{
    if(value <= fummin){
      LED = 0;
    }
    else{
      LED = 1;
    }
  }
  
  

  
  // Initialize the client library
  HttpClient client;
  String raspberry = String("192.168.43.59");
  String page = String("/Button.php");
  String chemin = String("http://");
  double temperature = sensorValue;
  double fumee = value;
  chemin = chemin+raspberry;
  chemin = chemin+page;
  chemin = chemin+String("?temperature=");
  chemin = chemin+temperature;
  chemin = chemin+String("&fumee=");
  chemin = chemin + fumee;
  chemin = chemin +String("&decision=");
  chemin = chemin + LED;
  Serial.println(chemin);
  // Make a HTTP request:
  client.get(chemin);
  

  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    SerialUSB.print(c);
  }
  SerialUSB.flush();

  delay(20000);

}
