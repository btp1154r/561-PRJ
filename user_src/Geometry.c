#include <stdint.h>
#include <math.h>
#include <string.h>

#include "config.h"
#include "Geometry.h"
#include "lcd.h"
#define PI 3.14159265


/****FS vars***/

#include<integer.h>
#include "ff.h"

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

/***************/

extern const PT_T waypoints[];

float Calc_Distance(PT_T * p1, const PT_T * p2) { 
// calculates distance in kilometers between locations (represented in degrees)
  return acos(sin(p1->Lat*PI/180)*sin(p2->Lat*PI/180) + 
		cos(p1->Lat*PI/180)*cos(p2->Lat*PI/180)*cos(p2->Lon*PI/180 - p1->Lon*PI/180)) * 6371;
}

float Calc_Bearing(PT_T * p1, const PT_T *  p2){
// calculates bearing in degrees between locations (represented in degrees)	
	float angle = atan2(
		sin(p1->Lon*PI/180 - p2->Lon*PI/180)*cos(p2->Lat*PI/180),
		cos(p1->Lat*PI/180)*sin(p2->Lat*PI/180) - 
		sin(p1->Lat*PI/180)*cos(p2->Lat*PI/180)*cos(p1->Lon*PI/180 - p2->Lon*PI/180)
		) * 180/PI;
	if (angle < 0.0)
		angle += 360;
	return angle;
}

void Find_Nearest_Waypoint(float cur_pos_lat, float cur_pos_lon, float * distance, float * bearing, 
	char * * wp_name) {
	// cur_pos_lat and cur_pos_lon are in degrees
	// distance is in kilometers
	// bearing is in degrees
		
	int i=0, closest_i;
	PT_T ref;
	float d, b, closest_d=1E10;
	char buf[10];
        PT_T twaypoint;
        
        FRESULT fresult;
        DIR dir;				/* Directory object */
        FILINFO fno;			/* File information object */
        UINT br, tbr = 0;
        char * lat_ptr, * lon_ptr;
        
        
        
        //set ref
        *distance = *bearing = NULL;
	

	ref.Lat = cur_pos_lat;
	ref.Lon = cur_pos_lon;
	strcpy(ref.Name, "Reference");

        /* Mount drive 0 */
        fresult = f_mount(0, &fs[0]);
  
        /* Open source file on the drive 0 */
        fresult = f_open(&fsrc, "0:NC_Feat.csv", FA_READ); 
        if (fresult != FR_OK) 
        {
  	  while (1) ;
        }
                           

	
        /* Read from file */

	//read header line and discard
	fresult = f_read(&fsrc, buffer, HEADER_SIZE, &br);
        
        
        

	
	do {

		fresult = f_read(&fsrc, buffer, READ_SIZE, &br);
		if (br > 0) 
                {
			tbr += br;

			strncpy(name, (char const *) buffer, NAME_SIZE);
			name[NAME_SIZE] = (char) 0;
			
			lat_ptr = strchr((char const *) buffer, ',');
			lat_ptr++;
			lat = atof(lat_ptr);						
			
			lon_ptr = strchr(lat_ptr, ',');
			lon_ptr++;
			lon = atof(lon_ptr);
                        
                        twaypoint.Lat = lat;
                        twaypoint.Lon = lon;
                        //twaypoint.Name = name;
                        
                        d = Calc_Distance(&ref, &twaypoint );
          		b = Calc_Bearing(&ref, &twaypoint );
                        
                        if (d<closest_d) 
                        {
			  closest_d = d;
			  *distance = d;
                          *bearing = b;
                          strncpy(*wp_name,name,NAME_SIZE+1);
                        }
	
		}			
	} while (br > 0);
	
	
  /* Close open file */
  f_close(&fsrc);

    
  /* Unmount drive 0 */
  f_mount(0, 0);
  
 
		

#if 0

	while (strcmp(waypoints[i].Name, "END")) 
        {
		d = Calc_Distance(&ref, &(waypoints[i]) );
		b = Calc_Bearing(&ref, &(waypoints[i]) );

#if 0
                LCDPrintf(1, 0, "                ");
		LCDPrintf(1, 0, "%1.16s", waypoints[i].Name);
		LCDPrintf(2, 0, "D:%5f", d);
#endif
		
		// if we found a closer waypoint, remember it and display it
		if (d<closest_d) {
			closest_d = d;
			closest_i = i;
#if 0
			LCDPrintf(3, 0, "                ");
			LCDPrintf(3, 0, "%1.16s", waypoints[i].Name);
			LCDPrintf(4, 0, "D:%5f", d);
#endif		
		}
		
		i++;
	}

	d = Calc_Distance(&ref, &(waypoints[closest_i]) );
	b = Calc_Bearing(&ref, &(waypoints[closest_i]) );

	// return information to calling function about closest waypoint 
	*distance = d;
	*bearing = b;
	*name = (char *)(waypoints[closest_i].Name);
        
#endif
        
        return ;
}