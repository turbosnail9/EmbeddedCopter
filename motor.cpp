#include "systemc.h"

SC_MODULE(motor) {
	sc_in<bool> reset;
	sc_in<bool> clock;

	sc_in<sc_int<8> > x_move_vect;
	sc_in<sc_int<8> > y_move_vect;
	sc_in<sc_int<8> > z_move_vect;

	sc_out<sc_int<8> > x_delta;
	sc_out<sc_int<8> > y_delta;
	sc_out<sc_int<8> > z_delta;

	void flush() {
		x_delta.write(0);
		y_delta.write(0);
		z_delta.write(0);
	}

	void update() {
		if(x_move_vect.read().to_int() > 0) x_delta.write(1);
		else if(x_move_vect.read().to_int() < 0) x_delta.write(-1);
		else x_delta.write(0);
	
		if(y_move_vect.read().to_int() > 0) y_delta.write(1);
		else if(y_move_vect.read().to_int() < 0) y_delta.write(-1);
		else y_delta.write(0);
	
		if(z_move_vect.read().to_int() > 0) z_delta.write(1);
		else if(z_move_vect.read().to_int() < 0) z_delta.write(-1);
		else z_delta.write(0);
	}

	SC_CTOR(motor) {
		cout << "New motorcontrol module" << endl;
		SC_METHOD(flush);
		sensitive << reset.pos();
		SC_METHOD(update);
		sensitive << clock.pos();
	}

};