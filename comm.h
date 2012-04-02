#ifndef EMBEDDED_COMM
#define EMBEDDED_COMM

typedef enum {
	NO_COMM = 0,
	TAKEOFF_COMM = 1,
	LAND_COMM = 2,
	MOVE_TO_COMM = 3,
	HOLD_POS_COMM = 4
} comm_instruction_t;

#endif