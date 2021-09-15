
/********************************************************
 * 
 * bezzera-bb005-digital-timer based on ESP8266
 *   
 *****************************************************/

 
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Wire.h>
#include <sstream>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//===============================================================
//SSID and Password to connect to your ESP Access Point
//===============================================================
const char* ssid = "Grinder-Configuration";
const char* password = "12345678";

ESP8266WebServer server(80); //Server on port 80

String cup1;
String cup2;

bool servermode = false;
int serverLed = 16;
int clientLed = 2;



//===============================================================
//     SETUP
//===============================================================
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  EEPROM.begin(512);
  u8g2.begin();
  
  pinMode(serverLed, OUTPUT);
  pinMode(clientLed, OUTPUT);
  digitalWrite(serverLed, HIGH); // turn the LED off 
  digitalWrite(clientLed, HIGH); // turn the LED off 
  
  pinMode(10, INPUT_PULLUP);  // grind
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  

  if (digitalRead(10) == LOW) {servermode = true;}

  if (servermode){
    //===============================================================
    //     SERVERMODE
    //===============================================================
    Serial.println("servermode-setup");

    WiFi.mode(WIFI_AP);           //Only Access point
    WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
   
    IPAddress myIP = WiFi.softAPIP(); //Get IP address
    Serial.print("HotSpt IP: ");
    Serial.println(myIP);
   
    server.on("/", handleRoot);      //Which routine to handle at root location
    server.on("/action_page", handleForm); //form action is handled here
    server.on("/reset_page", handleReset); //form action is handled here    
   
    server.begin();                  //Start server
    Serial.println("HTTP server started");

    digitalWrite(serverLed, LOW); //LED on!

    std::string ssid2(ssid);
    std::string password2(password);

    u8g2.setFont(u8g2_font_crox1h_tf);
    u8g2.drawStr(0,10,"Servermode");
    u8g2.drawStr(0,23,myIP.toString().c_str());
    u8g2.setCursor(0, 36);
    u8g2.print("ssid: ");
    u8g2.print(ssid2.c_str());
    u8g2.setCursor(0, 49);
    u8g2.print("password: ");
    u8g2.print(password2.c_str());    
    u8g2.sendBuffer();
    
  } else {
    //===============================================================
    //     CLIENTMODE
    //===============================================================

    Serial.println("clientmode-setup");
    WiFi.mode(WIFI_OFF);
    
    //Read strings from eeprom
    cup1=readEprom(0, 100);   
    cup2=readEprom(100, 200);
  
    greeting();  

  }

}

//===============================================================
//     LOOP
//===============================================================
void loop() {

  // put your main code here, to run repeatedly:


  if (servermode){
    //===============================================================
    //     SERVERMODE
    //===============================================================

    server.handleClient();          //Handle client requests
    
  } else {
    
    int number;

    if (digitalRead(10) == LOW)
    {
      delay(300);
      u8g2.setFont(u8g2_font_inb27_mf);
      u8g2.drawStr(9,40,"1");
      u8g2.setFont(u8g2_font_8x13B_tf);
      u8g2.drawStr(9,55,"Cup");
      u8g2.sendBuffer();
      number=1;

      while (digitalRead(10) == LOW){
        yield();
      }
      
      unsigned long mytime;
      mytime = millis(); 

      while (millis()-mytime<1000){
        yield();
        if (digitalRead(10) == LOW)
        { 
          number=2;
          u8g2.setFont(u8g2_font_inb27_mf);
          u8g2.drawStr(9,40,"2");
          u8g2.setFont(u8g2_font_8x13B_tf);
          u8g2.drawStr(9,55,"Cups");
          u8g2.sendBuffer();
          while (digitalRead(10) == LOW){
            yield();
          }
          delay(1000);
          break;
        }
      }
      
      Serial.print("Number: ");
      Serial.println(number);
      if (number==1){
        grind(cup1.toInt(), 1);
      } else if (number==2){
        grind(cup2.toInt(), 2);  
      }
      
      
    }
    else
    {
      doreset();
    }

  }
}

//===============================================================
//     GREETING
//===============================================================
void greeting(){

  // Greeting display
  
  u8g2.setFont(u8g2_font_6x13B_tf);
  u8g2.drawStr(0,10,"Coffee Grind Timer");
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(100,30,"V1.0");
  u8g2.sendBuffer();

  u8g2.setCursor(15, 45);
  u8g2.print("  cup1: ");
  u8g2.print(cup1.c_str());

  u8g2.setCursor(15, 55);
  u8g2.print("  cup2: ");
  u8g2.print(cup2.c_str()); 
  u8g2.sendBuffer(); 
  
  delay(3000);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  
}

//===============================================================
//     GRIND
//===============================================================
void grind(int millisec, int cups) {

  String zeit=String(float(millisec)/1000);

  u8g2.clearBuffer();
  u8g2.sendBuffer();

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setCursor(5, 10);
  u8g2.print(zeit.c_str());
  u8g2.print(" sec");
  
  u8g2.setFont(u8g2_font_7x13_tf);
  u8g2.setCursor(5, 23);
  u8g2.print("Cups to grind: ");
  u8g2.print((String(cups)).c_str());
  u8g2.sendBuffer(); 


  String temp;
  unsigned long mytime;
  mytime = millis();  

  while (millis()-mytime<millisec){
    yield();
    digitalWrite(12, LOW);
    float counter=(millis()-mytime)/1000.00;

    if (counter<=9.99){
      temp=" "+String(counter);
    } else {
      temp=String(counter);
    }
    
    u8g2.setFont(u8g2_font_inb27_mf);
    u8g2.drawStr(9,60,temp.c_str());
    u8g2.sendBuffer(); 

    //Abort?
    if (digitalRead(10) == LOW)
    {
      doreset();
      u8g2.setFont(u8g2_font_8x13B_tf);
      u8g2.drawStr(40,40,"Aborted!");
      u8g2.sendBuffer(); 
      delay(2500);
      return;
    }
                
  }

  //Done!
  doreset();

  u8g2.setFont(u8g2_font_6x13B_tf);
  u8g2.drawStr(4,30,"Done...");
  u8g2.drawStr(4,55,"Enjoy your coffee!");
  u8g2.sendBuffer();
  delay(5000);
}

//===============================================================
//     Relay off - Clear Display Text
//===============================================================
void doreset(){
  digitalWrite(12, HIGH);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
}


//===============================================================
//     Write string to eeprom
//===============================================================
void writeEprom(String val, int von){
  for(int i=von;i<val.length()+von;i++) //loop upto string lenght
  {
    EEPROM.write(0x0F+i,val[i-von]); //Write one by one with starting address of 0x0F
  }
}

//===============================================================
//     Read string from eeprom
//===============================================================
String readEprom(int von, int bis){
    String val = "";
    for(int i=von;i<bis;i++) 
    {
      val = val + char(EEPROM.read(0x0F+i)); //Read one by one with starting address of 0x0F    
    }
    int position = val.indexOf("#|#|");
    val = val.substring(0, position);

    return val;
}

//==============================================================
//     This is the webpage for the Server Mode
//==============================================================
const char MAIN_page[] = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <style>
    html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
    body{margin-top: 50px;}
    h1 {margin: 50px auto 30px;}
    h3 {margin-bottom: 50px;}
    p {font-size: 20px;margin-bottom: 10px;}
    input {font-size: 20px;}
  </style>
</head>

<body>
  <h1>Grinder-Configuration</h1>
  <h3>Please Setup your Time:</h3>
 
  <form action="/action_page">
    <p>All Parameters in Milliseconds!<br>
    <br>
    <br>
    <p>Time for one Cup:<br>
    <input type="text" name="cup1" value="6400" size="10" maxlength="5">
    <br>
    <p>Time for two Cups:<br>
    <input type="text" name="cup2" value="9600" size="10" maxlength="5">
    <br>     
    <br>
    <input type="submit" value="Safe Data">
  </form> 
</body>
</html>
)=====";

//==============================================================
//     This ist the webpage two for the Server Mode
//==============================================================
const char ACTION_page[] = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <style>
    html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
    body{margin-top: 50px;}
    h1 {margin: 50px auto 30px;}
    h3 {margin-bottom: 50px;}
    input {font-size: 20px;}
  </style>
</head>

<body>
  <h1>Data safed</h1>
  <h3>Insert Data again:</h3>
 
  <form action="/">
    <input type="submit" value="Back">
  </form> 
    <br>
    <br>
  <h3>Restart Device with new Data:</h3>
  <form action="/reset_page">
    <input type="submit" value="Start">
  </form>
  
</body>
</html>
)=====";

//==============================================================
//     This ist the webpage after reset for the Server Mode
//==============================================================
const char RESET_page[] = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <style>
    html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
    body{margin-top: 50px;}
    h1 {margin: 50px auto 30px;}
    h3 {margin-bottom: 50px;}
  </style>
</head>

<body>
  <h1>RESET</h1>
  <h3>The Device is restarting know</h3>
</body>
</html>
)=====";

//==============================================================
//     This rutine is executed when you open its IP in browser
//==============================================================
void handleRoot() {
  server.send_P(200, "text/html", MAIN_page, sizeof(MAIN_page) ); //Send web page
}

//===============================================================
//     This routine is executed when you press submit
//===============================================================
void handleForm() {
  cup1 = server.arg("cup1") + "#|#|"; 
  cup2 = server.arg("cup2") + "#|#|"; 
  
  // #|#| Its for the Identifier at the end of the string
  
  EEPROM.begin(512);  //Initialize EEPROM
  writeEprom(cup1, 0);
  writeEprom(cup2, 100);
  EEPROM.commit();    //Store data to EEPROM
  EEPROM.end();
  server.send_P(200, "text/html", ACTION_page, sizeof(ACTION_page) ); //Send web page

}

//==============================================================
//     This rutine is executed when you restart
//==============================================================
void handleReset() {
  server.send_P(200, "text/html", RESET_page, sizeof(RESET_page) );
  Serial.println("Reset...");
  //delay(5000);
  blink(5, 10, 240, clientLed);
  blink(5, 10, 240, serverLed);
  ESP.restart(); //Reset esp!
}

//===============================================================
//     blink LED
//===============================================================
void blink(int count, int mytime, int mypause, uint32_t colour){
  for (int i = 0; i < count; i++){
    digitalWrite(colour, LOW); // turn the LED on 
    delay(mytime); // wait for the given time
    digitalWrite(colour, HIGH); // turn the LED off 
    delay(mypause); // wait for half a second 
  }
}
