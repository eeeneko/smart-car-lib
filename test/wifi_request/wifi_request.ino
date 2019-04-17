#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "yimian-iot"
#define STAPSK  "1234567890."
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(12); 

    // Connect to WiFi network


    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Disconnected!");
        delay(500);
        Serial.println("Disconnected!");
        delay(500);
    }

    // Start the server
    server.begin();

}




void loop() {

    static String res = "";
    static String to = "";
    static unsigned long t= millis();

    if(millis() > t + 25){
        if(Serial.available() > 0) to = Serial.readStringUntil('\n');
        Serial.println(res);
        t = millis();
    }

    

    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return;
    }


    client.setTimeout(5000); // default is 1000

    // Read the first line of the request
    String req = client.readStringUntil('\r');

    char res_char[8];

    if (req.indexOf(F("/cmd/")) != -1){

        const char *t_req = req.c_str();
        res = "";
        for(int i = 0; i < 8; i++) res_char[i] = t_req[10+i];
        res = res_char;
    }


    while (client.available()) {

        client.read();
    }

    client.print(F("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n" ));
    client.print(to);

}
