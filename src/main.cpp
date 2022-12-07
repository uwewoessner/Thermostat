#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp

#include <FS.h>
#include <littlefs.h>
DHTesp dht;
#if defined Wohnzimmer
#define Zimmer "wohnzimmer"
#endif
#if defined WohnzimmerNeu
#define Zimmer "wohnzimmer"
const float TemperatureOffset = -3;
#define NewPCB
#endif
#if defined Kueche
#define Zimmer "kueche"
const float TemperatureOffset = -2;
#define NewPCB
#endif
#if defined Jan
#define Zimmer "Jan"
const float TemperatureOffset = -3;
#define NewPCB
#endif
#if defined Jule
#define Zimmer "Jule"
const float TemperatureOffset = -3;
#define NewPCB
#endif
#if defined Flur
#define Zimmer "flur"
const float TemperatureOffset = -3;
#define NewPCB
#endif
#if defined WC
#define Zimmer "wc"
const float TemperatureOffset = -1;
#define NewPCB
#endif
#if defined Mobile
#define Zimmer "mobile"
const float TemperatureOffset = 0;
#endif
#if defined Bad
#define Zimmer "bad"
const float TemperatureOffset = -2.4;
#define NewPCB
#define HaveButtons
#endif

#include <ArduinoOTA.h>
#ifdef ESP32
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>

#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansBold18pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 01       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#include <PubSubClient.h>
#include "debouncebutton.h"

#include <ESP32Encoder.h>
void updateDisplay();
// 'flame', 17x20px
const unsigned char epd_bitmap_flame [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x04, 0xc0, 0x00, 0x04, 0xc0, 0x00, 0x07, 
	0xe0, 0x00, 0x0f, 0xe4, 0x00, 0x0f, 0xec, 0x00, 0x2f, 0xfc, 0x00, 0x67, 0xff, 0x00, 0x77, 0xff, 
	0x00, 0x7f, 0x3f, 0x00, 0x7e, 0x1f, 0x00, 0x7c, 0x0f, 0x00, 0x78, 0x0f, 0x00, 0x78, 0x0f, 0x00, 
	0x38, 0x0e, 0x00, 0x1c, 0x0c, 0x00, 0x0e, 0x38, 0x00, 0x00, 0x00, 0x00
};
// 'floorHeating', 29x20px
const unsigned char epd_bitmap_floorHeating [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x08, 0x00, 0x01, 0x04, 0x10, 0x00, 0x02, 0x08, 0x20, 0x00, 
	0x02, 0x08, 0x20, 0x00, 0x01, 0x04, 0x10, 0x00, 0x00, 0x82, 0x08, 0x00, 0x00, 0x41, 0x04, 0x00, 
	0x00, 0x41, 0x04, 0x00, 0x00, 0x82, 0x08, 0x00, 0x01, 0x04, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0x7f, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfb, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x7d, 0xff, 0xf7, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf0
};
// 'floorNoHeating', 29x20px
const unsigned char epd_bitmap_floorNoHeating [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0x7f, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfb, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x7d, 0xff, 0xf7, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf0
};
// 'Heizkoerper', 20x20px
const unsigned char epd_bitmap_Heizkoerper [] PROGMEM = {
	0x01, 0x24, 0x00, 0x02, 0x48, 0x00, 0x02, 0x48, 0x00, 0x01, 0x24, 0x00, 0x02, 0x48, 0x00, 0x00, 
	0x00, 0x00, 0x0d, 0xb6, 0x00, 0x12, 0x49, 0x40, 0x12, 0x49, 0x40, 0x72, 0x49, 0xc0, 0x12, 0x49, 
	0x00, 0x12, 0x49, 0x00, 0x12, 0x49, 0x00, 0x12, 0x49, 0x00, 0x12, 0x49, 0x00, 0x12, 0x49, 0x00, 
	0x12, 0x49, 0x00, 0x12, 0x49, 0x00, 0x12, 0x49, 0x00, 0x0d, 0xb6, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 352)
const int epd_bitmap_allArray_LEN = 4;
const unsigned char* epd_bitmap_allArray[4] = {
	epd_bitmap_Heizkoerper,
	epd_bitmap_flame,
	epd_bitmap_floorHeating,
	epd_bitmap_floorNoHeating
};

float istTemperatur = 22.0;
float sollTemperatur = 22.0;
float istHumidity;
bool isOn=true;
float hysterese = 0.1;
bool heating = false;

unsigned long now=0;
bool fileChanged = false;
unsigned long fileChangeTime = 0;

ESP32Encoder encoder1;
#define DEBUG

#ifdef DEBUG

void DebugPrintf(const char *, ...); //Our printf function
#else
#define DebugPrintf(a) ;
#endif
char *convert(int, int); //Convert integer number into octal, hex, etc.

const char *mqtt_server = "192.168.178.34";
#include "../../../wifiPasswd.h"

//WiFiServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);

const int pButton1 = 17;
const int pA1 = 19;
const int pB1 = 18;
const int pLEDUp = 25;
const int pLEDDown = 23;
const int pUp = 14;
const int pDown = 13;
#ifdef NewPCB
const int ssrPort = 16;
#else
const int ssrPort = 23;
#endif
//const int pIntZero = 25;

int networkTimeout = 0;
bool PingArrived = false;

//default

debounceButton button1(pButton1);
debounceButton bUp(pUp);
debounceButton bDown(pDown);

#ifdef DEBUG
char linebuf[200];
unsigned int linePos = 0;
unsigned int printLine = 0;
void outputChar(char c)
{

  if (linePos < 199)
  {
    linebuf[linePos] = c;
    linePos++;
    linebuf[linePos] = '\0';
  }
  if (c == '\n')
  {
    linePos = 0;
    if (client.connected())
    {
      char top[50];
      sprintf(top, "Debug/Dimmer/%d", printLine);
      client.publish(top, linebuf);
      printLine++;
      if (printLine > 20)
        printLine = 0;
    }
    else
    {
      Serial.print(linebuf);
    }
    linebuf[0] = '\0';
  }
}
void outputCharp(const char *s)
{
  const char *c;
  for (c = s; *c != '\0'; c++)
  {
    outputChar(*c);
  }
}
void DebugPrintf(const char *format, ...)
{
  const char *traverse;
  int i;
  const char *s;
  char iBuf[20];

  va_list arg;
  va_start(arg, format);

  for (traverse = format; *traverse != '\0'; traverse++)
  {
    while (*traverse != '%' && *traverse != '\0')
    {
      outputChar(*traverse);
      traverse++;
    }
    if (*traverse == '\0')
    {
      break;
    }

    traverse++;

    //Module 2: Fetching and executing arguments
    switch (*traverse)
    {
    case 'c':
      i = va_arg(arg, int); //Fetch char argument
      outputChar(i);
      break;

    case 'd':
      i = va_arg(arg, int); //Fetch Decimal/Integer argument
      if (i < 0)
      {
        i = -i;
        outputChar('-');
      }
      outputCharp(itoa(i, iBuf, 10));
      break;

    case 'o':
      i = va_arg(arg, unsigned int); //Fetch Octal representation
      outputCharp(itoa(i, iBuf, 8));
      break;

    case 's':
      s = va_arg(arg, char *); //Fetch string
      outputCharp(s);
      break;

    case 'x':
      i = va_arg(arg, unsigned int); //Fetch Hexadecimal representation
      outputCharp(itoa(i, iBuf, 16));
      break;
    }
  }

  //Module 3: Closing argument list to necessary clean-up
  va_end(arg);
}

char *convert(int num, int base)
{
  static char Representation[] = "0123456789ABCDEF";
  static char buffer[50];
  char *ptr;

  ptr = &buffer[49];
  *ptr = '\0';

  do
  {
    *--ptr = Representation[num % base];
    num /= base;
  } while (num != 0);

  return (ptr);
}
#endif

void localLoop();

void sendState();

void callback(char *topicP, byte *payloadP, unsigned int length)
{
  char topic[200];
  char payload[200];
  strncpy(topic, topicP, 200);
  strncpy(payload, (char *)payloadP, length);
  payload[length] = '\0';
  //unsigned long now = millis();

  DebugPrintf("Message arrived [%s] %s\n", topic, payload);
  if (strcmp(topic, Zimmer"/Thermostat/command") == 0)
  {
    // set Temperature
    if (strcasecmp(payload,"ON")==0)
    {
      isOn = true;
    }
    if (strcasecmp(payload,"OFF")==0)
    {
      isOn = false;
    }
  }
  else if (strcmp(topic, Zimmer"/Thermostat/setSoll") == 0)
  {
    sscanf(payload,"%f",&sollTemperatur);
  }
  else if (strcmp(topic, Zimmer"/Thermostat/reset") == 0)
  {
    DebugPrintf("reset\n");
    ESP.restart();
  }
  else if (strcmp(topic, Zimmer"/Thermostat/LEDAb") == 0)
  {
    if (strcasecmp(payload,"ON")==0)
    {
        digitalWrite(pLEDDown,false);
    }
    if (strcasecmp(payload,"OFF")==0)
    {
        digitalWrite(pLEDDown,true);
    }
  }
  else if (strcmp(topic, Zimmer"/Thermostat/LEDAuf") == 0)
  {
    if (strcasecmp(payload,"ON")==0)
    {
        digitalWrite(pLEDUp,false);
    }
    if (strcasecmp(payload,"OFF")==0)
    {
        digitalWrite(pLEDUp,true);
    }
  }
  else if (strcmp(topic, "IOT/Ping") == 0)
  {
    networkTimeout = 0;
    PingArrived = true;
  }
}

void sendState()
{
  char buf[50];
  sprintf(buf, "%3.1f", istTemperatur);
  client.publish(Zimmer"/Thermostat/ist", buf);
  sprintf(buf, "%3.1f", sollTemperatur);
  client.publish(Zimmer"/Thermostat/soll", buf);
  sprintf(buf, "%3.1f", istHumidity);
  client.publish(Zimmer"/Thermostat/humidity", buf);
  if (heating)
  {
    client.publish(Zimmer"/Thermostat/heating", "ON");
  }
  else
  {
    client.publish(Zimmer"/Thermostat/heating", "OFF");
  }
  if (isOn)
  {
    client.publish(Zimmer"/Thermostat/isOn", "ON");
  }
  else
  {
    client.publish(Zimmer"/Thermostat/isOn", "OFF");
  }
}

void reconnect()
{
#ifdef NO_MQTT
  return;
#endif

  DebugPrintf("Attempting MQTT connection...\n");
  // Attempt to connect
  if (client.connect("Thermostat" Zimmer))
  {
    DebugPrintf("MQTTconnected\n");
    // Once connected, publish an announcement...
    sendState();
    // ... and resubscribe
    client.subscribe(Zimmer "/Thermostat/command");
    client.subscribe(Zimmer "/Thermostat/setSoll");
    client.subscribe(Zimmer "/Thermostat/reset");
    client.subscribe(Zimmer "/Thermostat/LEDAuf");
    client.subscribe(Zimmer "/Thermostat/LEDAb");
    client.subscribe("IOT/Ping");
  }
  else
  {
    DebugPrintf("failed, rc=");
    DebugPrintf("%d", client.state());
    DebugPrintf(" try again in 5 seconds\n");
  }
}
static float itOld = 0;
static float stOld = 0;
static float ihOld = 0;
static bool oldHeating = false;
static bool oldIsOn = false;
void setup()
{
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    esp_restart();
  }

  Serial.println(F("SSD1306 allocation finished"));

  display.clearDisplay();

  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 15);             // Start at top-left corner
  display.cp437(true);                 // Use full 256 char 'Code Page 437' font
  display.printf("connecting\nto WiFi");
  display.setFont(&FreeSansBold18pt7b);

  display.display();

  Wire.setClock(400000L); // 400Khz i2c

  pinMode(pButton1, INPUT_PULLUP);
  pinMode(pUp, INPUT_PULLUP);
  pinMode(pDown, INPUT_PULLUP);
  pinMode(ssrPort, OUTPUT);
  pinMode(pLEDUp, OUTPUT);
  pinMode(pLEDDown, OUTPUT);
  digitalWrite(pLEDUp, true);
  digitalWrite(pLEDDown, true);
  heating=false;
  digitalWrite(ssrPort, !heating);

  ESP32Encoder::useInternalWeakPullResistors = UP;

  encoder1.attachHalfQuad(pA1, pB1);

  pinMode(pA1, INPUT_PULLUP);
  pinMode(pB1, INPUT_PULLUP);

  encoder1.setCount(0);
  encoder1.setFilter(1023);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  ArduinoOTA.setPort(8266);
#ifdef Wohnzimmer
  ArduinoOTA.setHostname("thermostatWohnzimmer");
#endif
#ifdef Mobile
  ArduinoOTA.setHostname("thermostatMobile");
#endif
  ArduinoOTA.onStart([]()
                     {
                       display.clearDisplay();
                       display.setFont();
                       display.setCursor(20, 5);
                       display.print("firmware\n");
                       display.print("update");
                       display.display();
                     });
  ArduinoOTA.onEnd([]()
                   {
                     DebugPrintf("\nEnd\n");
                     display.clearDisplay();
                     display.display();
                   });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        {
  esp_task_wdt_reset(); // reset the watchdog
                          display.clearDisplay();
                          display.setFont();
                          display.setCursor(20, 5);
                          display.print("firmware");
                          unsigned int w = (progress / (total / SCREEN_WIDTH));
                          display.drawFastHLine(0, 16, w, SSD1306_WHITE);
                          display.drawFastHLine(0, 17, w, SSD1306_WHITE);
                          display.drawFastHLine(0, 18, w, SSD1306_WHITE);
                          display.drawFastHLine(0, 19, w, SSD1306_WHITE);
                          display.setCursor(20, 25);
                          display.print((progress / (total / 100)), 1);
                          display.print("%");
                          display.display();
                          DebugPrintf("Progress: %u %% \r", (progress / (total / 100)));
                        });

  ArduinoOTA.onError([](ota_error_t error)
                     {
                       DebugPrintf("Error[ %u]: ", error);
                       if (error == OTA_AUTH_ERROR)
                         DebugPrintf("Auth Failed\n");
                       else if (error == OTA_BEGIN_ERROR)
                         DebugPrintf("Begin Failed\n");
                       else if (error == OTA_CONNECT_ERROR)
                         DebugPrintf("Connect Failed\n");
                       else if (error == OTA_RECEIVE_ERROR)
                         DebugPrintf("Receive Failed\n");
                       else if (error == OTA_END_ERROR)
                         DebugPrintf("End Failed\n");
                     });
  ArduinoOTA.begin();

  DebugPrintf("%s\n", WiFi.localIP().toString().c_str());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

#ifdef ESP32
  esp_task_wdt_init(25, true); //socket timeout is 15seconds
  esp_task_wdt_add(nullptr);
#endif

  button1.init(false);
  bUp.init(false);
  bDown.init(false);

  dht.setup(5, DHTesp::DHT22); // Connect DHT sensor to GPIO 5
  
    sollTemperatur = 21.5;
    isOn = true;
    if(!LITTLEFS.begin(true)){
        Serial.println("LITTLEFS Mount Failed");
        return;
    }
    File file = LITTLEFS.open("/config.txt");
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }
    file.read((uint8_t *)&sollTemperatur,sizeof(float));
    file.read((uint8_t *)&isOn,sizeof(isOn));
    file.close();

  /*EEPROM.begin(sizeof(float)+sizeof(char)+10);
  sollTemperatur = EEPROM.readFloat(0);
  isOn = EEPROM.readBool(sizeof(float));
  Serial.println(sollTemperatur);
  */
  if(sollTemperatur<10.0 || sollTemperatur > 30 || sollTemperatur != sollTemperatur )
  {
    sollTemperatur = 21.5;
    isOn = true;
  }

  stOld = sollTemperatur;
  oldIsOn = isOn;
}

void reconnectWifi()
{
  bool ledState = false;
  while (WiFi.status() != WL_CONNECTED)
  {
    long start = millis();
    while (millis() - start < 500)
    {
      localLoop();
    }
    ledState = !ledState;
  }
}
long lastReconnectAttempt = 0;
long lastReconnectWifiAttempt = 0;

void loop()
{
  unsigned long now = millis();

  if (WiFi.status() != WL_CONNECTED)
  {
    if (now - lastReconnectWifiAttempt > 60000) // every 60 seconds
    {
      lastReconnectWifiAttempt = now;
      // Attempt to reconnect
      reconnectWifi();
    }
  }
  if (!client.connected())
  {
    if (now - lastReconnectAttempt > 10000) // every 10 seconds
    {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      reconnect();
    }
  }
  else
  {
    // Client connected

    client.loop();
  }

  localLoop(); // local updates which have to be done during reconnect as well;
}

void localLoop()
{
  debounceButton::update();
  esp_task_wdt_reset(); // reset the watchdog
  ArduinoOTA.handle();
  now = millis();
  static unsigned long oldTemperatureTime = 0;
  static int64_t oldCounter1 = 0;
  int64_t currentCounter1 = 0;
  currentCounter1 = encoder1.getCount();
  if(currentCounter1 != oldCounter1)
  {
    sollTemperatur += (currentCounter1 - oldCounter1)*0.05;
    oldCounter1 = currentCounter1;
    sendState();
    updateDisplay();
  }
  if (now - oldTemperatureTime > 2000)
  {
    oldTemperatureTime = now;

    istHumidity = dht.getHumidity();
    istTemperatur = dht.getTemperature() + TemperatureOffset;

    updateDisplay();

    digitalWrite(ssrPort, !heating);
  }

  if(itOld!=istTemperatur || ihOld< istHumidity-0.1 ||ihOld> istHumidity+0.1 || stOld != sollTemperatur || heating != oldHeating || isOn != oldIsOn)
  {
    if (sollTemperatur != stOld || isOn != oldIsOn)
    {
      fileChanged = true;
      fileChangeTime = now;
    }
    itOld = istTemperatur;
    ihOld = istHumidity;
    stOld = sollTemperatur;
    oldHeating = heating;
    oldIsOn = isOn;
    sendState();
  }

  if (fileChanged && (now - fileChangeTime) > 10000) // wait 10 seconds to write file after last change
  {
    File file = LITTLEFS.open("/config.txt", "w");
    if (!file || file.isDirectory())
    {
      Serial.println("- failed to open file for reading");
      return;
    }
    file.write((uint8_t *)&sollTemperatur, sizeof(float));
    file.write((uint8_t *)&isOn, sizeof(isOn));
    file.close();
    Serial.print("Writing");
    Serial.print(sollTemperatur);
    Serial.println(stOld);
    fileChanged = false;
  }

  if (button1.wasKlicked())
  {
    sendState();
  }
  if (button1.wasPressed())
  {
    sendState();
  }
  if (button1.wasDoubleKlicked())
  {
    ESP.restart();
  }
  static bool onceUp=true;
  
  static bool onceDown=true;
  
  
  if (bDown.isPressed())
  {
    if(onceDown==false)
    {
      onceDown = true;
      client.publish(Zimmer "/Thermostat/Ab", "ON");
    }
  }
  else if (bDown.wasKlicked())
  {
    if(onceDown==false)
    {
      onceDown = true;
      client.publish(Zimmer "/Thermostat/Ab", "ON");
    }
  }
  else
  {
    if(onceDown)
    {
      onceDown=false;
      client.publish(Zimmer "/Thermostat/Ab", "OFF");
    }
  }
  if (bUp.isPressed())
  {
    if(onceUp==false)
    {
      onceUp = true;
      client.publish(Zimmer "/Thermostat/Auf", "ON");
    }
  }
  else if (bUp.wasKlicked())
  {
    if(onceUp==false)
    {
      onceUp = true;
      client.publish(Zimmer "/Thermostat/Auf", "ON");
    }
  }
  else
  {
    if(onceUp)
    {
      onceUp=false;
      client.publish(Zimmer "/Thermostat/Auf", "OFF");
    }
  }

  static unsigned long networkMinute = 0;
  if ((now - networkMinute) > 60000)
  {
    networkMinute = now;
    if (PingArrived) // only activate network timeout if at least one Ping has arrived
    {
      networkTimeout++; // this is reset whenever an mqtt network ping arrives
    }
  }
  if (networkTimeout > 5) // 5 minute timeout
  {
    DebugPrintf("network Timeout %d\n", networkTimeout);
    ESP.restart();
  }
}

void updateDisplay()
{
    display.clearDisplay();
    //display.drawBitmap(0,0,logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);

    display.setFont(&FreeSansBold18pt7b);
    int16_t  x1, y1;
    uint16_t w, h;
    char string[50];
    sprintf(string,"%3.1f",istTemperatur);
    display.getTextBounds(string, 0, 0, &x1, &y1, &w, &h);
    
    display.setCursor(80-w, 25);
    display.print(string);
    display.setCursor(92, 25);
    display.print("C");
    display.setFont();
    display.drawCircle(89,4,4,1);
    display.drawCircle(89,4,3,1);
    display.drawCircle(89,4,2,1);
    display.setCursor(20, 30);

    if (fileChanged && (now - fileChangeTime) < 10000)
    {
      display.print("soll:");
      display.print(sollTemperatur, 1);
    }
    else
    {
      display.print("h:");
      display.print(istHumidity, 1);
    }
    if (isOn)
    {
      display.print("  On");
    }
    else
    {
      display.print("  Off");
    }
    if (istTemperatur >= (sollTemperatur + hysterese))
    {
      heating = false;
    }
    if (istTemperatur <= (sollTemperatur - hysterese))
    {
      if(isOn)
      {
        heating = true;
      }
    }
    if(heating)
    {
        display.drawBitmap(100,40,epd_bitmap_flame, 17, 20, 1);
    }

    display.display();
}
