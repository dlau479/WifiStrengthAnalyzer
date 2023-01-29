#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

bool wArrayfull = false;
int len = 0; // used for array size and also to get rid of old wifi numbers
int indexcount = 0; //counts the amount of valid numbers for wifi strength
int wArray[10]; // used to keep track of arrays. I put size 10 as I felt that it was a good number to use for gathering data
int wmean = 0; // Used to represent the mean value
int wmin = 0; // Used to represent the least or weakest wifi signal
int wmax = -100; // Used to represent the strongest wifi signal

WiFiServer server(80); 

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
  Serial.println("Wait one moment while the board is connecting");
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    status = WiFi.begin(ssid, pass);
    // wait 5 seconds for connection as this gives the machine time to find the wifi and make a good connection
    delay(5000);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          long w = WiFi.RSSI();
          // send a standard http response header
          client.println("HTTP/1.2 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");         // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          client.println("List of wifi strengths: ");
          //filling up wifi array
          if (len < 9) {
            wArray[len] = w;
            len++;
            wmean = 0;
            for (int i = 0; i < 10; i++) {
              if(wArray[i] != 0){
                client.print(" ");              
                client.print(wArray[i]);
                for(int i = 0 ; i < len; i++){ //this for loop is used to periodically get the minimum value
                  if(wArray[i] < wmin){ 
                    wmin = wArray[i];
                  }
                }
                if(wmax < wArray[i]){ // This is used to get the maximum value
                  wmax = wArray[i];
                }
                wmean += wArray[i]; // as the loop goes. This will start adding the mean value.
                indexcount++; // This is used to keep track of how many valid numbers are going to used in the calculation
              }
            }
          } else {
            len = 0;
            wArray[len] = w;
            len++;
            wmean  = 0;
            for (int i = 0; i < 10; i++) {
              if(wArray[i] != 0){
                client.print(" ");
                client.print(wArray[i]);
                for(int i = 0 ; i < len; i++){//this for loop is used to periodically get the minimum value
                  if(wArray[i] < wmin){
                    wmin = wArray[i];
                  }
                }
                if(wmax < wArray[i]){ // This is used to get the maximum value
                  wmax = wArray[i];
                }
                wmean += wArray[i]; // as the loop goes. This will start adding the mean value.
                indexcount++; // This is used to keep track of how many valid numbers are going to used in the calculation
              }
            }
          }
          wmean /= indexcount; // This is the final calculation of the mean
          indexcount = 0; // This will reset the valid number of indexs in the array and be recalculated agian
          client.println();
          client.print("Min: ");
          client.println(wmin);
          client.print("Max: ");
          client.println(wmax);
          client.print("Mean:");
          client.println(wmean);
          client.print("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


void printWifiStatus() {

  IPAddress ip = WiFi.localIP(); // This is used to help guide the user to connect to this address
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println("You may connect now");// This prompt helps the user that they may now connect.
}