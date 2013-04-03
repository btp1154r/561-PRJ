#include <ior5f100le.h>
#include <stdint.h>
#include "rtc_sched.h"
#include "lcd.h"
#include "profile.h"
#include "Geometry.h"

void Task1(void) {
	static char led_state=0;
	
	P5_bit.no5 = led_state;
	led_state ^= 1;
}

void Task2(void) {
	static char led_state=0;
	
	P6_bit.no2 = led_state;
	led_state ^= 1;
}

// Task3 runs at 1 Hz, updates elapsed time on LCD
void Task3(void) {
	static char led_state=0;
	static unsigned char m=0, s=0;
	
	P6_bit.no3 = led_state;
	led_state ^= 1;
	
	s++;
	if (s>59) {
		m++;
		s = 0;
	}
	LCDPrintf(0,0, "%02u:%02u", m, s);

}

void Task4(void) {
	float dist, bearing, cur_pos_lat, cur_pos_lon;
	char * name;
	cur_pos_lat = TEST1_LAT;
	cur_pos_lon = TEST1_LON;

	LCDClear();
	LCDInvert();
	// measure time to find closest waypoint 
	Enable_Profiling();
	Find_Nearest_Waypoint(cur_pos_lat, cur_pos_lon, &dist, &bearing, &name);
	Disable_Profiling();

	LCDPrintf(3, 0, "                ");
	LCDPrintf(3, 0, "%1.16s", name);
	LCDPrintf(4, 0, "D:%5f", dist);
	LCDPrintf(5, 0, "B:%5f", bearing);

	Print_Results();	
	
#if 0
	BNSPrintf(LCD, "\t%1.8s", name);
	DisplayDelay(2000);
	BNSPrintf(LCD, "\tD:%5f\nB: %5f", dist, bearing);
	DisplayDelay(2000);

	cur_pos_lat = TEST2_LAT;
	cur_pos_lon = TEST2_LON;

	// measure time to find waypoint closest to San Francisco Airport
	Enable_Profiling();
	Find_Nearest_Waypoint(cur_pos_lat, cur_pos_lon, &dist, &bearing, &name);
	Disable_Profiling();

	BNSPrintf(LCD, "\t%1.8s", name);
	DisplayDelay(2000);
	BNSPrintf(LCD, "\tD:%5f\nB: %5f", dist, bearing);
	DisplayDelay(2000);

	BNSPrintf(LCD, "\t  Done  \nt=%6d", profile_ticks);
#endif
	
}