//------------------------------------------------------------//

//Biblioteker til LED-koden
#include <Adafruit_DotStar.h>
#include <SPI.h>

//------------------------------------------------------------//

//Biblioteker til ZX_Sensoren
#include <Wire.h>
#include <ZX_Sensor.h>

//Variabler til ZX_Sensoren
const int ZX_ADDR = 0x10;    // ZX Sensor I2C address

ZX_Sensor zx_sensor = ZX_Sensor(ZX_ADDR);
GestureType gesture;
uint8_t gesture_speed;

//------------------------------------------------------------//

//Antallet af Pixels i hver strip
#define NUMPIXELS1 20 // Number of LEDs in strip
#define NUMPIXELS2 20 // Number of LEDs in strip
#define NUMPIXELS3 20 // Number of LEDs in strip
#define NUMPIXELS4 20 // Number of LEDs in strip

//LEDSTRIP_1__OVERTAG
#define DATAPIN1   2
#define CLOCKPIN1  1

Adafruit_DotStar strip1 = Adafruit_DotStar(
  NUMPIXELS1, DATAPIN1, CLOCKPIN1, DOTSTAR_BRG);

//LEDSTRIP_2__UNDERTAG
#define DATAPIN2   5
#define CLOCKPIN2  4

Adafruit_DotStar strip2 = Adafruit_DotStar(
  NUMPIXELS2, DATAPIN2, CLOCKPIN2, DOTSTAR_BRG);

//LEDSTRIP_3__MURSTEN
#define DATAPIN3    12
#define CLOCKPIN3   11

Adafruit_DotStar strip3 = Adafruit_DotStar(
  NUMPIXELS3, DATAPIN3, CLOCKPIN3, DOTSTAR_BRG);

//LEDSTRIP_4__BALJE
#define DATAPIN4    8
#define CLOCKPIN4   9

Adafruit_DotStar strip4 = Adafruit_DotStar(
  NUMPIXELS4, DATAPIN4, CLOCKPIN4, DOTSTAR_BRG);

//------------------------------------------------------------//

//Knap variabler
const int buttonPin = 7;     // Pin-indgangen på Arduinoen
int buttonState = 0;         // Knappens tilstand

//------------------------------------------------------------//

void setup() {
    uint8_t ver;

    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
    #endif

    strip1.begin(); // Initialize pins for output
    strip2.begin();
    strip3.begin(); // Initialize pins for output
    strip4.begin();
    strip1.show(); // Turn all LEDs off ASAP
    strip2.show(); // Turn all LEDs off ASAP
    strip3.show(); // Turn all LEDs off ASAP
    strip4.show(); // Turn all LEDs off ASAP

    for (int i = 0; i < 30; i++) {
        strip1.setPixelColor(i, 0, 0, 255);
    }
    for (int j = 0; j < 30; j++) {
        strip2.setPixelColor(j, 0, 255, 0);
    }
    for (int i = 0; i < 30; i++) {
        strip3.setPixelColor(i, 0, 0, 0);
    }
    for (int j = 0; j < 30; j++) {
        strip4.setPixelColor(j, 0, 0, 0);
    }
    strip1.setBrightness(50);
    strip1.show(); // Turn all LEDs off ASAP
    strip2.show(); // Turn all LEDs off ASAP
    strip3.show(); // Turn all LEDs off ASAP
    strip4.show(); // Turn all LEDs off ASAP

    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT);

    Serial.begin(9600);
    Serial.println();
    Serial.println("----------------------------------------");
    Serial.println("SparkFun/GestureSense - I2C Gesture Demo");
    Serial.println("Note: higher 'speed' numbers mean slower");
    Serial.println("----------------------------------------");

    // Initialize ZX Sensor (configure I2C and read model ID)
    if (zx_sensor.init()) {
        Serial.println("ZX Sensor initialization complete");
    } else {
        Serial.println("Something went wrong during ZX Sensor init!");
    }

    // Read the model version number and ensure the library will work
    ver = zx_sensor.getModelVersion();
    if (ver == ZX_ERROR) {
        Serial.println("Error reading model version number");
    } else {
        Serial.print("Model version: ");
        Serial.println(ver);
    }
    if (ver != ZX_MODEL_VER) {
        Serial.print("Model version needs to be ");
        Serial.print(ZX_MODEL_VER);
        Serial.print(" to work with this library. Stopping.");
        while (1);
    }

    // Read the register map version and ensure the library will work
    ver = zx_sensor.getRegMapVersion();
    if (ver == ZX_ERROR) {
        Serial.println("Error reading register map version number");
    } else {
        Serial.print("Register Map Version: ");
        Serial.println(ver);
    }
    if (ver != ZX_REG_MAP_VER) {
        Serial.print("Register map version needs to be ");
        Serial.print(ZX_REG_MAP_VER);
        Serial.print(" to work with this library. Stopping.");
        while (1);
    }
}

//------------------------------------------------------------//

float acc = 255;
float mod = 1;

float acc2 = 128;
float mod2 = -1;

float acc3 = 0;
float mod3 = 1;

int behaviour = 0;

float timer = 0;
float timerMod = 0.1;

bool swipeMode = true;

//------------------------------------------------------------//

void loop() {
    timer = timer + timerMod;

    acc = acc + mod;
    acc2 = acc2 + mod2;
    acc3 = acc3 + mod3;

    // If there is gesture data available, read and print it
    if (zx_sensor.gestureAvailable()) {
        gesture = zx_sensor.readGesture();
        gesture_speed = zx_sensor.readGestureSpeed();
        switch (gesture) {
            case NO_GESTURE:
                Serial.println("No Gesture");
                break;
            case RIGHT_SWIPE:
                acc2 = 0;
                Serial.println("Right Swipe");
                timer = 0;
                if(swipeMode){
                  behaviour = 1; 
                }
                //Serial.print("Right Swipe. Speed: ");
                //Serial.println(gesture_speed, DEC);
                break;
            case LEFT_SWIPE:
                acc2 = 0;
                Serial.println("Left Swipe");
                timer = 0;
                if(swipeMode){
                  behaviour = 1; 
                }
                //Serial.print("Left Swipe. Speed: ");
                //Serial.println(gesture_speed, DEC);
                break;
            case UP_SWIPE:
                Serial.print("Up Swipe. Speed: ");
                Serial.println(gesture_speed, DEC);
                break;
            default:
                break;
        }
    }


    //SwitchFunction der bestemmer hvilken tilstand lysene skal være i
    switch (behaviour) {
        //IdleMode
        case 0:      
        swipeMode = true;  
            for (int j = 0; j < 30; j++) {
                strip2.setPixelColor(j, 0, 0, acc);
            }
           
            
            for (int j = 0; j < 10; j++) {
                strip3.setPixelColor(0, 0, 0, acc);
                strip3.setPixelColor(1, 0, 0, acc);
                strip3.setPixelColor(2, 0, 0, acc);
                strip3.setPixelColor(3, 0, 0, acc);
                strip3.setPixelColor(4, 0, 0, acc);
                strip3.setPixelColor(5, 0, 0, acc);
            }
            for (int j = 0; j < 10; j++) {
                strip3.setPixelColor(6, 0, 0, acc2);
                strip3.setPixelColor(7, 0, 0, acc2);
                strip3.setPixelColor(8, 0, 0, acc2);
                strip3.setPixelColor(9, 0, 0, acc2);
                strip3.setPixelColor(10, 0, 0, acc2);
                strip3.setPixelColor(11, 0, 0, acc2);
            }
            for (int j = 0; j < 10; j++) {
                strip3.setPixelColor(12, 0, 0, acc3);
                strip3.setPixelColor(13, 0, 0, acc3);
                strip3.setPixelColor(14, 0, 0, acc3);
                strip3.setPixelColor(15, 0, 0, acc3);
                strip3.setPixelColor(16, 0, 0, acc3);
            }
            
            
            for (int j = 0; j < 30; j++) {
                strip4.setPixelColor(j, 0, 0, acc);
            }

            if (acc >= 175) {
                mod = -1;
            }
            if (acc <= 25) {
                mod = 1;
            }
            if (acc2 >= 255) {
                mod2 = -1;
            }
            if (acc2 <= 0) {
                mod2 = 1;
            }
            if (acc3 >= 255) {
              mod3 = -1;
            }
            if (acc3 <= 0) {
              mod3 = 1;
            }

            if (Serial.available() > 0) {
                behaviour = Serial.read();
            }
            break;

        //Swiping
        case 1:
            swipeMode = false;
            mod = 1;
            mod2 = 1;

            for (int j = 0; j < NUMPIXELS1; j++) {
                strip2.setPixelColor(j, acc2, acc2, acc);
                strip4.setPixelColor(j, acc2, acc2, acc);
            }
            if (acc >= 250) {
                mod = -1;
            }
            if (acc <= 0) {
                mod = 1;
            }
            if (acc2 >= 100) {
                mod2 = -1;
            }
            if (acc2 <= 0) {
                mod2 = 1;
            }
            
            if (Serial.available() > 0) {
                behaviour = Serial.read();
            }

            break;

        //Donation Transition
        case 2:
            swipeMode = false;
            
            for (int j = 0; j < 30; j++) {
                strip2.setPixelColor(j, 215, 255, 0);
                strip3.setPixelColor(j, 215, 255, 0);
                strip4.setPixelColor(j, 215, 255, 0);
                delay(150);
                if (j >= 29) {
                    behaviour = 3;
                }
            }

            if (acc >= 100) {
                mod = -0.5;
            }
            if (acc <= 0) {
                mod = 0.5;
            }

            if (Serial.available() > 0) {
                behaviour = Serial.read();
            }
            break;

        //Donation Idle
        case 3:
            swipeMode = true;
            
            for (int j = 0; j < NUMPIXELS1; j++) {
                strip2.setPixelColor(j, acc2, acc, 0);
            }

            for (int j = 0; j < NUMPIXELS2; j++) {
                strip3.setPixelColor(j, acc2, acc, 0);
            }

            for (int j = 0; j < NUMPIXELS3; j++) {
                strip4.setPixelColor(j, acc2, acc, 0);
            }

            if (acc >= 215) {
                mod = -0.5;
            }
            if (acc <= 50) {
                mod = 0.5;
            }

            if (acc2 >= 255) {
                mod2 = -0.5;
            }
            if (acc2 <= 50) {
                mod2 = 0.5;
            }

            if (Serial.available() > 0) {
                behaviour = Serial.read();
            }
            break;

        //Swipe is Ready!
        case 4:
            swipeMode = true;
            
            for (int j = 0; j < 30; j++) {
                strip4.setPixelColor(j, acc, 0, acc);
            }

            if (acc >= 215) {
                mod = -1;
            }
            if (acc <= 50) {
                mod = 1;
            }
            if (acc2 >= 50) {
                mod2 = -1;
            }
            if (acc2 <= 10) {
                mod2 = 1;
            }
            if (acc3 >= 170) {
                mod = -1;
            }
            if (acc3 <= 130) {
                mod = 1;
            }

            if (Serial.available() > 0) {
                behaviour = Serial.read();
            }
            break;
    
         //VideoMode
         case 5:
            for (int j = 0; j < 30; j++) {
                strip2.setPixelColor(j, 0, 0, acc);
            }
           
            for (int j = 0; j < 10; j++) {
                strip3.setPixelColor(0, 0, 0, acc);
                strip3.setPixelColor(1, 0, 0, acc);
                strip3.setPixelColor(2, 0, 0, acc);
                strip3.setPixelColor(3, 0, 0, acc);
                strip3.setPixelColor(4, 0, 0, acc);
                strip3.setPixelColor(5, 0, 0, acc);
            }
            for (int j = 0; j < 10; j++) {
                strip3.setPixelColor(6, 0, 0, acc2);
                strip3.setPixelColor(7, 0, 0, acc2);
                strip3.setPixelColor(8, 0, 0, acc2);
                strip3.setPixelColor(9, 0, 0, acc2);
                strip3.setPixelColor(10, 0, 0, acc2);
                strip3.setPixelColor(11, 0, 0, acc2);
            }
            for (int j = 0; j < 10; j++) {
                strip3.setPixelColor(12, 0, 0, acc3);
                strip3.setPixelColor(13, 0, 0, acc3);
                strip3.setPixelColor(14, 0, 0, acc3);
                strip3.setPixelColor(15, 0, 0, acc3);
                strip3.setPixelColor(16, 0, 0, acc3);
            }
            
            
            for (int j = 0; j < 30; j++) {
                strip4.setPixelColor(j, 0, 0, acc);
            }

            if (acc >= 175) {
                mod = -1;
            }
            if (acc <= 25) {
                mod = 1;
            }
            if (acc2 >= 255) {
                mod2 = -1;
            }
            if (acc2 <= 0) {
                mod2 = 1;
            }
            if (acc3 >= 255) {
              mod3 = -1;
            }
            if (acc3 <= 0) {
              mod3 = 1;
            }

            if (Serial.available() > 0) {
                behaviour = Serial.read();
            }
            break;
         
    }
    strip1.show();
    strip2.show();
    strip3.show();
    strip4.show();

    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);

    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
        Serial.println("Button Pressed!");
        timer = 0;
        behaviour = 2;
        delay(300);
    }
}
