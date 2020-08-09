#include <ESP8266WiFi.h>

const char* ssid = "WIFI SSID";
const char* password = "WIFI PASSWORD";

WiFiServer server(243);

bool Animation = false;
bool MusicMode = true;
int RedValue = 255;
int GreenValue = 255;
int BlueValue = 255;

int RedValue2;
int GreenValue2;
int BlueValue2;
int PredominantColorOne;
int PredominantColorTwo;
int LastPredominantColor;

float Delay;
float Delay2;
float TimeCounter;
long LastAnimationTime = 0;

String redString = "255";
String greenString = "255";
String blueString = "255";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;

String header;

const int redPin = 13;     
const int greenPin = 12;   
const int bluePin = 14;    

const int resolution = 256;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;
unsigned long LastChanged = 0;

String ClientIP = "";
String LastPerson = "Dexter";
String ListeningToMusic = "";

void setup() 
{
  Serial.begin(115200);
  
  analogWriteRange(resolution);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();   

  if(Animation == true and (MusicMode == true or Delay2 == 0) and millis() >= LastAnimationTime + Delay2)
  {        
    do
    {
      PredominantColorOne = random(0, 3);
    }    
    while(PredominantColorOne == LastPredominantColor);
    
    PredominantColorTwo = random(0, 2);
    
    if(PredominantColorOne == 0)
    {
      RedValue2 = 255;
      LastPredominantColor = 0;
        if(PredominantColorTwo == 0)
        {
            GreenValue2 = random(0, 256);
            BlueValue2 = random(0, 50);
        }
        else
        {
            BlueValue2 = random(0, 256);
            GreenValue2 = random(0, 50);
        }
    }
    else if(PredominantColorOne == 1)
    {
        GreenValue2 = 255;
        LastPredominantColor = 1;
        if(PredominantColorTwo == 0)
        {
            RedValue2 = random(0, 256);
            BlueValue2 = random(0, 50);
        }
        else
        {
            BlueValue2 = random(0, 256);
            RedValue2 = random(0, 50);
        }
    }
    else
    {
        BlueValue2 = 255;
        LastPredominantColor = 2;
        if(PredominantColorTwo == 0)
        {
            RedValue2 = random(0, 256);
            GreenValue2 = random(0, 50);
        }
        else
        {
            GreenValue2 = random(0, 256);
            RedValue2 = random(0, 50);
        }
    }
    
    while(RedValue != RedValue2 or GreenValue != GreenValue2 or BlueValue != BlueValue2)
    {
      analogWrite(redPin, RedValue);
      analogWrite(greenPin, GreenValue);
      analogWrite(bluePin, BlueValue);  

      if(RedValue > RedValue2)
      {
        RedValue--;
      }
      else if(RedValue < RedValue2)
      {
        RedValue++;
      }

      if(GreenValue > GreenValue2)
      {
        GreenValue--;
      }
      else if(GreenValue < GreenValue2)
      {
        GreenValue++;
      }

      if(BlueValue > BlueValue2)
      {
        BlueValue--;
      }
      else if(BlueValue < BlueValue2)
      {
        BlueValue++;
      }

      delay(Delay);
      TimeCounter = TimeCounter + Delay;
      
      if(TimeCounter > Delay2 and Delay2 != 0)
      {
        Serial.println("");
        Serial.print("Quit early");
        break;
      }
    }

    if(TimeCounter < Delay2)
    {
      Serial.println("");
      Serial.print("Waiting:");
      Serial.print(Delay2 - TimeCounter);
      delay(Delay2 - TimeCounter);
    }

    if(Delay2 != 0)
    {
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0); 
    }
    
    TimeCounter = 0;
    LastAnimationTime = millis();
  }
  
  if (client) 
  {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";               
    while (client.connected() && currentTime - previousTime <= timeoutTime) 
    {
      currentTime = millis();
      
      if (client.available()) 
      {             
        char c = client.read();            
        Serial.write(c);    

        ClientIP = String(client.remoteIP()[0]) + "." + String(client.remoteIP()[1]) + "." + String(client.remoteIP()[2]) + "." + String(client.remoteIP()[3]);
        Serial.println("IP IP IP");
        Serial.println(ClientIP);

        if(client.remoteIP()[0] == 192)
        {
          LastPerson = "Dexter Local IP";
        }
        
        if(ClientIP == "109.166.137.164" or ClientIP == "89.136.241.70")
        {
          LastPerson = "Dexter";
        }
        else
        {
          LastPerson = ClientIP;
        }
        
        header += c;
        if (c == '\n') 
        {        
    
          if (currentLine.length() == 0) 
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if(ListeningToMusic != "" and blueString.toInt() != 0)
            {
              LastPerson = "Dexter's AWS Server in Frankfurt";
            }

            client.println("<!DOCTYPE html> <html> <meta name = \"viewport\" content = \"width=device-width, initial-scale=1\"> <link rel = \"stylesheet\" href = \"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\"> <script src = \"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script> <script src = \"https://cdn.jsdelivr.net/npm/@jaames/iro\"></script> <title>Color Changer - by Dexter</title> <h1>Color Changer</h1> <h5>Current Color = R: " + String(RedValue) + " G: " + String(GreenValue) + " B: " + String(BlueValue) + " | Last Changed: " + String((millis() - LastChanged) / 60000) + " minutes ago by " + String(LastPerson) + String(ListeningToMusic) + "</h5> <div class = \"container\"> <div class = \"row\"> <div class = \"wheel\" id = \"ColorWheel\"></div> </div> <div class = \"row\"> <a class = \"btn btn-primary btn-lg\" href = \"?r0g0b0&\" id = \"ChangeColor\" role = \"button\">Change Color</a> </div> </div> <div class = \"container\"> <h3>Animations</h3> <div class = \"row\"> <div class = \"col-6\"> <a class = \"btn btn-primary btn-lg\" href = \"?r256g7.5b0&\" id = \"FadeAnimation\" role = \"button\">Fade</a> </div> <div class = \"col-6\"> <div class = \"range-wrap\"> <input type = \"range\" class = \"range\"> <output class = \"bubble\"></output> </div> </div> </div> <div class = \"row\"> <div class = \"col-6\"> <a class = \"btn btn-primary btn-lg\" href = \"?r257g0b0&\" id = \"RomanticAnimation\" role = \"button\">Romantic</a> </div> <div class = \"col-6\"> <a class = \"btn btn-primary btn-lg\" href = \"?r256g7.5b0&\" id = \"OtherAnimation\" role = \"button\">Alt mod</a> </div> </div> </div> <div class = \"FooterSection\"> </div> </html> <style> body { background-color: #313131; overflow-x: hidden; } h1, h2, h3, h4, h5, h6 { margin-top: 1.5%; margin-bottom: 2.5%; text-align: center; color:#0069D9; padding-left: 3%; padding-right: 3%; } .container { margin-top: 5%; } .btn { width: 75%; margin: 2.5% auto; border-radius: 10px; transition: transform 0.2s; } .btn:hover { transform: scale(1.1); } .row { text-align: center; } input { width: 75%; margin: 2.5% auto; } input:focus { outline: none; } .range-wrap { margin: 2.5% auto; } .bubble { background: #0069D9; color: white; padding: 4px 12px; position: absolute; border-radius: 4px; left: 50%; transform: translateX(-50%); top: -20%; } .bubble::after { overflow-x: auto; content: \"\"; position: absolute; width: 2px; height: 2px; background: #0069D9; left: 50%; } #ChangeColor { margin-top: 5%; width: 50%; } #ColorWheel { margin: 0 auto; } .FooterSection { margin-top: 20%; } @media only screen and (max-width: 800px) { h1, h2, h3, h4, h5, h6 { margin-top: 5%; margin-bottom: 7.5%; } .container { margin-top: 10%; } .btn, .range-wrap, input { margin: 7.5% auto; } #ChangeColor { width: 80%; } #ColorWheel{margin: 5% auto;} } </style> <script> var colorWheel = new iro.ColorPicker(\"#ColorWheel\", { color: \"rgb(" + String(RedValue) + "," + String(GreenValue) + "," + String(BlueValue) + ")\", borderWidth: 2, borderColor: '#000000', width: 250, }); colorWheel.on('color:change', function(color, changes) { var Color = colorWheel.color.rgb; document.getElementById(\"ChangeColor\").href = \"?r\" + Math.round(Color.r) + \"g\" + Math.round(Color.g) + \"b\" + Math.round(Color.b) + \"&\"; document.getElementById(\"ChangeColor\").innerText = \"Change Color to R: \" + Math.round(Color.r) + \" G: \" + Math.round(Color.g) + \" B: \" + Math.round(Color.b); }); const allRanges = document.querySelectorAll(\".range-wrap\"); allRanges.forEach(wrap => { const range = wrap.querySelector(\".range\"); const bubble = wrap.querySelector(\".bubble\"); range.addEventListener(\"input\", () => { setBubble(range, bubble); }); setBubble(range, bubble); }); function setBubble(range, bubble) { const val = range.value / 5; const min = range.min ? range.min : 0; const max = range.max ? range.max : 100; const newVal = Number(((val - min) * 75) / (max - min) * 5 + 12.5); bubble.innerHTML = \"Fade: \" + val + \" ms\" ; bubble.style.left = `calc(${newVal}% + (${8 - newVal * 0.15}px))`; document.getElementById(\"FadeAnimation\").href=\"?r256g\" + val + \"b0&\"; } </script>");

            client.println();

            if(header.indexOf("GET /?r") >= 0) 
            {
              pos1 = header.indexOf('r');
              pos2 = header.indexOf('g');
              pos3 = header.indexOf('b');
              pos4 = header.indexOf('&');
              redString = header.substring(pos1+1, pos2);
              greenString = header.substring(pos2+1, pos3);
              blueString = header.substring(pos3+1, pos4);
              
              LastChanged = currentTime;
              
              if(redString == "256")
              {
                 Animation = true;
                 Delay2 = blueString.toFloat();
  
                 if(Delay2 > 666)
                 {
                  Delay = 3;
                 }
                 else if(Delay2 < 666 and Delay2 > 545)
                 {
                  Delay = 2.5;
                 }
                 else if(Delay2 <545 and Delay2 > 461)
                 {
                  Delay = 2;
                 }
                 else if(Delay2 < 461 and Delay2 > 0)
                 {
                  Delay = 1.5;
                 }
                 else
                 {
                  Delay = greenString.toFloat();
                 }

                 if(Delay2 != 0)
                 {
                  ListeningToMusic = " | Listening to a song with BPM: " + String(60000 / Delay2);
                 }
                 else
                 {
                  ListeningToMusic = " | Leds in Fade Mode with Transition: " + String(greenString.toFloat()) + "ms";
                 }
                 
                 RedValue = 0;
                 GreenValue = 0;
                 BlueValue = 0;
              }
              else
              {
                RedValue = redString.toInt();
                GreenValue = greenString.toInt();
                BlueValue = blueString.toInt();
              
                analogWrite(redPin, RedValue);
                analogWrite(greenPin, GreenValue);
                analogWrite(bluePin, BlueValue);
                Animation = false;
                ListeningToMusic = "";
              }
            }
            break;
          } 
          else
          { 
            currentLine = "";
          }
        }
        else if (c != '\r')
        { 
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
