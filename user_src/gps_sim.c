// GPS message generation 

#include "gps_sim.h"
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "lcd.h"
#include "stdio.h"
#include "string.h"
#include "r_cg_userdefine.h"
#include <math.h>

extern volatile uint8_t G_UART_SendingData, G_UART_ReceivingData;

char msg[128];

uint8_t checksum(char *s) {
    uint8_t c = 0;
 
    while(*s)
        c ^= *s++;
		
    return c;
}

void create_GLL_msg(char * buffer, int lat_deg, float lat_min, int lon_deg, float lon_min, unsigned
hr, unsigned min, unsigned sec) {
	uint8_t cs=0;
	char end[8];
	
	// form message
	sprintf(buffer, "$GPGLL,%02d%08.5f,N,%03d%08.5f,W,%02d%02d%02d,A,*", lat_deg, lat_min, lon_deg, lon_min,
	hr, min, sec);

	// form checksum
	cs = checksum(buffer);

	// undo effects of exoring in start $ and end *
	cs ^= '$' ^ '*';
	
	sprintf(end, "%02X\n\r", cs);
	strcat(buffer, end);
}

void create_RMC_msg(char * buffer, int lat_deg, float lat_min, int lon_deg, float lon_min, unsigned
hr, unsigned min, unsigned sec, float speed, float track, uint32_t date, float var) {
	uint8_t cs=0;
	char end[8];
	
	// form message
	sprintf(buffer, "$GPRMC,%02d%02d%02d,A,%02d%08.5f,N,%03d%08.5f,W,%05.1f,%04.1f,%06ld,%05.1f,W*", 
					hr, min, sec, lat_deg, lat_min, lon_deg, lon_min, speed, track, date, var);

	// form checksum
	cs = checksum(buffer);

	// undo effects of exoring in start $ and end *
	cs ^= '$' ^ '*';

	sprintf(end, "%02X\n\r", cs);
	strcat(buffer, end);
}


void inc_time(unsigned * hours, unsigned * minutes, unsigned * seconds) {
	(*seconds)++;
	if (*seconds > 59) {
		*seconds -= 60;
		(*minutes)++;
		if (*minutes > 59) {
			*minutes -= 60;
			(*hours)++;
		}
	}
}

void find_destination(float lat_start, float lon_start, float bearing, 
											float distance, float * lat_dest, float * lon_dest) {
	float lat_start_rad = lat_start * (M_PI/180.0);
	float lon_start_rad = lon_start * (M_PI/180.0);
	float lat_dest_rad = 0.0;
	float lon_dest_rad = 0.0;
	float bearing_rad = bearing*(M_PI/180.0);
												
	lat_dest_rad = asin( sin(lat_start_rad)*cos(distance/6371) + 
											cos(lat_start_rad)*sin(distance/6371)*cos(bearing_rad));
	lon_dest_rad = lon_start_rad + 
											atan2(sin(bearing_rad)*sin(distance/6371)*cos(lat_start_rad),
											cos(distance/6371)-sin(lat_start_rad)*sin(*lat_dest));							
	
	*lat_dest = lat_dest_rad*(180/M_PI);
	*lon_dest = lon_dest_rad*(180/M_PI);
	
}

void sim_motion(void) {
	uint32_t date=22213;
	float lat = INIT_LAT, lon = INIT_LON;
	int16_t i, lat_deg, lon_deg;
	float lat_min, lon_min;
	float lat_dest=0.0, lon_dest=0.0;
	float spd=INIT_SPEED, trk=INIT_TRACK, var=0.0;
	float distance=0.0;
	
	unsigned h=0, m=0, s=0;
	volatile uint32_t dly;

	lat_deg = (int) lat;
	lon_deg = (int) lon;
	lat_min = 60*fmod(lat, 1.0);
	lon_min = 60*fmod(lon, 1.0);

	LCDPrintf(1,0, "GPSSimulator");
	LCDPrintf(2,0, "deg minutes");

	
	for (i=0; i<NUM_STEPS; i++) {
			lat_deg = (int) lat;
			lon_deg = (int) lon;
			lat_min = 60*fmod(lat, 1.0);
			if (lat_min < 0.0)
				lat_min *= -1.0;

			lon_min = 60*fmod(lon, 1.0);
			if (lon_min < 0.0)
				lon_min *= -1.0;

			LCDPrintf(3,0, "%03d %08.5f", lat_deg, lat_min);
			LCDPrintf(4,0, "%03d %08.5f", lon_deg, lon_min);
			LCDPrintf(5,0, "%02d:%02d:%02d", h, m, s);
			LCDPrintf(6,0, "%06.2f deg", trk);
			LCDPrintf(7,0, "%06.2f kt", spd);


			
			create_RMC_msg(msg, lat_deg, lat_min, lon_deg, lon_min, h, m, s, spd, trk, date, var);

#if 1
			G_UART_SendingData = 1;		
			R_UART0_Send((uint8_t *) msg, strlen(msg));
			//	wait for tx to finish
			while (G_UART_SendingData)	
				;
#endif
			
			// compute distance, convert to km
			distance = (spd*KM_PER_NAUTMILE)/3600.0;
			find_destination(lat, lon, trk, distance, &lat_dest, &lon_dest);
			// update variables and LCD
			lat = lat_dest;
			lon = lon_dest;
			
			trk += 10;
			
			inc_time(&h, &m, &s);


			// delay until user presses switch 3 
			while (SW_3 == 1) {
			}

			// debounce			
			for (dly=0; dly<60000; dly++) {
			}
			
			// wait until released
			while (SW_3 == 0) {
			}
	}
}

