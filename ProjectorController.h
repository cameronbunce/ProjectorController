// Schedule types for projector scheduler
// TIME and DATE data types defined in RTCInt.h 
// as found 
// https://dev.toppers.jp/trac_user/contrib/browser/rtos_arduino/trunk/arduino_lib/libraries/RTC/src/
// pulled 2017.01.03 version 136 ( matching .cpp also same time/place/version ) 
//
// RTCInt.h used for M0 internal interrupt 
// RTClib.h used for PCF8523 sync for backup of internal timer
// BENQ.h containing character arrays of RS232 commands for projector 

#ifndef PROJECTORCONTROLLER_H_
#define PROJECTORCONTROLLER_H_

#include <RTCInt.h>
//#include <RTClib.h>
#include <BENQ.h>

typedef struct node {
  //on or off
  bool pow;
  
  // time
  TIME myTime; 
  DATE myDate; //possibly used in the future to preserve the same datatype for schedule overrides
  //next event
  struct node *next;
  
} SchedPoint;

/*----------------------------------------------------------
// This operator is a horribly inaccurate test hack that is 
//not aware of the difference of days in months or days in years
//----------------------------------------------------------*/
bool operator <(const SchedPoint& x, const SchedPoint& y) {
	return (x.myTime.second + (x.myTime.minute *60) + (x.myTime.hour *3600 ) + (x.myDate.day * 86400) + ( x.myDate.month * 2592000 ) + ( (x.myDate.year - 2017) * 31104000 ))<(y.myTime.second + (y.myTime.minute *60) + (y.myTime.hour *3600 ) + (y.myDate.day * 86400) + ( y.myDate.month * 2592000 ) + ( (y.myDate.year - 2017) * 31104000 ));
}
  
class ProjectorController {
  public:
  void init();
  void setOverride(SchedPoint *newOverride);
  
  private:
  SchedPoint ProjectorSchedule;
  RTCInt M0rtc;
  //RTC_PCF8523 rtcpcf;
  
  bool checkPreviousSchedule();
  void getNextEvent();
  void initBackup();
  void addOverride(SchedPoint *start, SchedPoint *newOverride);
  void setDefault();
  void synchronize();
  void projectorOn();
  void projectorOff();
};

#endif