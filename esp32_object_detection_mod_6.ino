//const char* ssid     = "project";      //Define your own SSID
//const char* password = "project007";   // Define your own password
const char* ssid     = "TPLink";      //Define your own SSID
const char* password = "Excitel@103";   // Define your own password
int pulse = 0; //GPIO14 to act as trigger pin
int head = 1; //Define the thereshold number of object capture
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HardwareSerial.h>
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

String Feedback = "";
String Command = "", cmd = "", P1 = "", P2 = "", P3 = "", P4 = "", P5 = "", P6 = "", P7 = "", P8 = "", P9 = ""; // bounday parameters of object

byte ReceiveState = 0, cmdState = 1, strState = 1, questionstate = 0, equalstate = 0, semicolonstate = 0;


#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define S0 12
#define S1 13
#define S2 15
#define S3 14
#define sensorOut 2
#define button  16      // Pulse -> pin 16 and button -> pin0
int frequency = 0;
int redColor=0, greenColor=0;

WiFiServer server(80);

void ExecuteCommand() {
  if (cmd != "getstill") {
    //    Serial.println("cmd= "+cmd+" ,P1= "+P1+" ,P2= "+P2+" ,P3= "+P3+" ,P4= "+P4+" ,P5= "+P5+" ,P6= "+P6+" ,P7= "+P7+" ,P8= "+P8+" ,P9= "+P9);
    //        Serial.print("Execute command Start");
    //        Serial.println(P1);
    //        Serial.println(P2);
    //        Serial.println("Execute command Stop");
    
    if (P1.indexOf("bus") > 0)
    {
      //      Serial.println("bus c");
      Serial.println("112");
    }

    if (P1.indexOf("car") > 0)
      //    if(P1=="car")
    {
      //      Serial.println("car c");
      //      mySerial.print("224");
      Serial.println("224");
    }

    if (P1.indexOf("tra") > 0)
      //   if(P1=="traffic light")
    {
      //      Serial.println("traffic c");
      //      mySerial.print("336");
      Serial.println("336");
    }

    if (P1.indexOf("stop") > 0)
      //    if(P1=="stop sign")
    {
      //      Serial.println("stop c");
      //      mySerial.print("448");
      Serial.println("448");
    }

    if (P2.toInt() >= head)  digitalWrite(pulse, 1); //Set the pulse pin high on threshold count
  }

  if (cmd == "resetwifi") {
    WiFi.begin(P1.c_str(), P2.c_str());
    Serial.print("Connecting to ");
    Serial.println(P1);


    long int StartTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      if ((StartTime + 5000) < millis()) break;
    }
    Serial.println("");
    Serial.println("STAIP: " + WiFi.localIP().toString());
    Feedback = "STAIP: " + WiFi.localIP().toString();
  }
  else if (cmd == "restart") {
    ESP.restart();
  }
  else if (cmd == "digitalwrite") {
    ledcDetachPin(P1.toInt());
    pinMode(P1.toInt(), OUTPUT);
    digitalWrite(P1.toInt(), P2.toInt());
  }
  else if (cmd == "analogwrite") {
    if (P1 = "4") {
      ledcAttachPin(4, 4);
      ledcSetup(4, 5000, 8);
      ledcWrite(4, P2.toInt());
    }
    else {
      ledcAttachPin(P1.toInt(), 5);
      ledcSetup(5, 5000, 8);
      ledcWrite(5, P2.toInt());
    }
  }
  else if (cmd == "flash") {
    ledcAttachPin(4, 4);
    ledcSetup(4, 5000, 8);

    int val = P1.toInt();
    ledcWrite(4, val);
  }
  else if (cmd == "framesize") {
    sensor_t * s = esp_camera_sensor_get();
    if (P1 == "QQVGA")
      s->set_framesize(s, FRAMESIZE_QQVGA);
    else if (P1 == "HQVGA")
      s->set_framesize(s, FRAMESIZE_HQVGA);
    else if (P1 == "QVGA")
      s->set_framesize(s, FRAMESIZE_QVGA);
    else if (P1 == "CIF")
      s->set_framesize(s, FRAMESIZE_CIF);
    else if (P1 == "VGA")
      s->set_framesize(s, FRAMESIZE_VGA);
    else if (P1 == "SVGA")
      s->set_framesize(s, FRAMESIZE_SVGA);
    else if (P1 == "XGA")
      s->set_framesize(s, FRAMESIZE_XGA);
    else if (P1 == "SXGA")
      s->set_framesize(s, FRAMESIZE_SXGA);
    else if (P1 == "UXGA")
      s->set_framesize(s, FRAMESIZE_UXGA);
    else
      s->set_framesize(s, FRAMESIZE_QVGA);
  }
  else if (cmd == "quality") {
    sensor_t * s = esp_camera_sensor_get();
    int val = P1.toInt();
    s->set_quality(s, val);
  }
  else if (cmd == "contrast") {
    sensor_t * s = esp_camera_sensor_get();
    int val = P1.toInt();
    s->set_contrast(s, val);
  }
  else if (cmd == "brightness") {
    sensor_t * s = esp_camera_sensor_get();
    int val = P1.toInt();
    s->set_brightness(s, val);
  }
  else if (cmd == "serial") {
    Serial.println(P1);
  }
  else if (cmd == "detectCount") {
    //    Serial.print("detectCount Start");
    //    Serial.println(P1 + " = " + P2);
    //    Serial.println("detectCount Stop");
    //    Serial.println(P1.substring(0, 2));

    //    if(P1.indexOf("bus") > 0)
    if (P1.substring(0, 3) == "bus")
    {
      //      Serial.println("bus c");
      Serial.println("112");
    }

    if (P1.substring(0, 3) == "car")
    {
      //      Serial.println("car detected");
      Serial.println("224");
    }

    if (P1.substring(0, 3) == "tra")
    {
      //     Serial.println("traffic detected");
      Serial.println("336");
    }

    if (P1.substring(0, 4) == "stop")
    {
      //      Serial.println("stop detected");
      Serial.println("448");
    }

////    // Setting red filtered photodiodes to be read
//    digitalWrite(S2,LOW);
//    digitalWrite(S3,LOW);
//    // Reading the output frequency
//    frequency = pulseIn(sensorOut, LOW);
//    redColor = frequency; 
//    delay(40);
//    // Setting Green filtered photodiodes to be read
//    digitalWrite(S2,HIGH);
//    digitalWrite(S3,HIGH);
//    // Reading the output frequency
//    frequency = pulseIn(sensorOut, LOW);
//    greenColor = frequency; 
//    
//    Serial.print("R= ");//printing name
//    Serial.print(redColor);//printing RED color frequency
//    Serial.print("G= ");//printing name
//    Serial.println(greenColor);//printing RED color frequency
////    if(redColor<25 && greenColor>30)
////    Serial.println("555");     // RED
////    if(redColor>25 && greenColor<30)
////    Serial.println("666");     // GREEN
    

  }
  else if (cmd == "tcp") {
    String domain = P1;
    int port = P2.toInt();
    String request = P3;
    int wait = P4.toInt();

    Feedback = tcp_http(domain, request, port, wait);
  }
  else if (cmd == "linenotify") {
    String token = P1;
    String request = P2;
    Feedback = LineNotify(token, request, 1);
    if (Feedback.indexOf("status") != -1) {
      int s = Feedback.indexOf("{");
      Feedback = Feedback.substring(s);
      int e = Feedback.indexOf("}");
      Feedback = Feedback.substring(0, e);
      Feedback.replace("\"", "");
      Feedback.replace("{", "");
      Feedback.replace("}", "");
    }
  }
  else if (cmd == "sendCapturedImageToLineNotify") {
    Feedback = sendCapturedImageToLineNotify(P1);
    if (Feedback == "") Feedback = "The image failed to send. <br>The framesize may be too large.";
  }
  else {
    Feedback = "Command is not defined.";
  }
  if (Feedback == "") Feedback = Command;
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);     //disable brown out detection to run on low power upto 2.44v
  pinMode(pulse, OUTPUT);
  digitalWrite(pulse, 0);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(button, INPUT);  
   
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,HIGH);
  
  //ESP32 Camera pin set up
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;  //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);  //UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA
  //added extra controls
  s->set_vflip(s, 1); // flip it back
  //  s->set_brightness(s, 1); // up the brightness just a bit
  //  s->set_saturation(s, -2); // lower the saturation
  // control ends

  ledcAttachPin(4, 4);
  ledcSetup(4, 5000, 8);

  WiFi.mode(WIFI_AP_STA);
  //WiFi.config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);
  //Change local IP as per your requirements.
  // WiFi.config(IPAddress(192, 168, 43, 10), IPAddress(192, 168, 43, 2), IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);

  delay(1000);
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  long int StartTime = millis();
  while (WiFi.status() != WL_CONNECTED)   {
    delay(500);
    if ((StartTime + 10000) < millis()) break;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("STAIP address: ");
    Serial.println(WiFi.localIP());

    for (int i = 0; i < 5; i++) {
      ledcWrite(4, 10);
      delay(200);
      ledcWrite(4, 0);
      delay(200);
    }
  }

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  server.begin();
}

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
  <!DOCTYPE html>
  <head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <script src="https:\/\/ajax.googleapis.com/ajax/libs/jquery/1.8.0/jquery.min.js"></script>
  <script src="https:\/\/cdn.jsdelivr.net/npm/@tensorflow/tfjs@1.3.1/dist/tf.min.js"> </script>
  <script src="https:\/\/cdn.jsdelivr.net/npm/@tensorflow-models/coco-ssd@2.1.0"> </script>   
  </head><body>
  <img id="ShowImage" src="" style="display:none">
  <canvas id="canvas" width="0" height="0"></canvas>  
  <table>
  <tr>
    <td><input type="button" id="restart" value="Restart"></td> 
    <td colspan="2"><input type="button" id="getStill" value="Start Detect" style="display:none"></td> 
  </tr>
  <tr>
    <td>Object</td> 
    <td colspan="2">
        <select id="object" onchange="count.innerHTML='';">
          <option value="car">car</option>
          <option value="bus">bus</option>
          <option value="traffic light">traffic light</option>
          <option value="stop">stop</option>
        </select>
    </td>
    <td><span id="count" style="color:red"><span></td>
  </tr>
  <tr>
    <td>ScoreLimit</td> 
    <td colspan="2">
      <select id="score">
        <option value="1.0">1</option>
        <option value="0.9">0.9</option>
        <option value="0.8">0.8</option>
        <option value="0.7">0.7</option>
        <option value="0.6">0.6</option>
        <option value="0.5">0.5</option>
        <option value="0.4">0.4</option>
        <option value="0.3">0.3</option>
        <option value="0.2">0.2</option>
        <option value="0.1">0.1</option>
        <option value="0" selected="selected">0</option>
      </select>
    </td>
  </tr>
  <tr>
    <td>MirrorImage</td> 
    <td colspan="2">  
      <select id="mirrorimage">
        <option value="1">yes</option>
        <option value="0">no</option>
      </select>
    </td>
  </tr>     
  <tr>
    <td>Resolution</td> 
    <td colspan="2">
      <select id="framesize">
        <option value="UXGA">UXGA(1600x1200)</option>
        <option value="SXGA">SXGA(1280x1024)</option>
        <option value="XGA">XGA(1024x768)</option>
        <option value="SVGA">SVGA(800x600)</option>
        <option value="VGA">VGA(640x480)</option>
        <option value="CIF">CIF(400x296)</option>
        <option value="QVGA"  selected="selected"  >QVGA(320x240)</option>
        <option value="HQVGA">HQVGA(240x176)</option>
        <option value="QQVGA">QQVGA(160x120)</option>
      </select> 
    </td>
  </tr>    
  <tr>
    <td>Flash</td>
    <td colspan="2"><input type="range" id="flash" min="0" max="255" value="0"></td>
  </tr>
  <tr>
    <td>Quality</td>
    <td colspan="2"><input type="range" id="quality" min="10" max="63" value="10"></td>
  </tr>
  <tr>
    <td>Brightness</td>
    <td colspan="2"><input type="range" id="brightness" min="-2" max="2" value="0"></td>
  </tr>
  <tr>
    <td>Contrast</td>
    <td colspan="2"><input type="range" id="contrast" min="-2" max="2" value="0"></td>
  </tr>

  </table>
  <div id="result" style="color:red"><div>
  </body>
  </html> 
  
  <script>
    var getStill = document.getElementById('getStill');
    var ShowImage = document.getElementById('ShowImage');
    var canvas = document.getElementById("canvas");
    var context = canvas.getContext("2d"); 
    var object = document.getElementById('object');
    var score = document.getElementById("score");
    var mirrorimage = document.getElementById("mirrorimage");      
    var count = document.getElementById('count'); 
    var result = document.getElementById('result');
    var flash = document.getElementById('flash'); 
    var lastValue="";
    var myTimer;
    var restartCount=0;
    var Model;    
    getStill.onclick = function (event) {
      clearInterval(myTimer);  
      myTimer = setInterval(function(){error_handle();},5000);
      ShowImage.src=location.origin+'/?getstill='+Math.random();
    }
    function error_handle() {
      restartCount++;
      clearInterval(myTimer);
      if (restartCount<=2) {
        result.innerHTML = "Get still error. <br>Restart ESP32-CAM "+restartCount+" times.";
        myTimer = setInterval(function(){getStill.click();},10000);
      }
      else
        result.innerHTML = "Get still error. <br>Please close the page and check ESP32-CAM.";
    }    
    ShowImage.onload = function (event) {
      clearInterval(myTimer);
      restartCount=0;
      canvas.setAttribute("width", ShowImage.width);
      canvas.setAttribute("height", ShowImage.height);
      if (mirrorimage.value==1) {
        context.translate((canvas.width + ShowImage.width) / 2, 0);
        context.scale(-1, 1);
        context.drawImage(ShowImage, 0, 0, ShowImage.width, ShowImage.height);
        context.setTransform(1, 0, 0, 1, 0, 0);
      }
      else
        context.drawImage(ShowImage,0,0,ShowImage.width,ShowImage.height);
      if (Model) {
        DetectImage();
      }          
    }     
    
    restart.onclick = function (event) {
      fetch(location.origin+'/?restart=stop');
    }    
    framesize.onclick = function (event) {
      fetch(document.location.origin+'/?framesize='+this.value+';stop');
    }  
    flash.onchange = function (event) {
      fetch(location.origin+'/?flash='+this.value+';stop');
    } 
    quality.onclick = function (event) {
      fetch(document.location.origin+'/?quality='+this.value+';stop');
    } 
    brightness.onclick = function (event) {
      fetch(document.location.origin+'/?brightness='+this.value+';stop');
    } 
    contrast.onclick = function (event) {
      fetch(document.location.origin+'/?contrast='+this.value+';stop');
    }                             
    
    function ObjectDetect() {
      result.innerHTML = "Please wait for loading model.";
      cocoSsd.load().then(cocoSsd_Model => {
        Model = cocoSsd_Model;
        result.innerHTML = "";
        getStill.style.display = "block";
      }); 
    }
    
    function DetectImage() {
      Model.detect(canvas).then(Predictions => {    
        var s = (canvas.width>canvas.height)?canvas.width:canvas.height;
        var objectCount=0;
        
        //console.log('Predictions: ', Predictions);
        if (Predictions.length>0) {
          result.innerHTML = "";
          for (var i=0;i<Predictions.length;i++) {
            const x = Predictions[i].bbox[0];
            const y = Predictions[i].bbox[1];
            const width = Predictions[i].bbox[2];
            const height = Predictions[i].bbox[3];
            context.lineWidth = Math.round(s/200);
            context.strokeStyle = "#00FFFF";
            context.beginPath();
            context.rect(x, y, width, height);
            context.stroke(); 
            context.lineWidth = "2";
            context.fillStyle = "red";
            context.font = Math.round(s/30) + "px Arial";
            context.fillText(Predictions[i].class, x, y);
            //context.fillText(i, x, y);
            result.innerHTML+= "[ "+i+" ] "+Predictions[i].class+", "+Math.round(Predictions[i].score*100)+"%, "+Math.round(x)+", "+Math.round(y)+", "+Math.round(width)+", "+Math.round(height)+"<br>";
            
            if (Predictions[i].class==object.value&&Predictions[i].score>=score.value) {
              objectCount++;
            }  
          }
          count.innerHTML = objectCount;
        }
        else {
          result.innerHTML = "Unrecognizable";
          count.innerHTML = "0";
        }
    
      //if (count.innerHTML != lastValue) { 
          lastValue = count.innerHTML;
          if (objectCount>0) {
      //      $.ajax({url: document.location.origin+'/?serial='+object.value+';stop', async: false});
            $.ajax({url: document.location.origin+'/?detectCount='+object.value+';'+String(objectCount)+';stop', async: false}); 
           }  
      //}
        try { 
          document.createEvent("TouchEvent");
          setTimeout(function(){getStill.click();},250);
        }
        catch(e) { 
          setTimeout(function(){getStill.click();},150);
        } 
      });
    }
    function getFeedback(target) {
      var data = $.ajax({
      type: "get",
      dataType: "text",
      url: target,
      success: function(response)        {
          result.innerHTML = response;
        },
        error: function(exception)        {
          result.innerHTML = 'fail';
        }
      });
    }      
    window.onload = function () { 
      ObjectDetect(); 

       
      }    
  </script>   
)rawliteral";



void loop() {
  Feedback="";Command="";cmd="";P1="";P2="";P3="";P4="";P5="";P6="";P7="";P8="";P9="";
  ReceiveState=0,cmdState=1,strState=1,questionstate=0,equalstate=0,semicolonstate=0;
  
   WiFiClient client = server.available();

  if (client) { 
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();  
        

        getCommand(c);   
                
        if (c == '\n') {
          if (currentLine.length() == 0) {    
            
            if (cmd=="getstill") {
             
              camera_fb_t * fb = NULL;
              fb = esp_camera_fb_get();  
              if(!fb) {
                Serial.println("Camera capture failed");
                delay(1000);
                ESP.restart();
              }
  
              client.println("HTTP/1.1 200 OK");
              client.println("Access-Control-Allow-Origin: *");              
              client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
              client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
              client.println("Content-Type: image/jpeg");
              client.println("Content-Disposition: form-data; name=\"imageFile\"; filename=\"picture.jpg\""); 
              client.println("Content-Length: " + String(fb->len));             
              client.println("Connection: close");
              client.println();
              
              uint8_t *fbBuf = fb->buf;
              size_t fbLen = fb->len;
              for (size_t n=0;n<fbLen;n=n+1024) {
                if (n+1024<fbLen) {
                  client.write(fbBuf, 1024);
                  fbBuf += 1024;
                }
                else if (fbLen%1024>0) {
                  size_t remainder = fbLen%1024;
                  client.write(fbBuf, remainder);
                }
              }  
              
              esp_camera_fb_return(fb);
            
              pinMode(4, OUTPUT);
              digitalWrite(4, LOW);               
            }
            else {
              
              client.println("HTTP/1.1 200 OK");
              client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
              client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
              client.println("Content-Type: text/html; charset=utf-8");
              client.println("Access-Control-Allow-Origin: *");
              client.println("Connection: close");
              client.println();
              
              String Data="";
              if (cmd!="")
                Data = Feedback;
              else {
                Data = String((const char *)INDEX_HTML);
              }
              int Index;
              for (Index = 0; Index < Data.length(); Index = Index+1000) {
                client.print(Data.substring(Index, Index+1000));
              }           
              
              client.println();
            }
                        
            Feedback="";
            break;
          } else {
            currentLine = "";
          }
        } 
        else if (c != '\r') {
          currentLine += c;
        }

        if ((currentLine.indexOf("/?")!=-1)&&(currentLine.indexOf(" HTTP")!=-1)) {
          if (Command.indexOf("stop")!=-1) {   http://192.168.xxx.xxx/?cmd=aaa;bbb;ccc;stop
            client.println();
            client.println();
            client.stop();
          }
          currentLine="";
          Feedback="";
          ExecuteCommand();

      //    // Setting red filtered photodiodes to be read
          digitalWrite(S2,LOW);
          digitalWrite(S3,LOW);
          // Reading the output frequency
          frequency = pulseIn(sensorOut, LOW);
          redColor = frequency; 
//          delay(40);
          // Setting Green filtered photodiodes to be read
          digitalWrite(S2,HIGH);
          digitalWrite(S3,HIGH);
          // Reading the output frequency
          frequency = pulseIn(sensorOut, LOW);
          greenColor = frequency; 
          
//          Serial.print("R= ");//printing name
//          Serial.print(redColor);//printing RED color frequency
//          Serial.print("G= ");//printing name
//          Serial.println(greenColor);//printing RED color frequency
//          if(digitalRead(button) == 0)   // if button is pressed
//          {
            if(redColor<20 && greenColor>30)
            Serial.println("555");     // RED
            if(redColor>30 && greenColor<22)
            Serial.println("666");     // GREEN
//          }
          
         }
     }
    }
    delay(1);
    client.stop();
  }
}


void getCommand(char c){
  if (c=='?') ReceiveState=1;
  if ((c==' ')||(c=='\r')||(c=='\n')) ReceiveState=0;
  
  if (ReceiveState==1)  {
    Command=Command+String(c);
    
    if (c=='=') cmdState=0;
    if (c==';') strState++;
  
    if ((cmdState==1)&&((c!='?')||(questionstate==1))) cmd=cmd+String(c);
    if ((cmdState==0)&&(strState==1)&&((c!='=')||(equalstate==1))) P1=P1+String(c);
    if ((cmdState==0)&&(strState==2)&&(c!=';')) P2=P2+String(c);
    if ((cmdState==0)&&(strState==3)&&(c!=';')) P3=P3+String(c);
    if ((cmdState==0)&&(strState==4)&&(c!=';')) P4=P4+String(c);
    if ((cmdState==0)&&(strState==5)&&(c!=';')) P5=P5+String(c);
    if ((cmdState==0)&&(strState==6)&&(c!=';')) P6=P6+String(c);
    if ((cmdState==0)&&(strState==7)&&(c!=';')) P7=P7+String(c);
    if ((cmdState==0)&&(strState==8)&&(c!=';')) P8=P8+String(c);
    if ((cmdState==0)&&(strState>=9)&&((c!=';')||(semicolonstate==1))) P9=P9+String(c);

if(P2.toInt()<1 or P2.toInt()==0)  digitalWrite(pulse,0);
    
    if (c=='?') questionstate=1;
    if (c=='=') equalstate=1;
    if ((strState>=9)&&(c==';')) semicolonstate=1;
  }
}

String tcp_http(String domain,String request,int port,byte wait){
    WiFiClient client_tcp;

    if (client_tcp.connect(domain.c_str(), port)) {
      Serial.println("GET " + request);
      client_tcp.println("GET " + request + " HTTP/1.1");
      client_tcp.println("Host: " + domain);
      client_tcp.println("Connection: close");
      client_tcp.println();

      String getResponse="",Feedback="";
      boolean state = false;
      int waitTime = 3000;   // timeout 3 seconds
      long startTime = millis();
      while ((startTime + waitTime) > millis())      {
        while (client_tcp.available())         {
            char c = client_tcp.read();
            if (state==true) Feedback += String(c);          
            if (c == '\n') {
              if (getResponse.length()==0) state=true; 
              getResponse = "";
            } 
            else if (c != '\r')
              getResponse += String(c);
            if (wait==1)
              startTime = millis();
         }
         if (wait==0)
          if ((state==true)&&(Feedback.length()!= 0)) break;
      }
      client_tcp.stop();
      return Feedback;
    }
    else
      return "Connection failed";  
}



String LineNotify(String token, String request, byte wait){
  request.replace("%","%25");  
  request.replace(" ","%20");
  request.replace("&","%20");
  request.replace("#","%20");
  //request.replace("\'","%27");
  request.replace("\"","%22");
  request.replace("\n","%0D%0A");
  request.replace("%3Cbr%3E","%0D%0A");
  request.replace("%3Cbr/%3E","%0D%0A");
  request.replace("%3Cbr%20/%3E","%0D%0A");
  request.replace("%3CBR%3E","%0D%0A");
  request.replace("%3CBR/%3E","%0D%0A");
  request.replace("%3CBR%20/%3E","%0D%0A"); 
  request.replace("%20stickerPackageId","&stickerPackageId");
  request.replace("%20stickerId","&stickerId");    
  
  WiFiClientSecure client_tcp;
  client_tcp.setInsecure();   //run version 1.0.5 or above

  
}

String sendCapturedImageToLineNotify(String token) {
  String getAll="", getBody = "";
  
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
    return "";
  }  
      
  WiFiClientSecure client_tcp;
  client_tcp.setInsecure();   
  return getBody;

}
