/*
  Web Server
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi
 Besides that, this project was modified by Matheus Vrech and Bruna Zamith for university classes purpose, enjoy it
 */

#include <SPI.h>
#include <Stepper.h>
#include <Ethernet.h>
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int pinButton = 4;
int counter = 0;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(200, 18, 97, 11);
EthernetServer server(80);

/* Motor definitions */
const int stepsPerRevolution = 200;  
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

int _direction = 1;
boolean _running = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  /* set up the LCD's number of columns and rows: */
  lcd.begin(16, 2);
  lcd.setRGB(186, 85, 211);
  
  /* Print a message to the LCD. */
  lcd.print("Press B or V!");

  /* Define a led to use */
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinButton, INPUT);
}


void loop() {
  //myStepper.step(1);
  if (digitalRead(pinButton)) {
    counter++;
  }

  if (counter > 0){
    if (counter % 2 == 0) {
        display_lcd("Bruna Zamith", "628093");
    } else {
        display_lcd("Matheus Vrech", "727349");
    }
  }
  
  if(_running == 1)
    myStepper.step(_direction * 20);
  
  EthernetClient client = server.available();
  /* Get the http packet */
  String readString; 
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
         Serial.write(c);
         
        /* Store the entire packet */
        if (readString.length() < 100) {
          readString += c;
        } 

        /* Receive GET commands */
        if (readString.indexOf("lighton") > 0) {
          digitalWrite(LED_BUILTIN, HIGH);
          client.println("HTTP/1.1 200 OK");
          break;
        }
        else if (readString.indexOf("lightoff") > 0) {
          digitalWrite(LED_BUILTIN, LOW);
          client.println("HTTP/1.1 200 OK");
          break;
        }
        else if (readString.indexOf("bruna") > 0) {
          display_lcd("Bruna Zamith", "628093");
          client.println("HTTP/1.1 200 OK");
          break;
        }
        else if (readString.indexOf("vrechson") > 0) {
          display_lcd("Matheus Vrech", "727349");
          client.println("HTTP/1.1 200 OK");
          break;
        }
        /* working with our motor right now */
        else if (readString.indexOf("slow") > 0) {
          myStepper.setSpeed(10);
          client.println("HTTP/1.1 200 OK");
          break;
        }
        else if (readString.indexOf("medium") > 0) {
          myStepper.setSpeed(40);
          client.println("HTTP/1.1 200 OK");
          break;
        }
        else if (readString.indexOf("fast") > 0) {
          myStepper.setSpeed(75);
          client.println("HTTP/1.1 200 OK");
          break;
        }
        else if (readString.indexOf("stop") > 0) {
          _running = 0;
          client.println("HTTP/1.1 200 OK");
          break;
        }
        else if (readString.indexOf("90") > 0) {
          myStepper.step(_direction * 27);
          _running = 0;
          client.println("HTTP/1.1 200 OK");
          break;
        }             
        else if (readString.indexOf("180") > 0) {
          myStepper.step(_direction * 55);
          _running = 0;
          client.println("HTTP/1.1 200 OK");
          break;
        }
        else if (readString.indexOf("rotate") > 0) {
          _running = 1;
          client.println("HTTP/1.1 200 OK");
          break;
        }
        else if (readString.indexOf("reverse") > 0) {
          _direction = _direction ? -1 : 1;
          client.println("HTTP/1.1 200 OK");
          break;
        }
        /* end of motor functions */                                                   
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: keep-alive"); 
          //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head></head>");
          /* easter egg for ninjas */
          client.println("<body>i'm a happy arduino, and you found my easter egg!<br>curiosity will kill you anyway.</body>");
          client.println("</html>");
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    /* wait for something: nothing at all*/
    delay(1);
    //client.stop();
    //Serial.println("client disconnected");
  }
}

void display_lcd(String nome, String ra){
        lcd.clear();
        if(nome.indexOf("Zamith") > 0){
          lcd.setRGB(255, 127, 80);
        }
        else if(nome.indexOf("Vrech") > 0){
          lcd.setRGB(135, 206, 235);
        }
        lcd.setCursor(0, 0);
        lcd.print(nome);
        lcd.setCursor(0,1);
        lcd.print(ra);
        delay(300);
}
