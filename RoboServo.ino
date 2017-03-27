#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
int x1,x2,angle;
// Update these with values suitable for your network.
const char* ssid = "Network Ssid";
const char* password = "Network Password";
 const char* mqtt_server = "Mqtt IP address";
//const char* mqtt_server = "iot.eclipse.org";

Servo myservo;  // create servo object to control a servo
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{ int i;
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
  for( i=0;i<length;i++)
  {
    //if((int)payload[i]>194||(int)payload[i]<0)
    //break;
    myservo.write((int)payload[i]);
    //Serial.print((int)payload[i]);// tell servo to go to position in variable '(int)payload[i]'
  }
  //myservo.write((int)payload[i]);
  Serial.println(length);
  if (length==2){
    x1=(int)payload[0]-48;
    x2=(int)payload[1]-48;
  Serial.println(x1);
  Serial.println(x2);
  angle=x1*10+x2;
  Serial.println(angle);
  myservo.write(angle);
  
  }
  if (length==3){
    int x1=(int)payload[0]-48;
    int x2=(int)payload[1]-48;
    int x3=(int)payload[2]-48;
  Serial.println(x1);
  Serial.println(x2);
  Serial.println(x3);
  angle=x1*100+x2*10+x3;
  Serial.println(angle);
  myservo.write(angle);
  }
  //Serial.println((int)payload[2]-48);
  //
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("robo");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  myservo.attach(5);  // attaches the servo on pin D1 to the servo object
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
