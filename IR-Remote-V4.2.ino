#include <ir_NEC.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>

int relayOne = 4;
IRsend irsend(relayOne);

uint16_t rawData[67] = {9000, 4500, 650, 550, 650, 1650, 600, 550, 650, 550,
                        600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650,
                        600, 550, 650, 1650, 650, 1650, 650, 550, 600, 1650,
                        650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550,
                        650, 550, 650, 550, 600, 550, 650, 550, 650, 550,
                        650, 1650, 600, 550, 650, 1650, 650, 1650, 650, 1650,
                        650, 1650, 650, 1650, 650, 1650, 600};

uint8_t samsungState[kSamsungAcStateLength] = {
    0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
    0x01, 0xE2, 0xFE, 0x71, 0x40, 0x11, 0xF0};

#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif
#include "fauxmoESP.h"

// Rename the credentials.sample.h file to credentials.h and 
// edit it according to your router configuration
#define WIFI_SSID "NETGEAR34"
#define WIFI_PASS "kevinryan"

fauxmoESP fauxmo;

// -----------------------------------------------------------------------------

#define SERIAL_BAUDRATE     115200

//#define IRLightPin              4

#define LEDs          "LEDs"
#define Colors       "colors"
#define Modes            "modes"
#define Settings        "settings"

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void setup() {

  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);

    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();

    // LEDs
    //pinMode(IRLightPin, OUTPUT);
    //digitalWrite(IRLightPin, LOW);

    // Wifi
    wifiSetup();

    // By default, fauxmoESP creates it's own webserver on the defined port
    // The TCP port must be 80 for gen3 devices (default is 1901)
    // This has to be done before the call to enable()
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices

    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    fauxmo.enable(true);

    // You can use different ways to invoke alexa to modify the devices state:
    // "Alexa, turn yellow lamp on"
    // "Alexa, turn on yellow lamp
    // "Alexa, set yellow lamp to fifty" (50 means 50% of brightness, note, this example does not use this functionality)

    // Add virtual devices
    fauxmo.addDevice(LEDs);
    fauxmo.addDevice(Colors);
    fauxmo.addDevice(Modes);
    fauxmo.addDevice(Settings);

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

        // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
        // Otherwise comparing the device_name is safer.

        if (strcmp(device_name, LEDs)==0) {
            irsend.sendNEC(0x00FF02FD);
        } else if (strcmp(device_name, Colors)==0) {
            if (value == 2) { //Dim Red
              irsend.sendNEC(0x00FF30CF);
            } else if (value == 5) { //Dim Green
              irsend.sendNEC(0x00FFB04F);
            } else if (value == 7) { //Dim Blue
              irsend.sendNEC(0x00FF708F);
            } else if (value == 10) { //Dim Yellow
              irsend.sendNEC(0x00FF10EF);
            } else if (value == 12) { //Dim Light Blue
              irsend.sendNEC(0x00FF906F);
            } else if (value == 15) { //Dim Purple
              irsend.sendNEC(0x00FF50AF);
            } else if (value == 17) { //Red
              irsend.sendNEC(0x00FF1AE5);
            } else if (value == 20) { //Green
              irsend.sendNEC(0x00FF9A65);
            } else if (value == 22) { //Blue
              irsend.sendNEC(0x00FFA25D);
            } else if (value == 25) { //White
              irsend.sendNEC(0x00FF22DD);
            } else if (value == 28) { //Red 2
              irsend.sendNEC(0x00FF2AD5);
            } else if (value == 30) { //Green 2
              irsend.sendNEC(0x00FFAA55);
            } else if (value == 33) { //Blue 2
              irsend.sendNEC(0x00FF926D);
            } else if (value == 35) { //White 2
              irsend.sendNEC(0x00FF12ED);
            } else if (value == 38) { //Orange
              irsend.sendNEC(0x00FF0AF5);
            } else if (value == 40) { //Turquoise
              irsend.sendNEC(0x00FF8A75);
            } else if (value == 43) { //Indigo
              irsend.sendNEC(0x00FFB24D);
            } else if (value == 45) { //Light Pink
              irsend.sendNEC(0x00FF32CD);
            } else if (value == 48) { //Yellow Orange
              irsend.sendNEC(0x00FF38C7);
            } else if (value == 51) { //Light Blue 2
              irsend.sendNEC(0x00FFB847);
            } else if (value == 53) { //Light Purple
              irsend.sendNEC(0x00FF7887);
            } else if (value == 56) { //Light Blue 3
              irsend.sendNEC(0x00FFF807);
            } else if (value == 58) { //Lime Green
              irsend.sendNEC(0x00FF18E7);
            } else if (value == 61) { //Light Blue
              irsend.sendNEC(0x00FF9867);
            } else if (value == 63) { //Pink
              irsend.sendNEC(0x00FF58A7);
            } else if (value == 66) { //Light Blue 4
              irsend.sendNEC(0x00FFD827);
            }
        } else if (strcmp(device_name, Modes)==0) {
            if (value == 2) { //Flash
              irsend.sendNEC(0x00FFD02F);
            } else if (value == 5) { //Fade
              irsend.sendNEC(0x00FFE01F);
            } else if (value == 7) { //RGB Fade
              irsend.sendNEC(0x00FF609F);
            } else if (value == 10) { //RGB Blink
              irsend.sendNEC(0x00FF20DF);
            } else if (value == 12) { //Blink
              irsend.sendNEC(0x00FFA05F);
            } else if (value == 15) { //Random
              irsend.sendNEC(0x00FFF00F);
            }
        } else if (strcmp(device_name, Settings)==0) {
            if (value == 2) { //Brighten
              irsend.sendNEC(0x00FF3AC5);
            } else if (value == 5) { //Dim
              irsend.sendNEC(0x00FFBA45);
            } else if (value == 7) { //Speed Up
              irsend.sendNEC(0x00FFE817);
            } else if (value == 10) { //Slow Down
              irsend.sendNEC(0x00FFC837);
            } else if (value == 12) { //Freeze
              irsend.sendNEC(0x00FF827D);
            }
        }
    });

}

void loop() {

    // fauxmoESP uses an async TCP server but a sync UDP server
    // Therefore, we have to manually poll for UDP packets
    fauxmo.handle();

    // This is a sample code to output free heap every 5 seconds
    // This is a cheap way to detect memory leaks
    static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }

    // If your device state is changed by any other means (MQTT, physical button,...)
    // you can instruct the library to report the new state to Alexa on next request:
    // fauxmo.setState(ID_YELLOW, true, 255);

}
