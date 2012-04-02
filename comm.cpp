#include "systemc.h"
#include "comm.h"

SC_MODULE(comm) {
	sc_in<bool> reset;
	
	sc_in<sc_uint<8> > signal_quality;
	sc_in<sc_uint<8> > external_command;
	sc_in<sc_uint<8> > external_command_arg1;
	sc_in<sc_uint<8> > external_command_arg2;
	sc_in<sc_uint<8> > external_command_arg3;

	sc_out<bool> connected;
	sc_out<sc_uint<8> > internal_command;
	sc_out<sc_uint<8> > internal_command_arg1;
	sc_out<sc_uint<8> > internal_command_arg2;
	sc_out<sc_uint<8> > internal_command_arg3;

	void flush() {
		connected.write(0);
		internal_command.write(0);
		internal_command_arg1.write(0);
		internal_command_arg2.write(0);
		internal_command_arg3.write(0);
	}

	void new_command() {
		if(connected.read()) {
			internal_command.write(external_command.read());
			internal_command_arg1.write(external_command.read());
			internal_command_arg2.write(external_command.read());
			internal_command_arg3.write(external_command.read());
		}
	}

	void signal_quality_change() {
		if(signal_quality.read().to_uint() < 10) connected.write(0);
		else connected.write(1);
	}

	SC_CTOR(comm) {
		cout << "New comm module" << endl;
		SC_METHOD(flush);
		sensitive << reset.pos();
		SC_METHOD(new_command);
		sensitive << external_command;
		SC_METHOD(signal_quality_change);
		sensitive << signal_quality;
	}

};