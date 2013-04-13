#ifndef GEOMETRY_H
#define GEOMETRY_H
// geometry 

#include "config.h"

#define RALEIGH_LAT 35.81888889
#define RALEIGH_LON	78.64472222

#define NCSU_LAT  35.78709590
#define NCSU_LON -78.67250400

#define SEARCH_WINDOW 20

#define SFO_LAT 37.618889
#define SFO_LON 122.375

#define TEST1_LAT 35.73515280

#define TEST1_LON -78.70639400


#define TEST2_LAT 35.74737470

#define TEST2_LON -78.72611610


typedef struct {
	float Lat;
	float Lon;
	char Name[24];
} PT_T;

typedef struct {
	float Lat;
	float Lon;
}waypoint;



extern void Find_Nearest_Waypoint(float cur_pos_lat, float cur_pos_lon, float * distance, float * bearing, 
	char * * name);
void openFeatFile(void);
void closeFeatFile(void);

#endif