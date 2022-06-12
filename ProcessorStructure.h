#pragma once

class ProcessorStructure
{
public:
	static short int T;
	static unsigned short int PC;
	static unsigned short int SP;
	static short int RG[16];
	static short flags; // NZVC
	static bool BE0;
	static bool BE1;
	static bool BVI;
	static bool BPO;
	static int MEM[65536];
 	static unsigned long long int MPM[116];
	static unsigned short int MAR;
	static unsigned long long int MIR;
	static unsigned short int ADR;
	static short int MDR;
	static int IR;
	static short int SBUS;
	static short int DBUS;
	static short int RBUS;
	static short C;
	static short N;
	static short V;
	static short Z;
};

