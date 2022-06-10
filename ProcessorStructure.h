#pragma once

class ProcessorStructure
{
public:
	static int T;
	static int PC;
	static int SP;
	static int RG[16];
	static int flags; // NZVC
	static bool BE0;
	static bool BE1;
	static bool BVI;
	static bool BPO;
	static int MEM[65536];
 	static long int MPM[116];
	static int MAR;
	static long int MIR;
	static int ADR;
	static int MDR;
	static int IR;
	static int SBUS;
	static int DBUS;
	static int RBUS;
	static short C;
	static short N;
	static short V;
	static short Z;
};

