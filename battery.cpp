#include "systemc.h"

SC_MODULE(battery) {
	sc_in_clk clock;
	sc_in<bool> reset;
	sc_in<sc_uint<8> > level_change;

	sc_out<bool> warning;

	sc_uint<8> level;

	void flush() {
		level = 100;
		warning.write(0);
	}

	void level_changed() {
		if(level_change.read() <= level) level -= level_change.read();
		if(level.to_uint() < 10) warning.write(1);
		if(level.to_uint() < 0) level = 0;
	}

	SC_CTOR(battery) {
		cout << "New battery module" << endl;
		SC_METHOD(flush);
		sensitive << reset.pos();
		SC_METHOD(level_changed);
		sensitive << clock.pos();
	}

};