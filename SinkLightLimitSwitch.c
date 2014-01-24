#include "hardware.h"

#include "light_ws2812.h"
#include "Sink_Lighting_Limit_Switch.h"

struct CRGB { uint8_t g; uint8_t r; uint8_t b; };
struct CRGB onled[1];
struct CRGB offled[1];
struct CRGB led[11];

// IOPin limitSwitchTriggerPin;

int8_t i;
int8_t j;

uint16_t inactive;
static uint16_t offtime = 30000;

boolean status_on = FALSE;


// Initialise the hardware
void appInitHardware(void) {
	initHardware();
}
// Initialise the software
TICK_COUNT appInitSoftware(TICK_COUNT loopStart){
	
	onled[0].r = 60;
	onled[0].g = 60;
	onled[0].b = 60;
	
	offled[0].r = 0;
	offled[0].g = 0;
	offled[0].b = 0;
	
	inactive = 0;
	
	cli();				// Disable interrupts. Can be removed if no interrupts are used.
	for (i=0; i<11; i++) {
		led[i] = onled[0];
	}
	ws2812_sendarray((uint8_t *)&led[0], 33);
	sei();				// Enable interrupts.
	
	delay_ms(1000);
	
	cli();				// Disable interrupts. Can be removed if no interrupts are used.
	for (i=0; i<11; i++) {
		led[i] = offled[0];
	}
	ws2812_sendarray((uint8_t *)&led[0], 33);
	sei();				// Enable interrupts.
		
	return 0;
}
// This is the main loop
TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart) {

	if (SWITCH_pressed(&LimitSwitch)) {
		
		cli();				// Disable interrupts. Can be removed if no interrupts are used.
		for (i=0; i<11; i++) {
			led[i] = onled[0];
		}
		ws2812_sendarray((uint8_t *)&led[0], 33);
		sei();				// Enable interrupts.
		status_on = TRUE;
		inactive = 4000;
		return 5000000; // wait 5 s
	}
	
	if (status_on) {
		if (pin_is_high(PID_sensor)) {
			inactive = 0;
		}
		else {
			inactive += 1000;
		}
		
		if (inactive > offtime){
			
			cli();				// Disable interrupts. Can be removed if no interrupts are used.
			for (i=0; i<11; i++){
				led[i] = offled[0];
			}
			ws2812_sendarray((uint8_t *)&led[0],33);
			sei();				// Enable interrupts.
			
			//pin_change_attach(B2, &limitSwitchTriggered, null);
			status_on = FALSE;
		}
	}
	
	/* ...add your code here... */
	return 1000000; // microseconds
}


// void limitSwitchTriggered(const IOPin* io, boolean val, volatile void* data){
	
	// int8_t i;
	// for (i=0; i<11; i++)
			// led[i] = onled;
		
	// status_on = TRUE;
// }
