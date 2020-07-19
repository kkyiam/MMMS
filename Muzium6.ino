
#include <UbidotsESPMQTT.h> //include UbidotsESPMQTT.h library
#include "DHT.h" //https://github.com/adafruit/DHT-sensor-library
#include <ESP8266WiFi.h> //include ESP8266WiFi.h library
#define DHTPIN 5 //Pin D1
#define DHTTYPE DHT22 //Temp and Humidity Sensor that are using
#define TOKEN "BBFF-n4WpGJb8PExpSO4FudectNw2f6VeiN" // token for Ubidots 
#define WIFINAME "Musa_Wifi" //current SSID
#define WIFIPASS "mbk061112" //current Wifi password
const int led1 = 14; //declaring led1 pin
DHT dht(DHTPIN, DHTTYPE); //initialized dht sensors


Ubidots client(TOKEN); //function to set up for display in serial monitor

void callback(char* topic, byte* payload, unsigned int length) { //initialize topic name and size of data 
  Serial.print("Message arrived ["); //print in serial monitor
  Serial.print(topic); //print in serial monitor
  Serial.print("] "); //print in serial monitor
  for (int i=0;i<length;i++) { //loop for payload
    Serial.print((char)payload[i]); //print in serial monitor
  }
  Serial.println(); //print in serial monitor
}

void setup() { //function to setup connection with ubidots
  
  client.setDebug(true); //set debug to true
  Serial.begin(115200); //set speed parameter at 115200 bits per second
  client.wifiConnection(WIFINAME, WIFIPASS); //connect to local wifi
  client.begin(callback); //start the client
  
  Serial.println("");
  Serial.println("WiFi connected");  //show wifi is connected
  Serial.println("IP address: "); //show the IP address
  Serial.println(WiFi.localIP()); //IP address of the shield

  dht.begin(); //start dht22 sensor
  pinMode(led1, OUTPUT); //innitialize led1 pin as output
}

void loop() { //function to  measure variable and send data to Ubidots


  float h = dht.readHumidity(); //Humidity in %
  float t = dht.readTemperature(); //Temp in Celsius
  float lux =  analogRead(A0); //Lux 
  if (isnan(h) || isnan(t)) { //Did the sensor read fail?
    Serial.println("Failed to read from DHT sensor!"); //print sensor failed
 
  }
  
  Serial.print("Humidity: "); //show humidity
  Serial.print(h);    //show humidity level measured
  Serial.print("   Temperature: "); //show temperature
  Serial.println(t);  //show temperature level measured
  Serial.print("      Lux: "); //show lux
  Serial.print(lux);   //show lux level measured
  
  if (lux>900 || t>26 || h>60){ //set the conditions for each variable
    digitalWrite(led1, HIGH); //turn on LED
    Serial.print("        Led ON"); //print in serial monnitor
    }
   else {
    digitalWrite(led1, LOW); //turn off LED
    Serial.print("        Led OFF"); //print in serial monnitor
   }
  
   
  if(!client.connected()){ //condition when client not connected
      client.reconnect(); //reconnecct with client
  }

  client.add("temperature", t); //send t to Ubidots
  client.add("humidity", h); //send h to Ubidots
  client.add("Lux", lux); //send lux to Ubidots
  client.ubidotsPublish("Temp_Humidity_SmallGH"); //show the value in Ubidots
  client.loop(); //loop back the function

  delay(300000); //delay by 5 minutes
  
}
