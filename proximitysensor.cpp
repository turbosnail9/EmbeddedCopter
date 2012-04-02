#include "systemc.h"
#include "proximitysensor.h"

SC_MODULE(proximitysensor) {
	sc_in<bool> reset;
	sc_in<sc_uint<3> > nearby_object;
	sc_out<sc_uint<3> > warning;

	void flush() {
		warning.write(0);
	}

	void new_data() {
		warning.write(nearby_object.read());
	}

	SC_CTOR(proximitysensor) {
		cout << "New proximitysensor module" << endl;
		SC_METHOD(flush);
		sensitive << reset.pos();
		SC_METHOD(new_data);
		sensitive << nearby_object;
	}

};