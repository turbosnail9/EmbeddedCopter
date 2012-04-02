#include "systemc.h"

#include "battery.cpp"
#include "comm.cpp"
#include "mainlogic.cpp"
#include "motor.cpp"
#include "motorcontrol.cpp"
#include "positionsensor.cpp"
#include "proximitysensor.cpp"

int sc_main(int argc, char* argv[]) {

	/** Signals **/

	sc_signal<bool> clock;
	sc_signal<bool> reset;
	sc_signal<bool> warning;
	sc_signal<sc_uint<8> > level_change;

	/** Modules **/

	comm comm("comm");
	//In
	comm.reset(reset);
	// comm.signal_quality();
	// comm.external_command();
	// comm.external_command_arg1();
	// comm.external_command_arg1();
	// comm.external_command_arg1();
	//Out
	// comm.connected();
	// comm.internal_command();
	// comm.internal_command_arg1();
	// comm.internal_command_arg2();
	// comm.internal_command_arg3();

	battery battery("battery");
	//In
	battery.reset(reset);
	battery.clock(clock);
	battery.level_change(level_change);
	//Out
	battery.warning(warning);

	mainlogic mainlogic("mainlogic");
	//In
	mainlogic.reset(reset);
	// mainlogic.x_pos();
	// mainlogic.y_pos();
	// mainlogic.z_pos();
	// mainlogic.prox_warning();
	// mainlogic.comm_connected();
	// mainlogic.comm_instruction();
	// mainlogic.comm_instruction_arg1();
	// mainlogic.comm_instruction_arg2();
	// mainlogic.comm_instruction_arg3();
	// mainlogic.battery_warning();
	//Out
	// mainlogic.x_dest();
	// mainlogic.y_dest();
	// mainlogic.z_dest();

	motor motor("motor");
	//In
	motor.reset(reset);
	// motor.clock();
	// motor.x_move_vect();
	// motor.y_move_vect();
	// motor.z_move_vect();
	//Out
	// motor.x_delta();
	// motor.y_delta();
	// motor.z_delta();

	motorcontrol motorcontrol("motorcontrol");
	//In
	motorcontrol.reset(reset);
	// motorcontrol.x_pos();
	// motorcontrol.y_pos();
	// motorcontrol.z_pos();
	// motorcontrol.x_des();
	// motorcontrol.y_des();
	// motorcontrol.z_des();
	//Out
	// motorcontrol.x_move_vect();
	// motorcontrol.y_move_vect();
	// motorcontrol.z_move_vect();

	proximitysensor proximitysensor("proximitysensor");
	//In
	proximitysensor.reset(reset);
	// proximitysensor.nearby_object();
	//Out
	// proximitysensor.warning();

	positionsensor positionsensor("positionsensor");
	//In
	positionsensor.reset(reset);
	// positionsensor.x_reading();
	// positionsensor.y_reading();
	// positionsensor.z_reading();
	//Out
	// positionsensor.x_value();
	// positionsensor.y_value();
	// positionsensor.z_value();

	/** VCD File **/

	sc_trace_file *wf = sc_create_vcd_trace_file("trace");
	sc_trace(wf, clock, "clock");
	sc_trace(wf, reset, "reset");
	sc_trace(wf, warning, "warning");
	sc_trace(wf, level_change, "level_change");

	/** Simulation **/

	sc_start(1, SC_NS, SC_RUN_TO_TIME);

	reset = 1;
	sc_start(1, SC_NS, SC_RUN_TO_TIME);
	reset = 0;
	sc_start(1, SC_NS, SC_RUN_TO_TIME);

	level_change = 1;
	for (int i = 0 ;i < 96;i++) {
		clock = 1;
		sc_start(1, SC_NS, SC_RUN_TO_TIME);
		clock = 0;
		sc_start(1, SC_NS, SC_RUN_TO_TIME);
	}

	sc_close_vcd_trace_file(wf);
	return 0;
 }
