/* 
Command Definition table for projector commands
for projector control in scheduling application

Cameron Bunce 2017.01.04
Red Ventures 

BenQ projectors

*/


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

