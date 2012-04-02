#include "systemc.h"

SC_MODULE(positionsensor) {
	sc_in<bool> reset;

	sc_in<sc_uint<8> > x_reading;
	sc_in<sc_uint<8> > y_reading;
	sc_in<sc_uint<8> > z_reading;

	sc_out<sc_uint<8> > x_value;
	sc_out<sc_uint<8> > y_value;
	sc_out<sc_uint<8> > z_value;

	void flush() {
		x_value.write(0);
		y_value.write(0);
		z_value.write(0);
	}

	void new_x_data() {
		x_value.write(x_reading.read());
	}

	void new_y_data() {
		y_value.write(y_reading.read());
	}

	void new_z_data() {
		z_value.write(z_reading.read());
	}

	SC_CTOR(positionsensor) {
		cout << "New positionsensor module" << endl;
		SC_METHOD(flush);
		sensitive << reset.pos();
		SC_METHOD(new_x_data);
		sensitive << x_reading;
		SC_METHOD(new_y_data);
		sensitive << y_reading;
		SC_METHOD(new_z_data);
		sensitive << z_reading;
	}

};