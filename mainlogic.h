#ifndef EMBEDDED_MAINLOGIC
#define EMBEDDED_MAINLOGIC

typedef enum {
	STOPPED = 0,
	TAKEOFF = 1,
	LAND = 2,
	MOVE_TO = 3,
	HOLD_POS = 4,
	LOSS_COMM = 5,
	DANGER = 6
} main_state_t;


#endif