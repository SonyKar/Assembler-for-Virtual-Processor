#pragma once

struct FLAG {
	int N = 0;
	int Z = 0;
	int V = 0;
	int C = 0;
};

class ProcessorStructure
{
public:
	static int T;
	static int PC;
	static int SP;
	static int RG[16];
	static FLAG flags;
	static int MEM[65536];
	static int MPM[105];
	static int MAR;
	static int MIR;
	static int ADR;
	static int MDR;
	static int IR;
	static int SBUS;
	static int DBUS;
	static int RBUS;
};

