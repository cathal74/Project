#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <HTTPClient.h>
  
//Connect to my wifi
const char* ssid = "VM55624D8";
const char* password = "b3kuQpvmuxmw";

const char *host = "192.168.0.18"; //Your pc ip

#define DHTPIN 27     // Gpio pin 27
#define DHTTYPE    DHT11     // DHT 11


//Instantiate a DHTobject with the type and pin defined earlier.
DHT dht(DHTPIN, DHTTYPE);


// Creates AsyncWebServer object on port 80
AsyncWebServer server(80);


//Two functions to read the data from dht11
String readDHTTemperature() {
  float t = dht.readTemperature(); // Read temperature as Celsius 
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {    //Error if not a number
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1"> //makes web page responsive in any browser
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: centre;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Green House Weather Updates</h2>
  <p>
    
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    
  </p>
  <p>
    
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    
  </p>
</body>
<script> //some JavaScript code  that updates the temperature and humidity automatically, every 10 seconds.
setInterval(function ( ) {  //initializes a newly-created request in the temperature URL to get the latest temperature reading. When it receives that value, it updates the HTML element whose id is temperature.
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;  //allows to display Temperature
    }
  };
  xhttp.open("GET", "/temperature", true); 
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;  //allows to display humidity
    }
  };
  xhttp.open("GET", "/humidity", true); //initializes a newly-created request 
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// processor() function, that replaces the placeholders in our HTML text with the actual temperature and humidity values.
String processor(const String& var){
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  dht.begin(); //starts sensor
  
  // Connects to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){ //receive a request on the /temperature URL,sends the updated temperature value.
    request->send_P(200, "text/plain", readDHTTemperature().c_str()); //sends as a char
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
  HTTPClient http;    //Declare object of class HTTPClient
 
  String TempValueSend, postData;
  int tempvalue = dht.readTemperature();  //Read value of DHT
  TempValueSend = String(tempvalue);   //String to interger conversion
 
  //Posts Data
  postData = "tempvalue=" + TempValueSend;
  
  http.begin("http://192.168.0.18/Nodemcu_db_record_view/InsertDBB.php");              //request destination, link to php
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header, the media type
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  //Serial.println("Temp Value=" + tempvalue);
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  Serial.println("tempvalue=" + TempValueSend);
  
  http.end();  //Close connection

  delay(9000);  //9 second delay between readings for sake of demo
  
  
}
