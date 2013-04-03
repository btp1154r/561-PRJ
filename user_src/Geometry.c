#include <stdint.h>
#include <math.h>
#include <string.h>

#include "config.h"
#include "Geometry.h"
#include "lcd.h"
#define PI 3.14159265

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
	char * * name) {
	// cur_pos_lat and cur_pos_lon are in degrees
	// distance is in kilometers
	// bearing is in degrees
		
	int i=0, closest_i;
	PT_T ref;
	float d, b, closest_d=1E10;
	char buf[10];

	*distance = *bearing = NULL;
	*name = NULL;

	ref.Lat = cur_pos_lat;
	ref.Lon = cur_pos_lon;
	strcpy(ref.Name, "Reference");

	while (strcmp(waypoints[i].Name, "END")) {
		d = Calc_Distance(&ref, &(waypoints[i]) );
		b = Calc_Bearing(&ref, &(waypoints[i]) );

#if 1		
		LCDPrintf(1, 0, "                ");
		LCDPrintf(1, 0, "%1.16s", waypoints[i].Name);
		LCDPrintf(2, 0, "D:%5f", d);
#endif
		
		// if we found a closer waypoint, remember it and display it
		if (d<closest_d) {
			closest_d = d;
			closest_i = i;
#if 1
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
}