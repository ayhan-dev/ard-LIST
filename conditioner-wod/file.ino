#include <ESP8266WiFi.h>

const char* ssid     = "xxxx";
const char* password = "xxxxx";

WiFiServer server(80);
String header;
int app11 = D1;
int app22 = D2; 
String app11state = "off";
String app22state = "off";

void setup() {
  Serial.begin(115200);
  pinMode(app11, OUTPUT);
  pinMode(app22, OUTPUT);
  digitalWrite(app11, LOW);
  digitalWrite(app22, LOW);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();  
  if (client) {                             
    String currentLine = "";                
    while (client.connected()) {          
      if (client.available()) {        
        char c = client.read();         
        Serial.write(c);     
        header += c;
        if (c == '\n') {        
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (header.indexOf("GET /app1/on") >= 0) {
              Serial.println("App 1 on");
              app11state = "on";
              digitalWrite(app11, HIGH);
            } else if (header.indexOf("GET /app1/off") >= 0) {
              Serial.println("App 1 off");
              app11state = "off";
              digitalWrite(app11, LOW);
            } else if (header.indexOf("GET /app2/on") >= 0) {
              Serial.println("App 2 on");
              app22state = "on";
              digitalWrite(app22, HIGH);
            } else if (header.indexOf("GET /app2/off") >= 0) {
              Serial.println("App 2 off");
              app22state = "off";
              digitalWrite(app22, LOW);
            }
       
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            client.println("<body><h1>ESP8266 Web Server</h1>");
            client.println("<p>app1 - State " + app11state + "</p>");
            if (app11state == "off") {
              client.println("<p><a href=\"/app1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/app1/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            client.println("<p>app2 - State " + app22state + "</p>");
            if (app22state == "off") {
              client.println("<p><a href=\"/app2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/app2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;   
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
