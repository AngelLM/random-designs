/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "MOVISTAR_CAC7";
const char* password = "ePwqkqUcQJCK6CaYhcbq";

ESP8266WebServer server(80);

#define IN1  D1
#define IN2  D2
#define IN3  D5
#define IN4  D6

//int LEDPin = 13;

bool motorOn=false;
bool AllDisabled;
String dir="", mov="";
unsigned long stepsToDo=0, stepsDone=0, stepDelay=0;

int paso[4][4] =
{
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
};

String page = "";

void setup() {
  page = "<!DOCTYPE HTML><html>  <head>    <meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, ser-scalable=0\">    <title>Spining Platform</title>    <style>@import url(https://fonts.googleapis.com/css?family=Space+Mono);    html,body{      height: 95.5%;      background:#f4f7f7;      color:#34495e;      font-family: Arial, Helvetica, Sans-Serif;}    #title{font-family: 'Space Mono',serif;      font-weight: bold;      margin-top: -5px;      text-align: center;      font-size: 30px;      height: 50px;}    #central_section{      text-align: center;      float: center;      width: 400 px;      margin: 0 auto;      padding:10px}    .footer{      position: relative;      padding: 1px;      clear:both;      background:#5da0a2;      text-align:center;      color:#e4f9f5;}    .wrapper{      min-height: 100%;      height: auto !important;      height: 100%;      margin: 0 auto -2em;}    .textInput{      width: 50px;      margin-right:15px}    .button {      background-color: #34495e;      border: none;      color: white;      padding: 15px 32px;      text-align: center;      text-decoration: none;      font-weight: bold;      display: inline-block;      font-size: 16px;      margin:15px;      border-radius:10px;    }    a{      color:#34495e;    }    </style>  </head>  <body>    <div class=\"wrapper\">      <div id=\"central_section\">      <p id = \"title\">Spinning Platform</p>      <FORM action=\"/\" method=\"post\">        <P><b>Revolutions</b>  <INPUT class=\"textInput\" type=\"text\" name=\"revolutions\"><b>Time (min)</b>  <INPUT class=\"textInput\" type=\"text\" name=\"time\"></P>        <P><b>Direction</b>  <input type=\"radio\" name=\"direction\" value=\"cw\" checked>CW<input type=\"radio\" name=\"direction\" value=\"ccw\">CCW<br>        <P><b>Movement</b>  <input type=\"radio\" name=\"movement\" value=\"continuous\" checked>Continuous<input type=\"radio\" name=\"movement\" value=\"once\">Just once<br>        <P><INPUT class=button type=\"submit\" value=\"Spin\">        <INPUT class=button type=\"button\" value=\"Stop\"></P>      </FORM>      </div>    </div>    <div class=\"footer\">      <p id=\"Footer_Text\">Spinning Platform by <a href=\"http://www.angel-lm.com\">Angel L.M.</a> - 2018</p>    </div>  </body></html>";
  
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Web server started!");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  AllDisabled=true;
}


void handleMotor()
{
  if(motorOn){
    if(stepsDone>=stepsToDo){
      motorOn=false;
      }
    if(dir=="ccw" && stepsToDo>stepsDone){
        for (int i = 0; i < 4; i++)
        {
          digitalWrite(IN1, paso[i][0]);
          digitalWrite(IN2, paso[i][1]);
          digitalWrite(IN3, paso[i][2]);
          digitalWrite(IN4, paso[i][3]);
          delay(stepDelay);
          if (mov=="once"){
            stepsDone++;
            }
        }
      }
      
      else if(dir=="cw" && stepsToDo>stepsDone){
        for (int i = 3; i >= 0; i--)
        {
          digitalWrite(IN1, paso[i][0]);
          digitalWrite(IN2, paso[i][1]);
          digitalWrite(IN3, paso[i][2]);
          digitalWrite(IN4, paso[i][3]);
          delay(stepDelay);
          if (mov=="once"){
            stepsDone++;
            }
          }
        }
      }
  else{
//    if (!AllDisabled){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
//      AllDisabled=true;
//    }
  }

}



void handleRoot()
{
  if (server.hasArg("revolutions")&&server.hasArg("time")) {
    handleSubmit();
  }
  else {
//    Serial.println("nope");
    server.send(200, "text/html", page);
  }
}

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void handleSubmit()
{
  String stringRevs, stringTime;
  float revs, time;

  if (!server.hasArg("revolutions")||!server.hasArg("time")) return returnFail("BAD ARGS");
  stringRevs = server.arg("revolutions");
  stringTime = server.arg("time");
  dir = server.arg("direction");
  mov = server.arg("movement");
  revs = stringRevs.toFloat();
  time = stringTime.toFloat();
  spinIt(revs,time);
//  Serial.println("done");
  server.send(200, "text/html", page);
}

void returnOK()
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}

void spinIt(float revs, float time) {
  if(revs!=0 and time!=0){    
    motorOn=true;
    AllDisabled=false;
    stepsToDo=revs*2048;
    stepsDone=0;
    stepDelay=(60000*time/stepsToDo);
    if (stepDelay<10){stepDelay=10;}
  }
  else{
    motorOn=false;
    }

}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}





void loop() {
  server.handleClient();
  handleMotor();
}
