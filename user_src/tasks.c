#include <ior5f100le.h>
#include <stdint.h>
#include "rtc_sched.h"
#include "lcd.h"
#include "profile.h"
#include "Geometry.h"
#include <stdlib.h>

//#include "r_cg_serial.h"
typedef struct 
{
 
 unsigned char hour;
 unsigned char min;
 unsigned char  sec;
 unsigned char day;
 unsigned char month;
 unsigned char year;
  int lat_deg;
  int long_deg;
  float lat_min;
  float long_min;
  float speed;
  float angle;
  float var;
  char valid;
}CurGPSInfo;
extern CurGPSInfo info;


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
	//LCDPrintf(0,0, "%02u:%02u", m, s);

}

void Task4(void) {
	float dist, bearing, cur_pos_lat, cur_pos_lon;
	char *name;
        while(1)
        {
        while(!info.valid);
	cur_pos_lat = info.lat_deg + info.lat_min/60*100;
	cur_pos_lon = info.long_deg + info.long_min/60*100;
        info.valid =0;
        name = (char *)malloc(24*sizeof(char));

	//LCDClear();
	//LCDInvert();
	// measure time to find closest waypoint 
        
	Enable_Profiling();
	Find_Nearest_Waypoint(cur_pos_lat, cur_pos_lon, &dist, &bearing, &name);
	Disable_Profiling();
#if 1
	LCDPrintf(3, 0, "                ");
	LCDPrintf(3, 0, "%1.16s", name);
	LCDPrintf(4, 0, "D:%5f", dist);
	LCDPrintf(5, 0, "B:%5f", bearing);

        Print_Results();	
     #endif   
        free(name);
	}

        
	
}