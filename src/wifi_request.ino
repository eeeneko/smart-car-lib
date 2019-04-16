
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

    // Connect to WiFi network


    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println('0');
        delay(500);
        Serial.println('1');
        delay(500);
    }

    // Start the server
    server.begin();

}



void loop() {
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return;
    }


    client.setTimeout(5000); // default is 1000

    // Read the first line of the request
    String req = client.readStringUntil('\r');
    static String res = "";

    // Match the request
    static char val = '0';
    int sent = 0;
    
    if (req.indexOf(F("/motor/stop")) != -1) {
        Serial.println('0');
        val = '0';sent = 1;
    } else if (req.indexOf(F("/motor/forward")) != -1) {
        Serial.println('1');
        val = '1';sent = 1;
    } else if (req.indexOf(F("/motor/backward")) != -1) {
        Serial.println('2');
        val = '2';sent = 1;
    } else if (req.indexOf(F("/motor/left")) != -1) {
        Serial.println('3');
        val = '3';sent = 1;
    } else if (req.indexOf(F("/motor/right")) != -1) {
        Serial.println('4');
        val = '4';sent = 1;
    }else if (req.indexOf(F("/motor/backleft")) != -1) {
        Serial.println('5');
        val = '5';sent = 1;
    }else if (req.indexOf(F("/motor/backright")) != -1) {
        Serial.println('6');
        val = '6';sent = 1;
    }else if (req.indexOf(F("/mode/auto")) != -1) {
        Serial.println('7');
        val = '7';sent = 1;
    }else if (req.indexOf(F("/mode/manul")) != -1) {
        Serial.println('8');
        val = '8';sent = 1;
    }else if (req.indexOf(F("/speed/0")) != -1) {
        Serial.println('9');
        val = '9';sent = 1;
    }else if (req.indexOf(F("/speed/1")) != -1) {
        Serial.println('a');
        val = 'a';sent = 1;
    }else if (req.indexOf(F("/speed/2")) != -1) {
        Serial.println('b');
        val = 'b';sent = 1;
    }else if (req.indexOf(F("/speed/3")) != -1) {
        Serial.println('c');
        val = 'c';sent = 1;
    }

    Serial.print(val);

    if(Serial.available() > 0) res = Serial.readString();




    while (client.available()) {

        client.read();
    }

    if (sent == 1){

      client.print(F("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" ));
    }else{

        client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><script>var preUrl=\"http://192.168.4.8\";document.onkeydown=function(event){var e=event||window.event||arguments.callee.caller.arguments[0];if(e&&e.keyCode==38){get(preUrl+\"/motor/forward\");document.getElementById(\"display\").innerHTML=\"前进\"}else{if(e&&e.keyCode==40){get(preUrl+\"/motor/backward\");document.getElementById(\"display\").innerHTML=\"后退\"}else{if(e&&e.keyCode==188){get(preUrl+\"/motor/backleft\");document.getElementById(\"display\").innerHTML=\"左后\"}else{if(e&&e.keyCode==190){get(preUrl+\"/motor/backright\");document.getElementById(\"display\").innerHTML=\"右后\"}else{if(e&&e.keyCode==37){get(preUrl+\"/motor/left\");document.getElementById(\"display\").innerHTML=\"左转\"}else{if(e&&e.keyCode==39){get(preUrl+\"/motor/right\");document.getElementById(\"display\").innerHTML=\"右转\"}else{if(e&&e.keyCode==49){get(preUrl+\"/mode/auto\");document.getElementById(\"mode\").innerHTML=\"自动驾驶\"}else{if(e&&e.keyCode==50){get(preUrl+\"/mode/manul\");document.getElementById(\"mode\").innerHTML=\"手动驾驶\"}else{if(e&&e.keyCode==65){get(preUrl+\"/speed/0\");document.getElementById(\"display\").innerHTML=\"速度：0\"}else{if(e&&e.keyCode==83){get(preUrl+\"/speed/1\");document.getElementById(\"display\").innerHTML=\"速度：1\"}else{if(e&&e.keyCode==68){get(preUrl+\"/speed/2\");document.getElementById(\"display\").innerHTML=\"速度：2\"}else{if(e&&e.keyCode==70){get(preUrl+\"/speed/3\");document.getElementById(\"display\").innerHTML=\"速度：3\"}else{if(e&&e.keyCode==32){get(preUrl+\"/motor/stop\");document.getElementById(\"display\").innerHTML=\"停止\"}}}}}}}}}}}}}};var xmlHttp;function get(url){xmlHttp=new XMLHttpRequest();xmlHttp.open(\"GET\",url,true);xmlHttp.send()};</script></head><body><div id=\"mode\"></div><div id=\"display\"></div></body></html>"));

    }
}