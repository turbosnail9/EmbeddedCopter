#include "systemc.h"

#include "mainlogic.h"
#include "comm.h"
#include "proximitysensor.h"

SC_MODULE(mainlogic) {
	sc_in<bool> reset;

	sc_in<sc_uint<8> > x_pos;
	sc_in<sc_uint<8> > y_pos;
	sc_in<sc_uint<8> > z_pos;

	sc_in<sc_uint<3> > prox_warning;
	
	sc_in<bool> comm_connected;
	sc_in<sc_uint<8> > comm_instruction;
	sc_in<sc_uint<8> > comm_instruction_arg1;
	sc_in<sc_uint<8> > comm_instruction_arg2;
	sc_in<sc_uint<8> > comm_instruction_arg3;

	sc_in<bool> battery_warning;

	sc_out<sc_uint<8> > x_dest;
	sc_out<sc_uint<8> > y_dest;
	sc_out<sc_uint<8> > z_dest;

	void flush() {
		x_dest.write(0);
		y_dest.write(0);
		z_dest.write(0);
	}

	void state_change() {
		if(battery_warning.read()) {
			x_dest.write(0);
			y_dest.write(0);
			z_dest.write(0);
		} else if(prox_warning.read()) {
			switch(prox_warning.read()) { //Move away from warning
				case UP_WARNING:
					z_dest.write(z_pos.read()-1);
					break;
				case DOWN_WARNING:
					z_dest.write(z_pos.read()+1);
					break;
				case LEFT_WARNING:
					x_dest.write(x_pos.read()-1);
					break;
				case RIGHT_WARNING:
					x_dest.write(x_pos.read()+1);
					break;
				case FRONT_WARNING:
					y_dest.write(y_pos.read()-1);
					break;
				case BACK_WARNING:
					y_dest.write(y_pos.read()+1);
					break;
			}
		} else if(!comm_connected.read()) {
			//Stay put
			x_dest.write(x_pos.read());
			y_dest.write(y_pos.read());
			z_dest.write(z_pos.read());
			//TODO add timeout for landing
		} else {
			switch(comm_instruction.read().to_uint()) {
				case NO_COMM: //No comm but connected = continue prev action
					//Do nothing
					break;
				case TAKEOFF_COMM:
					x_dest.write(10);
					y_dest.write(10);
					z_dest.write(10);
					break;
				case LAND_COMM:
					x_dest.write(0);
					y_dest.write(0);
					z_dest.write(0);
					break;
				case MOVE_TO_COMM:
					x_dest.write(comm_instruction_arg1.read());
					y_dest.write(comm_instruction_arg2.read());
					z_dest.write(comm_instruction_arg3.read());
					break;
				case HOLD_POS_COMM:
					x_dest.write(x_pos.read());
					y_dest.write(y_pos.read());
					z_dest.write(z_pos.read());
					break;
				default: //Invalid comm
					//Do nothing
					break;
			}
		}
	}

	SC_CTOR(mainlogic) {
		cout << "New motorcontrol module" << endl;
		SC_METHOD(flush);
		sensitive << reset.pos();
		SC_METHOD(state_change);
		sensitive << x_pos;
		sensitive << y_pos;
		sensitive << z_pos;
		sensitive << prox_warning;
		sensitive << comm_connected;
		sensitive << comm_instruction;
		sensitive << battery_warning;
	}

};