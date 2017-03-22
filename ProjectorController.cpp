#include <ProjectorController.h>
/**********************************************************
initialize schedule to defaults, in the future this 
might look to a backup stored on an SD card 
**********************************************************/

void init(void){
	DateTime syncnow;
	if(checkPreviousSchedule()){
		initBackup();
	}
	else{
		setDefault(ProjectorSchedule);
	}
	//start communication with projector at Serial1 
	Serial1.begin(115200);
	//rtcpcf.begin();			//external, battery backed clock
	M0rtc.begin(TIME_H24);	//internal to the M0
	/*syncnow = rtcpcf.now();
	M0rtc.setTime(syncnow.hour(), 0, syncnow.minute(), syncnow.second());
	M0rtc.setDate(syncnow.day(), syncnow.month(), syncnow.year());
	M0rtc.local_time.minute = 0;
	M0rtc.local_time.second = 0; // commented out to hold for method, pulling out the PCF RTC */
}

/**********************************************************
 
**********************************************************/

void setOverride(SchedPoint *newOverride){
	addOverride( ProjectorSchedule, newOverride);
}

/**********************************************************
future use - check SD card to see if there is a backup
of previous schedule, if found return true, else false 
**********************************************************/

bool checkPreviousSchedule(void){
	return false;
}
/**********************************************************
 here's the heavy lift
 check the time,
 we are concerned with four things: Midnight - clock sync
									7am - projector on
									7pm - projector off
									the next override
									
 we set the M0rtc alarm for the closest event
 and the callback to the appropriate function
 
**********************************************************/

void getNextEvent(){
	unsigned int compare;
	M0rtc.getDate();
	M0rtc.getTime();
	if ( ProjectorSchedule == NULL){
		// nothing abnormal scheduled, just run the ons and offs and syncs
		// we can assume a few things about our hours in this case
		compare=M0rtc.getHour();
		if (compare < 7){
			//set the timer for starting the room at 7
			M0rtc.local_time.hour = 7;
			M0rtc.enableArarm(HHMMSS,ALARM_INTERRUPT, projectorOn);
		}else if (compare < 19){
			//set the timer for turning the room off
			M0rtc.local_time.hour = 19;
			M0rtc.enableAlarm(HHMMSS,ALARM_INTERRUPT, projectorOff);
		}else{
			//set the timer to sync the clocks
			M0rtc.local_time.hour = 0;
			M0rtc.enableAlarm(HHMMSS,ALARM_INTERRUPT, synchronize);
		}
	}
	M0rtc.setAlarm();
}

/**********************************************************
 does nothing and is not currently called
 in the future would read SD card and load schedule and 
 overrides from file
 
 RIGHT NOW it just sets it to dafault to make sure things are not breakable
**********************************************************/
void initBackup(void){
	setDefault(ProjectorSchedule);
}

// paramaters: a pointer to either null or a node of type schedPoint and the schedPoint to add
//
// determine if the pointer is to NULL
// if that is the case, point to the new schedpoint, and make the new schedpoint point to null
// else look to see if the next SchedPoint is higher than the new schedpoint
//  iterate until schedpoint->next is higher than new override or null

void addOverride(SchedPoint *start, SchedPoint *newOverride){
	if(start != NULL){
		if( newOverride < start){
			newOverride->next = &start;
			start = *newOverride;
		}
		else{
			addOverride(start->next, newOverride);
		}
	}
	else{
		newOverride->next = NULL;
		start = *newOverride;
		
	}
}

/**********************************************************
Build projector control calendar with a basic week of 
Mon-Fri 7am-6pm on, and otherwise off 

so for mon-fri set first SchedPoint->pow = 1 and ->myTime to 7am 
create new SchedPoint at Mon->next and set pow to 0 and myTime to 7pm 

**********************************************************/
/**********************************************************
// Previous implementation 
// deprecated previous use of a struct for the calendar 
// in favor of using the getnextevent() function to just know
// when on and off times are

void setDefault(SCHEDULE *setme){
  TIME timeOn;
  timeOn.hour = 7;
  timeOn.minute = 0;
  timeOn.second = 0;
  //timeOn.Tmode = '1'; //not even sure if this is used
  TIME timeOff;
  timeOff = timeOn;
  timeOff.hour = 19;
  new SchedPoint* SchedOff;
  SchedOff->pow = 0;
  SchedOff->myTime = timeOff;
  SchedOff->next = NULL;
  
  for(int i=0; i<7; i++){
    setme->Week[i]->pow = 1;
    setme->myTime = timeOn;
    setme->Week[i]->next = SchedOff;
  }
  setme->Override = NULL;
}
**********************************************************/

void setDefault(){
	ProjectorSchedule = NULL;
}
// call back functions
void synchronize(){//blotting this out for now 
	/*
	DateTime syncnow;
	syncnow = rtcpcf.now();
	M0rtc.setTime(syncnow.hour(), 0, syncnow.minute(), syncnow.second());
	M0rtc.setDate(syncnow.day(), syncnow.month(), syncnow.year());
	RTC->MODE2.INTFLAG.bit.ALARM0=1; //clearing alarm0 flag
	getNextEvent();
	*/
}

void projectorOn(){
	Serial1.println();
	Serial1.println(SETPOWERON);
	RTC->MODE2.INTFLAG.bit.ALARM0=1; //clearing alarm0 flag
	getNextEvent();
}

void projectorOff(){
	Serial1.println();
	Serial1.println(SETPOWEROFF);
	RTC->MODE2.INTFLAG.bit.ALARM0=1; //clearing alarm0 flag
	getNextEvent();
	
}