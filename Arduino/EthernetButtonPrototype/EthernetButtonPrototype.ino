#include <Bounce.h>

/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 modified 9 Apr 2012
 by David A. Mellis
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress server(192,168,1,100); // house server

const int buttonPinA = 3;     // the number of the pushbutton pin
const int buttonPinB = 4;     // the number of the pushbutton pin

char server[] = "house.wohlershome.net";
int  port = 8080;

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
boolean lastConnected = false;                    // state of the connection last time through the main loop

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 5;

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

Bounce bouncerA = Bounce( buttonPinA,5 ); 
Bounce bouncerB = Bounce( buttonPinB,5 ); 

int updateA = 0;
int updateB = 0;

String result = "";

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);

  //define input pins
  pinMode(buttonPinA, INPUT);
  pinMode(buttonPinB, INPUT);
  pinMode(led, OUTPUT);     

}

void loop()
{  
  // update debouncer
  updateA = bouncerA.update();
  updateB = bouncerB.update();
 
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    //Serial.print(c);
    result = result + c;
    // Delete HTTP headers
    if(result.endsWith("Content-Type: application/json"))
    {
      result = "";
    }
    
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {

    Serial.println(result);
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
 
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
 
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if ( updateA && bouncerA.read() == HIGH) {     
    sendData(1);
    Serial.println( "Button 1 Pressed" );

  }
  if ( updateB && bouncerB.read() == HIGH) {     
    sendData(2);
    Serial.println( "Button 2 Pressed" );

  }

  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();

}

void sendData(int thisData) {
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW

  // if there's a successful connection:
  if (client.connect(server, port)) {
    Serial.println("connected");
    // Make a HTTP request:
    // See http://arduino.cc/en/Tutorial/PachubeClient for more
//    client.println("POST /test.php HTTP/1.1");
    client.println("POST /LibraryInteractions/post.php HTTP/1.1");
    client.println("Host: house.wohlershome.net");
    client.println("User-Agent: LibraryStat/DJ01-0000001");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    
    int thisLength = 7 + getLength(thisData);
    client.println(thisLength);    
    
    client.println("Connection: close");
    client.println();
    client.print("button=");
    client.println(thisData);


  }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
}

// This method calculates the number of digits in the
// sensor reading.  Since each digit of the ASCII decimal
// representation is a byte, the number of digits equals
// the number of bytes:

int getLength(int someValue) {
  // there's at least one byte:
  int digits = 1;
  // continually divide the value by ten, 
  // adding one to the digit count for each
  // time you divide, until you're at 0:
  int dividend = someValue /10;
  while (dividend > 0) {
    dividend = dividend /10;
    digits++;
  }
  // return the number of digits:
  return digits;
}
