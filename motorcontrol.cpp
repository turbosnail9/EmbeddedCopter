#include "systemc.h"

SC_MODULE(motorcontrol) {
	sc_in<bool> reset;

	sc_in<sc_uint<8> > x_pos;
	sc_in<sc_uint<8> > y_pos;
	sc_in<sc_uint<8> > z_pos;

	sc_in<sc_uint<8> > x_des;
	sc_in<sc_uint<8> > y_des;
	sc_in<sc_uint<8> > z_des;

	sc_out<sc_int<8> > x_move_vect;
	sc_out<sc_int<8> > y_move_vect;
	sc_out<sc_int<8> > z_move_vect;

	void flush() {
		x_move_vect.write(0);
		y_move_vect.write(0);
		z_move_vect.write(0);
	}

	void update() {
		x_move_vect.write(x_des.read()-x_pos.read());
		y_move_vect.write(y_des.read()-y_pos.read());
		z_move_vect.write(z_des.read()-z_pos.read());
	}

	SC_CTOR(motorcontrol) {
		cout << "New motorcontrol module" << endl;
		SC_METHOD(flush);
		sensitive << reset.pos();
		SC_METHOD(update);
		sensitive << x_pos;
		sensitive << y_pos;
		sensitive << z_pos;
		sensitive << x_des;
		sensitive << y_des;
		sensitive << z_des;
	}

};