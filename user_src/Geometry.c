#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "Geometry.h"

#include "lcd.h"
#define PI 3.14159265


#define SEARCH_WINDOW 15
/****FS vars***/

#include<integer.h>
#include "ff.h"

#define READ_SIZE (49)
#define NAME_SIZE (22)
#define HEADER_SIZE (47)

#define MIN_LINE 0
#define MAX_LINE 288
PT_T wp_cache[2*SEARCH_WINDOW];
int wp_head_index=0;
int wp_tail_index=0;
int gf_start=0;
int gf_end=0;
char first_sample=1;


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

void Find_Waypoint(float cur_pos_lat, float cur_pos_lon, float true_course, float speed) {
		
	int i=0, closest_i;
	PT_T ref;
	float d, b; //closest_d=1E10;
        float least_dist = 1E10;
	char buf[10];
        float cross_track_dist, along_track_dist, time; 
	ref.Lat = cur_pos_lat;
	ref.Lon = cur_pos_lon;
	strcpy(ref.Name, "Reference");

	for(i=wp_head_index;i!=wp_tail_index;i=(i+1)%(2*SEARCH_WINDOW)) { 
                
	        d = Calc_Distance2(&ref, &(wp_cache[i]) );
		b = Calc_Bearing(&ref, &(wp_cache[i]) );
                //cross_track_dist = fabs(asin(sin(d/6371)*sin((b*PI/180)-(true_course*PI/180))));
                //cross_track_dist = fabs(asin(sin(d/6371)*sin((b*PI/180)-(true_course*PI/180))));
                Enable_Profiling();
                cross_track_dist = fabs(asin(sin(d)*sin((b*PI/180)-(true_course*PI/180))));
                Disable_Profiling();
                
#if 0	
		LCDPrintf(1, 0, "                ");
		LCDPrintf(1, 0, "%1.16s", wp_cache[i].Name);
		LCDPrintf(2, 0, "D:%5f", d);
                LCDPrintf(3, 0, "C:%5f", cross_track_dist);
#endif	
		// if we found a closer waypoint, remember it and display it
		if (cross_track_dist<.00007849) {
			least_dist = cross_track_dist;
                        along_track_dist = acos(cos(d/6371)/cos(cross_track_dist/6371))*6371;
                        time = (along_track_dist/(speed*1.85));
			closest_i = i;
#if 1
			LCDPrintf(5, 0, "                ");
			LCDPrintf(5, 0, "%1.16s", wp_cache[i].Name);
			LCDPrintf(6, 0, "D:%5f", d);
                        LCDPrintf(7, 0, "C:%5f", cross_track_dist);
                        LCDPrintf(8, 0, "T:%5f",time);
#endif		
		}
		i++;
	}	
}

void openFeatFile()
{
  /* Mount drive 0 */
  FRESULT fresult;
        fresult = f_mount(0, &fs[0]);
  
        /* Open source file on the drive 0 */
        fresult = f_open(&fsrc, "0:NC_Feat.csv", FA_READ); 
        if (fresult != FR_OK) 
        {
  	  while (1) ;
        }
        return;
}

void closeFeatFile()
{
  
        /* Close open file */
  f_close(&fsrc);

    
  /* Unmount drive 0 */
  f_mount(0, 0);
  return;
       
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
        
        int rl_count=0;   //number of lines read from file
        unsigned long file_offset=0 ;
        int f_start=0;
        int f_end=0;
        int write_index;
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
         
	fseek_float = 0.2759*x*x*x*x*x*x - 5.0821*x*x*x*x*x + 34.728*x*x*x*x - 108.5*x*x*x + 157.58*x*x - 54.246*x + 8.8365;        
        
        
        if(fseek_float<0)
          fseek_float=MIN_LINE;
        if(fseek_float>MAX_LINE)
          fseek_float = MAX_LINE;
        
        fseek_line = (unsigned int)(fseek_float);
        
        
        
        //reading from file must be cyclic
       
          f_start = fseek_line-SEARCH_WINDOW;
       if(f_start<MIN_LINE)
          f_start =MIN_LINE;
        

          f_end = fseek_line+SEARCH_WINDOW;
        if(f_end>MAX_LINE)
          f_end = MAX_LINE;

        
        //update cache
        if(abs(f_start-gf_start)>2*SEARCH_WINDOW)
        {
           gf_start=f_end;
           gf_end = f_start+1;
           wp_head_index=0;
           wp_tail_index=0;
        }
        
       
        wp_head_index = (wp_head_index+f_start-gf_start)%(2*SEARCH_WINDOW);
        wp_tail_index = (wp_tail_index+f_end-gf_end)%(2*SEARCH_WINDOW);
         
         if(f_start>gf_start)
         {
          file_offset = (unsigned long)(HEADER_SIZE+(READ_SIZE+2)*(unsigned long)gf_end);
          rl_count = f_end-gf_end;
          write_index = wp_tail_index;
         }
         else
         {
           file_offset = (unsigned long)(HEADER_SIZE+(READ_SIZE+2)*(unsigned long)f_start);
           rl_count = gf_start-f_start;
           write_index = wp_head_index;
         }
          fresult = f_lseek(&fsrc,file_offset);
          
          for(i=rl_count;i>0;i--)
          {
              fresult = f_read(&fsrc, buffer, READ_SIZE+2, &br);
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
        
                      wp_cache[write_index].Lat = lat;
                      wp_cache[write_index].Lon = lon;
                      strncpy(wp_cache[write_index].Name,name,NAME_SIZE+1);
                      write_index = (write_index+1)%(2*SEARCH_WINDOW);
                      
                      
                      
                    
      
              }	
          }
          
          gf_start=f_start;
           gf_end = f_end;
	
       

       
	//search cache for closest point
	for(i=wp_head_index;i!=wp_tail_index;i=(i+1)%(2*SEARCH_WINDOW))
        {
          
              
              
              d = Calc_Distance(&ref, &wp_cache[i] );
              b = Calc_Bearing(&ref, &wp_cache[i] );
              
              if (d<closest_d) 
              {
                closest_d = d;
                *distance = d;
                *bearing = b;
               strncpy(*wp_name,wp_cache[i].Name,NAME_SIZE+1);
              }
	
				
	}
	
	
        
        return ;
}