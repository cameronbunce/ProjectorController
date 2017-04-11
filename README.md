# ProjectorController

Deprecating the complicating things for the time being. DualRTC accomplishes the guts without the shiny things.

This is based on the ATSAMD21G18, and using the external timer of the PCF8523 

Built on a Feather M0 BLE and the Adalogger Feather.
Interrupts drive communication (TTL out to a theoretical MAX232 ) and synchronization of the internal and external clocks

Currently the code only turns the device on and off and synchronizes, but the direction would be to allow status checks 
from BLE connections and overrides to be processed from BLE as well. This would enable drive-by lamp hour checks, and
power toggles to be scheduled or overrideden from outside the room. 
