void setup() 
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}
void loop()                                                                             
{
    static String res = "";
    static String to = "hi~ This is from Smart Car";

    if(Serial.available() > 0){
        res = Serial.readString();
        if(res!="Disconnected!")
        {to += res;
        Serial.println(to);}
    }
    
}


