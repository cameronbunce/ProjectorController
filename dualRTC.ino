// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <RTCZero.h>

const char SETPOWERON[] = "*pow=on#";     // On
const char SETPOWEROFF[] = "*pow=off#";   // Off

const char SETVGA[] = "*sour=RGB#";       // source to VGA
const char SETHDMI[] = "*sour=hdmi#";     // Source to HDMI

const char GETMODEL[] = "*modelname=?#";  // poll model name
const char GETPOWER[] = "*pow=?#";        // poll power state
const char GETLAMPHOURS[] = "*ltim=?#";   // poll lamp 1 hours 
const char GETLAMP2HOURS[] = "*ltim2=?#"; // poll lamp 2 hours
const char GETLAMPMODE[] = "*lampm=?#";   // poll lamp mode status
const char GETSOURCE[] = "*sour=?#";      // poll current source
const char GETMUTE[] = "*mute=?#";        // poll mute status 
const char GETVOLUME[] = "*vol=?#";       // poll volume status 


void testAlarm(void);
void nextEvent(void);
void projectorOn(void);
void projectorOff(void);
void synchronize(void);


RTC_PCF8523 rtc;
RTCZero m0rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  DateTime set;

  while (!Serial) {
    delay(1);  // for Leonardo/Micro/Zero
  }

  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  else {
    Serial.println("PCF8523 connected");
  }
    //start communication with projector at Serial1 
  Serial1.begin(115200);


  if (! rtc.initialized()) {
    Serial.println("RTC is NOT configured! Setting Time.");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  else {
    Serial.println("Using existing clock settings from PCF8523.");
    
  }
  
  m0rtc.begin();
  set = rtc.now();
  m0rtc.setTime(set.hour(), set.minute(), set.second());
  m0rtc.setDate(set.day(), set.month(), (set.year()-2000 ) );
  m0rtc.setAlarmTime(set.hour(), ((set.minute()+1)%59), set.second());
  m0rtc.enableAlarm(m0rtc.MATCH_HHMMSS);
  m0rtc.attachInterrupt(testAlarm);
}

void loop () {

}

void testAlarm()
{
  DateTime now = rtc.now();
  Serial.println("Real Time Clocks synchronized and interrupts functional.");
  Serial.print("Current time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  nextEvent();
}

void nextEvent()
{
  Serial.println("Seting alarm for next event");
  unsigned int compare;
  compare=m0rtc.getHours();
  if (compare < 7)
  {
    Serial.print("The next event is to start the room at 7am.");
    m0rtc.setAlarmTime(7, 0, 0);
    m0rtc.enableAlarm(m0rtc.MATCH_HHMMSS);
    m0rtc.attachInterrupt(projectorOn);
    
  }
  else if ( compare < 19 )
  {
    Serial.print("The next event is to turn off the room at 7pm.");
    m0rtc.setAlarmTime(19,0,0);
    m0rtc.enableAlarm(m0rtc.MATCH_HHMMSS);
    m0rtc.attachInterrupt(projectorOff);
  }
  else 
  {
    Serial.print("The next event is to synchronize the clocks at midnight.");
    m0rtc.setAlarmTime(0,0,0);
    m0rtc.enableAlarm(m0rtc.MATCH_HHMMSS);
    m0rtc.attachInterrupt(synchronize);
  }
}


void projectorOn(){
  Serial1.println();
  Serial1.println(SETPOWERON);
  nextEvent();
}
void projectorOff(){
  Serial1.println();
  Serial1.println(SETPOWEROFF);
  nextEvent();
}
void synchronize(){
  DateTime set;
  set = rtc.now();
  m0rtc.setTime(set.hour(), set.minute(), set.second());
  m0rtc.setDate(set.day(), set.month(), (set.year()-2000 ) );
  nextEvent();
}
