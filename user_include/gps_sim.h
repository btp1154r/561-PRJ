#ifndef GPS_SIM_H
#define GPS_SIM_H

#define M_PI (3.14159265358979323846)

#define NUM_STEPS 100
#define MSG_PERIOD 2000


// NCSU coordinates
#define INIT_LAT 		(35.78709590)
#define INIT_LON		(-78.67250400)


#define INIT_TRACK 	(0.0)
#define INIT_SPEED 	(200.0)  // kt

#define KM_PER_NAUTMILE (1.852)

extern void sim_motion(void);


#endif // GPS_SIM_H

