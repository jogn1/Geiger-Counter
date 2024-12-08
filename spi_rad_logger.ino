#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR   0x3C

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define LOG_PERIOD 15000  //Logging period in milliseconds, recommended value 15000-60000.
#define MAX_PERIOD 60000  //Maximum logging period without modifying this sketch

unsigned long counts;     //variable for GM Tube events
unsigned long ticks;
unsigned long cpm;        //variable for CPM
unsigned int multiplier;  //variable for calculation CPM in this sketch
unsigned long previousMillis;  //variable for time measurement

// int x2 = 5;

// float generateGaussian(float mean, float stdDev) {
//   // Generate two random floats between 0 and 1
//   float u1 = random(0, 10000) / 10000.0; // Convert to float [0, 1]
//   float u2 = random(0, 10000) / 10000.0; // Convert to float [0, 1]

//   // Apply Box-Muller Transform
//   float z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
//   // z0 is normally distributed with mean 0 and std deviation 1

//   return z0 * stdDev + mean; // Scale to desired mean and std deviation
// }

void setup() {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  ticks = 0;
  counts = 0;
  cpm = 0;
  multiplier = MAX_PERIOD / LOG_PERIOD;      //calculating multiplier, depend on your log period
  attachInterrupt(0, tube_impulse, FALLING); //define external interrupts 

  randomSeed(analogRead(0)); // Seed random generator for better randomness

  // Define mean and standard deviation
  float mean = 50.0;
  float stdDev = 10.0;

  // Generate and print Gaussian numbers
//   for (int i = 0; i < 50; i++) {
//     float value = generateGaussian(mean, stdDev);
// //    Serial.println(value);
//   }
}

void tube_impulse(){       //subprocedure for capturing events from Geiger Kit
  counts++;
}

void loop(){                                 //main cycle
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > LOG_PERIOD){
    previousMillis = currentMillis;
    cpm = counts * multiplier;
    
    ticks = counts;
  //  Serial.println(cpm);
    counts = 0;
  }


  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5,30);

  display.print("counts per minute:");
  display.println(cpm);

  display.setCursor(20,40);

  display.print("total ticks:");
  display.println(ticks);

  display.display();
}

// float mean(int aPin)
// {
//   const int arrayLen = 60;
//   static int readings[arrayLen];
  
//   static unsigned long sum;
//   int newRead;
//   static int index;

//   newRead = analogRead(aPin);

//   sum = sum - readings[index];

//   sum = sum + newRead;

//   readings[index] = newRead;

//   index = index + 1;

//   if (index > arrayLen - 1)
//   {
//     index = 0;
//   }

//   float mean = sum/arrayLen;

//   return mean;
// }

// long factorial(int x)
// {
//   x;
//   long result;
//   int y = x;
//   while(x > 1)
//   {
//     y = y*(x-1); 
  
//     x = x - 1;
//   }

//   return result;
// }
