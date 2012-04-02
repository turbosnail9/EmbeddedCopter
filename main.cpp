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

	/** Extern **/
	sc_signal<bool> reset;

	//Battery
	sc_signal<sc_uint<8> > level_change;
	sc_signal<bool> batt_trig;

	//Prox
	sc_signal<sc_uint<3> > prox_input;

	//Pos
	sc_signal<sc_uint<8> > x_pos;
	sc_signal<sc_uint<8> > y_pos;
	sc_signal<sc_uint<8> > z_pos;

	//Motor
	sc_signal<bool> motor_clk;

	//Comm
	sc_signal<sc_uint<8> > signal_quality;
	sc_signal<sc_uint<8> > external_command;
	sc_signal<sc_uint<8> > external_command_arg1;
	sc_signal<sc_uint<8> > external_command_arg2;
	sc_signal<sc_uint<8> > external_command_arg3;

	/** Intern **/

	sc_signal<bool> connected;
	sc_signal<sc_uint<8> > internal_command;
	sc_signal<sc_uint<8> > internal_command_arg1;
	sc_signal<sc_uint<8> > internal_command_arg2;
	sc_signal<sc_uint<8> > internal_command_arg3;

	sc_signal<bool> batt_warning;

	sc_signal<sc_uint<8> > x_pos_internal;
	sc_signal<sc_uint<8> > y_pos_internal;
	sc_signal<sc_uint<8> > z_pos_internal;

	sc_signal<sc_uint<8> > x_dest;
	sc_signal<sc_uint<8> > y_dest;
	sc_signal<sc_uint<8> > z_dest;

	sc_signal<sc_uint<3> > prox_warning;

	sc_signal<sc_int<8> > x_move_vect;
	sc_signal<sc_int<8> > y_move_vect;
	sc_signal<sc_int<8> > z_move_vect;

	sc_signal<sc_int<8> > x_delta;
	sc_signal<sc_int<8> > y_delta;
	sc_signal<sc_int<8> > z_delta;

	/** Modules **/

	comm comm("comm");
	//In
	comm.reset(reset);
	comm.signal_quality(signal_quality);
	comm.external_command(external_command);
	comm.external_command_arg1(external_command_arg1);
	comm.external_command_arg2(external_command_arg2);
	comm.external_command_arg3(external_command_arg3);
	//Out
	comm.connected(connected);
	comm.internal_command(internal_command);
	comm.internal_command_arg1(internal_command_arg1);
	comm.internal_command_arg2(internal_command_arg2);
	comm.internal_command_arg3(internal_command_arg3);

	battery battery("battery");
	//In
	battery.reset(reset);
	battery.trig(batt_trig);
	battery.level_change(level_change);
	//Out
	battery.warning(batt_warning);

	mainlogic mainlogic("mainlogic");
	//In
	mainlogic.reset(reset);
	mainlogic.x_pos(x_pos_internal);
	mainlogic.y_pos(y_pos_internal);
	mainlogic.z_pos(z_pos_internal);
	mainlogic.prox_warning(prox_warning);
	mainlogic.comm_connected(connected);
	mainlogic.comm_instruction(internal_command);
	mainlogic.comm_instruction_arg1(internal_command_arg1);
	mainlogic.comm_instruction_arg2(internal_command_arg2);
	mainlogic.comm_instruction_arg3(internal_command_arg3);
	mainlogic.battery_warning(batt_warning);
	//Out
	mainlogic.x_dest(x_dest);
	mainlogic.y_dest(y_dest);
	mainlogic.z_dest(z_dest);

	motor motor("motor");
	//In
	motor.reset(reset);
	motor.clock(motor_clk);
	motor.x_move_vect(x_move_vect);
	motor.y_move_vect(y_move_vect);
	motor.z_move_vect(z_move_vect);
	//Out
	motor.x_delta(x_delta);
	motor.y_delta(y_delta);
	motor.z_delta(z_delta);

	motorcontrol motorcontrol("motorcontrol");
	//In
	motorcontrol.reset(reset);
	motorcontrol.x_pos(x_pos_internal);
	motorcontrol.y_pos(y_pos_internal);
	motorcontrol.z_pos(z_pos_internal);
	motorcontrol.x_dest(x_dest);
	motorcontrol.y_dest(y_dest);
	motorcontrol.z_dest(z_dest);
	//Out
	motorcontrol.x_move_vect(x_move_vect);
	motorcontrol.y_move_vect(y_move_vect);
	motorcontrol.z_move_vect(z_move_vect);

	proximitysensor proximitysensor("proximitysensor");
	//In
	proximitysensor.reset(reset);
	proximitysensor.nearby_object(prox_input);
	//Out
	proximitysensor.warning(prox_warning);

	positionsensor positionsensor("positionsensor");
	//In
	positionsensor.reset(reset);
	positionsensor.x_reading(x_pos);
	positionsensor.y_reading(y_pos);
	positionsensor.z_reading(z_pos);
	//Out
	positionsensor.x_value(x_pos_internal);
	positionsensor.y_value(y_pos_internal);
	positionsensor.z_value(z_pos_internal);

	/** VCD File **/

	sc_trace_file *wf = sc_create_vcd_trace_file("trace");
	sc_trace(wf, batt_trig, "clock");
	sc_trace(wf, reset, "reset");
	sc_trace(wf, batt_warning, "warning");
	sc_trace(wf, level_change, "level_change");

	/** Simulation **/

	sc_start(1, SC_NS, SC_RUN_TO_TIME);

	reset = 1;
	sc_start(1, SC_NS, SC_RUN_TO_TIME);
	reset = 0;
	sc_start(1, SC_NS, SC_RUN_TO_TIME);

	level_change = 1;
	for (int i = 0 ;i < 96;i++) {
		batt_trig = 1;
		sc_start(1, SC_NS, SC_RUN_TO_TIME);
		batt_trig = 0;
		sc_start(1, SC_NS, SC_RUN_TO_TIME);
	}

	sc_close_vcd_trace_file(wf);
	return 0;
 }
