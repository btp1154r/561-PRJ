#include <ior5f100le.h>
#include <stdint.h>
#include "rtc_sched.h"
#include "lcd.h"
#include "profile.h"
#include "Geometry.h"

#include<integer.h>
#include "ff.h"
#include<string.h>
#include<math.h>

#define READ_SIZE (49)
#define NAME_SIZE (22)
#define HEADER_SIZE (47)



 FATFS fs[1];         /* Work area (file system object) for logical drive */
 FIL fsrc;               /* file objects */
BYTE buffer[READ_SIZE+16];   /* buffer of information to write to file */
 FRESULT res;         /* FatFs function common result code */
UINT br;          /* write 9 bytes to file */
UINT bw;         /* actual file read/write count */

volatile float lat, lon;
char name[NAME_SIZE+1];

void FileReadTask(void) {
  FRESULT fresult;
  DIR dir;				/* Directory object */
  FILINFO fno;			/* File information object */
  UINT br, tbr = 0;
  char * lat_ptr, * lon_ptr;
  /* Mount drive 0 */
  fresult = f_mount(0, &fs[0]);
  
#if 0  
  
  fresult = f_opendir(&dir, "");

	
  for (;;){
		fresult = f_readdir(&dir, &fno);		/* Read a directory item */
		if (fresult || !fno.fname[0]) 
			break;	/* Error or end of dir */
		if (fno.fattrib & AM_DIR)    {                  
		  //LCDPrintf(line,0,"<d>");
		  //LCDPrintf(line,4,fno.fname);
		  LED01 = 1;
		}	else    {
		  LED02 = 1;
		  //LCDPrintf(line,0,"<f>");
		  //LCDPrintf(line,4,fno.fname);
		}                
  }
	
#endif
  /* Open source file on the drive 0 */
  fresult = f_open(&fsrc, "0:NC_Feat.csv", FA_READ); 
	if (fresult != FR_OK) {
		while (1) ;
	}
                           

	
  /* Read from file */

	//read header line and discard
	fresult = f_read(&fsrc, buffer, HEADER_SIZE, &br);


	
	do {

		fresult = f_read(&fsrc, buffer, READ_SIZE, &br);
		if (br > 0) {
			tbr += br;

			strncpy(name, (char const *) buffer, NAME_SIZE);
			name[NAME_SIZE] = (char) 0;
			
			lat_ptr = strchr((char const *) buffer, ',');
			lat_ptr++;
			lat = atof(lat_ptr);						
			
			lon_ptr = strchr(lat_ptr, ',');
			lon_ptr++;
			lon = atof(lon_ptr);
	
		}			
	} while (br > 0);
	
	
  /* Close open file */
  f_close(&fsrc);

    
  /* Unmount drive 0 */
  f_mount(0, 0);
		
  
}

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
	char * name;
	cur_pos_lat = TEST1_LAT;
	cur_pos_lon = TEST1_LON;

	//LCDClear();
	//LCDInvert();
	// measure time to find closest waypoint 
	Enable_Profiling();
	Find_Nearest_Waypoint(cur_pos_lat, cur_pos_lon, &dist, &bearing, &name);
	Disable_Profiling();

	//LCDPrintf(3, 0, "                ");
	//LCDPrintf(3, 0, "%1.16s", name);
	//LCDPrintf(4, 0, "D:%5f", dist);
	//LCDPrintf(5, 0, "B:%5f", bearing);

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