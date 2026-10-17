// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

// You can use any (4 or) 5 pins 
#define SCLK_PIN 13 //yellow
#define MOSI_PIN 11 //blue (din)
#define DC_PIN   7 //green
#define CS_PIN   9 //orange
#define RST_PIN  8 //white


// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS  .5

Adafruit_SSD1351 OLED = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

PulseOximeter pox;

uint32_t tsLastReport = 0;
int sec = 0;

int red_count = 0;
int red_light = 0;
int last_red_light = 0;
int green_count = 0;
int issues_count = 0;

int spo2 = pox.getSpO2();
int last_spo2 = -2;
int second_last_spo2 = -1;
int third_last_spo2 = -1;
int fourth_last_spo2 = -1;
int last_spo2_connection = -1;

int heart = pox.getHeartRate();
int last_heart = 0;
int second_last_heart = 0;
int third_last_heart = 0;
int fourth_last_heart = 0;

int reconnect_time = 0;
int times_through = 0;
int ramp = 0;

bool in = true;
bool reconnectBool = false;
bool menu = true;
bool yikes = false;
bool first_yike_fix = true;
bool not_first_time = false;

void warning()
{
  OLED.fillScreen(RED);
  OLED.setCursor(2, 20);
  OLED.setTextColor(WHITE);
  OLED.setTextSize(3);
  OLED.println("WARNING");
  OLED.setTextSize(2);
  OLED.setCursor (37, 50); 
  OLED.println("SpO2%: ");
  OLED.setCursor(50, 70);
  OLED.println(spo2);
}

void checkConnection()
{
  OLED.setTextColor(BLACK);
  OLED.setTextSize(2);
  OLED.setCursor(35, 10);
  OLED.println("Check");
  OLED.setCursor(5, 30); 
  OLED.println("Connection");
  OLED.setCursor(7, 60); 
  OLED.println("Last SpO2:"); 
  OLED.setCursor(47, 80);
  OLED.print(last_spo2_connection);
  OLED.println("%");
}

void reconnect()
{
  OLED.setTextColor(WHITE);
  OLED.setTextSize(2);
  OLED.setCursor(4, 45);
  OLED.println("Connection");
  OLED.setCursor(36, 65);
  OLED.println("Fixed"); 
}

void mainScreen()
{
  //O2 Saturation
  OLED.setTextColor(GREEN);
  OLED.setTextSize(2);
  OLED.setCursor (0, 0); 
  OLED.print("SpO2:");
  OLED.setCursor (0, 17); 
  OLED.print(spo2); //spo2
  OLED.println("%");

  //Heart Rate
  OLED.setTextColor(RED);
  OLED.setCursor (0, 36); 
  OLED.println("Heart");
  OLED.setCursor (68, 36);
  OLED.println("Rate:");
  OLED.setCursor (0, 53); 
  OLED.println(heart); //heart
}

void noise() //repeated increasing pitch tone 
{
  if (ramp <= 6) 
  {
    if (times_through % 2 == 0)
    {
      tone(5, 75, 1000);
    }
    if (times_through % 2 == 1)
    {
      tone(5, 175, 1000);
    }
  }
  if (ramp > 6 && ramp <= 12)
  {
    if (times_through % 2 == 0)
    {
      tone(5, 125, 1000);
    }
    if (times_through % 2 == 1)
    {
      tone(5, 275, 1000);
    }
  }
  if (ramp > 12 && ramp <= 18)
  {
    if (times_through % 2 == 0)
    {
      tone(5, 200, 1000);
    }
    if (times_through % 2 == 1)
    {
      tone(5, 350, 1000);
    }
  }
  if (ramp > 18 && ramp <= 24)
  {
    if (times_through % 2 == 0)
    {
      tone(5, 250, 1000);
    }
    if (times_through % 2 == 1)
    {
      tone(5, 500, 1000);
    }
  }
  if (ramp > 24 && ramp <= 34)
  {
    if (times_through % 2 == 0)
    {
      tone(5, 350, 1000);
    }
    if (times_through % 2 == 1)
    {
      tone(5, 650, 1000);
    }
  }
  if (ramp > 34 && ramp <= 40)
  {
    if (times_through % 2 == 0)
    {
      tone(5, 425, 1000);
    }
    if (times_through % 2 == 1)
    {
      tone(5, 825, 1000);
    }
  }
  if (ramp > 40 && ramp <= 46)
  {
    if (times_through % 2 == 0)
    {
      tone(5, 350, 1000);
    }
    if (times_through % 2 == 1)
    {
      tone(5, 650, 1000);
    }
  }
  if (ramp > 46 && ramp <= 52)
  {
    if (times_through % 2 == 0)
    {
      tone(5, 250, 1000);
    }
    if (times_through % 2 == 1)
    {
      tone(5, 500, 1000);
    }
  }
  if (ramp > 52 && ramp <= 58)
  {
    if (times_through % 2 == 0)
    {
      tone(5, 200, 1000);
    }
    if (times_through % 2 == 1)
    {
      tone(5, 350, 1000);
    }
  }
  if (ramp > 58 && ramp <= 64)
  {
    if (times_through % 2 == 0)
    {
      tone(5, 125, 1000);
    }
    if (times_through % 2 == 1)
    {
      tone(5, 275, 1000);
    }
  }
  if (ramp == 65)
  {
    ramp = 0;
  }
  ramp++;
}

void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup() 
{
  Serial.begin(115200);

  OLED.begin();

  if (!pox.begin()) 
  {
    OLED.fillScreen(BLACK);
    OLED.setTextSize(2);
    OLED.setTextColor(RED);
    OLED.setCursor(30, 5);
    OLED.println("FAILED");
    OLED.setCursor(52, 25);
    OLED.println("TO");
    OLED.setCursor(35, 45);
    OLED.println("START");
    OLED.setTextColor(WHITE);
    OLED.setCursor(35, 85);
    OLED.println("RESET");
    OLED.setCursor(30, 105);
    OLED.println("DEVICE");
    for(;;);
  } 
  else 
  {
      Serial.println("SUCCESS");
      OLED.fillScreen(BLACK);
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()  
{
  pox.update();
  spo2 = pox.getSpO2();
  heart = pox.getHeartRate();
  sec = millis() / 1000;
  if (sec - tsLastReport > REPORTING_PERIOD_MS) //1 second intervals
  {
    if (menu == true) //reset screen and print menu
    {
      OLED.fillScreen(BLACK);
      mainScreen();
      menu = false;
    }
    if (spo2 <= 0 || spo2 > 100) //check if spo2 % is not 1-100 (invalid #) and save last valid spo2 %
    {
      if (in == true)
      {
        last_spo2_connection = last_spo2;
        in = false;
      }
      issues_count++;
    }
    if (spo2 == last_spo2 && heart == last_heart && in == true) //check for bad connection
    {
      last_spo2_connection = last_spo2;
      in = false;
      if (spo2 > 0 && spo2 <= 100) //repeating value issue count
      {
        issues_count++; 
      }
    }
    if (spo2 > 0 && spo2 <= 100) //check for everything working properly
    {
      if (spo2 != last_spo2 || heart != last_heart) 
      {
        issues_count = 0;
      }
      in = true;
    }
    if (spo2 <= 90 && spo2 > 0) //danger spo2 %
    {
      red_count++;
      green_count = 0;
    }
    else if (spo2 <= 100 && spo2 > 90) //safe spo2 %
    {
      green_count++;
      red_count = 0;
    }
    if (green_count >= 5) //safe spo2 % for 5 seconds
    {
      red_light = 0;
    }
    if (red_count >= 5) //dangerous spo2 % for 5 seconds
    {
      red_light++;
    }
    if (red_light == 35 && issues_count < 10) //prepare to print warning
    {
      OLED.fillScreen(RED);
      ramp = 0;
    }
    if (red_light >= 35 && issues_count < 10) //After 40 seconds of dangerous spo2 %, warrning prints
    {
      warning();
      noise();
      yikes = true;
    }
    if (red_light == 0 && last_red_light >= 35) //if warning just turned off
    {
      menu = true;
      yikes = false;
      first_yike_fix = false;
    }
    if (issues_count == 10 && spo2 == last_spo2) //prepare to print check connection
    {
      OLED.fillScreen(YELLOW);
    }
    if (issues_count >= 10 && spo2 == last_spo2) //After 10 seconds of no connection print check connection
    {
      checkConnection();
      yikes = true;
      reconnectBool = true;
    }
    if (issues_count == 0 && reconnectBool == true && spo2 != last_spo2) //print reconnection screen
    {
      OLED.fillScreen(GREEN);
      reconnect();
      reconnect_time = times_through;
      reconnectBool = false;
      first_yike_fix = false;
    }
    if (reconnect_time + 1 >= times_through && not_first_time == true) //keep printing reconnect screen for 2 seconds
    {
      reconnect();
    }
    if (reconnect_time + 1 == times_through) //print the menu on the next loop around
    {
      menu = true;
      yikes = false;
    }
    
    if (yikes == false) //if not printing warning, check connection, or reconnection, update values on menu
    {
      if (spo2 != last_spo2 && first_yike_fix == true && spo2 >= 0 && spo2 <= 100)
      {
        if (spo2 == 0 && last_spo2 == 0 && second_last_spo2 == 0 && third_last_spo2 == 0 && fourth_last_spo2 ==0)
        {
          OLED.setTextColor(BLACK, BLACK);
          OLED.setTextSize(2);
          OLED.setCursor (0, 17); 
          OLED.print("  %%%");
          OLED.setTextColor(GREEN, BLACK);
          OLED.setCursor (0, 17);
          OLED.print(spo2); //spo2
          OLED.println("%");
        }
        else if (spo2 <= 100 && spo2 >= 0)
        {
          OLED.setTextColor(BLACK, BLACK);
          OLED.setTextSize(2);
          OLED.setCursor (0, 17); 
          OLED.print("  %%%");
          OLED.setTextColor(GREEN, BLACK);
          OLED.setCursor (0, 17);
          OLED.print(spo2); //spo2
          OLED.println("%");
        }
      }
      if (heart != last_heart && first_yike_fix == true)
      {
        int total = fourth_last_heart + third_last_heart + second_last_heart + last_heart;
        double average = total / 4;
        if ((average + 12) > heart && (average - 12) < heart)
        {
          OLED.setTextColor(BLACK, BLACK);
          OLED.setTextSize(2);
          OLED.setCursor (0, 53); //the following draws a black box
          OLED.print(" 000");
          OLED.setCursor (0, 53); 
          OLED.print(" 111");
          OLED.setCursor (0, 53); 
          OLED.print(" 222");
          OLED.setCursor (0, 53); 
          OLED.print(" 333");
          OLED.setCursor (0, 53); 
          OLED.print(" 444");
          OLED.setCursor (0, 53); 
          OLED.print(" 555");
          OLED.setCursor (0, 53); 
          OLED.print(" 666");
          OLED.setCursor (0, 53); 
          OLED.print(" 777");
          OLED.setCursor (0, 53); 
          OLED.print(" 888");
          OLED.setCursor (0, 53); 
          OLED.print(" 999");
          OLED.setTextColor(RED, BLACK);
          OLED.setCursor (0, 53);
          OLED.println(heart); //heart
        }
      }
      if (times_through >= reconnect_time + 2)
      {
        OLED.setTextColor (WHITE);
        OLED.setTextSize(2);
        OLED.setCursor (13, 83); 
        OLED.print("Run Time:");
      }
      first_yike_fix = true;
    }
    //update all last value variables
    last_spo2 = spo2;  
    fourth_last_heart = third_last_heart;
    third_last_heart = second_last_heart;
    second_last_heart = last_heart;
    last_heart = heart;
    last_red_light = red_light;
    
    if (times_through >= 2)
    {
      not_first_time = true;
      if (times_through < 60) //position for seconds before minutes
      {
        if (times_through < 10)
        {
          OLED.setCursor (50, 103);
          OLED.setTextColor (WHITE, BLACK);
          OLED.print(times_through);
          OLED.println("s");
        }
        else
        {
          OLED.setCursor (45, 103);
          OLED.setTextColor (WHITE, BLACK);
          OLED.print(times_through);
          OLED.println("s");
        }
      }
      if (times_through >= 60 && times_through < 3600) //position for minutes before hours
      {
        int secs = times_through % 60;
        if (secs == 0)
        {
          OLED.setCursor (25, 103);
          OLED.setTextColor (BLACK, BLACK);
          OLED.print("ssssssssss");
          OLED.setCursor (28, 103);
          OLED.print("1");
          OLED.setCursor (28, 103);
          OLED.print("2");
          OLED.setCursor (28, 103);
          OLED.print("3");
          OLED.setCursor (28, 103);
          OLED.print("4");
          OLED.setCursor (28, 103);
          OLED.print("5");
          OLED.setCursor (28, 103);
          OLED.print("6");
          OLED.setCursor (28, 103);
          OLED.print("7");
          OLED.setCursor (28, 103);
          OLED.print("8");
          OLED.setCursor (28, 103);
          OLED.print("9");
        }
        if (secs < 10)
        {
          OLED.setCursor (28, 103);
          OLED.setTextColor (WHITE, BLACK);
          OLED.print(times_through / 60);
          OLED.print("m ");
          OLED.print(secs); 
          OLED.print("s"); 
        }
        else if (secs >= 10)
        {
          OLED.setCursor (25, 103);
          OLED.setTextColor (WHITE, BLACK);
          OLED.print(times_through / 60);
          OLED.print("m ");
          OLED.print(secs);
          OLED.print("s");
        }
      }
    }
    times_through++;
    tsLastReport = millis() / 1000;
  }
}
