#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

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

waypoint *wp_cache;
waypoint *wp_head;
waypoint *wp_tail;


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
		
	int i=0;
	PT_T ref;
	float d, b, closest_d=1E10;
	
        PT_T twaypoint;
        PT_T NCSU;
        unsigned int fseek_line;
        float x;
        float fseek_float;
        unsigned int search_win=SEARCH_WINDOW;
        int rl_count=0;   //number of lines read from file
        unsigned long file_offset=0 ;
        
        FRESULT fresult;
        
        
        UINT br, tbr = 0;
        char * lat_ptr, * lon_ptr,*end_ptr;
        char temp_str[20];
       
        
        //set ref
        *distance = *bearing = NULL;
	

	ref.Lat = cur_pos_lat;
	ref.Lon = cur_pos_lon;
        strcpy(ref.Name, "Reference");
        
        NCSU.Lat = NCSU_LAT;
        NCSU.Lon = NCSU_LON;
        strcpy(NCSU.Name, "NCSU");
        
        //bearing_at_ncsu
         x = atan2(
		sin(NCSU_LON*PI/180 - cur_pos_lon*PI/180)*cos(cur_pos_lat*PI/180),
		cos(NCSU_LAT*PI/180)*sin(cur_pos_lat*PI/180) - 
		sin(NCSU_LAT*PI/180)*cos(cur_pos_lat*PI/180)*cos(NCSU_LON*PI/180 - cur_pos_lon*PI/180))+PI;
	if(x<1.394764025)
          fseek_float = (6175.6*x*x*x*x - 11559*x*x*x + 4867.4*x*x + 3809.9*x + 2.6179);
        else if(x<2.030296278)
          fseek_float = (-22093*x*x*x*x + 248010*x*x*x - 937985*x*x + (10^6)*x - 846782);
        else if(x<4.157029752)
          fseek_float = (-442.41*x*x*x*x + 6079.6*x*x*x - 30816*x*x + 69186*x - 29742);
        else 
          fseek_float = 1836.4*x*x*x*x - 38821*x*x*x + 305136*x*x - (10^6)*x + 1000000;
        
        fseek_line = (unsigned int)(fseek_float);

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
	//fresult = f_read(&fsrc, buffer, HEADER_SIZE, &br);
        
        file_offset = (unsigned long)(HEADER_SIZE+(READ_SIZE+2)*(unsigned long)(fseek_line-search_win));
//          file_offset = (unsigned long)(HEADER_SIZE+(READ_SIZE+2)*(unsigned long)28145);
       fresult = f_lseek(&fsrc,file_offset);
       

       
	
	do {
                
		fresult = f_read(&fsrc, buffer, READ_SIZE+2, &br);
                rl_count++;
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
	} while (rl_count!=SEARCH_WINDOW*2);
	
	
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