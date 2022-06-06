#include "pch.h"
#include "ProcessorStructure.h"

int ProcessorStructure::T = 0;
int ProcessorStructure::PC = 32;
int ProcessorStructure::SP = 65536;
int ProcessorStructure::RG[16] = {}; // initialize all elements with 0
int ProcessorStructure::MEM[65536] = {};
int ProcessorStructure::MPM[105] = {};
int ProcessorStructure::MAR = 0;
int ProcessorStructure::MIR = 0;
int ProcessorStructure::ADR = 0;
int ProcessorStructure::MDR = 0;
int ProcessorStructure::IR = 0;
int ProcessorStructure::SBUS = 0;
int ProcessorStructure::DBUS = 0;
int ProcessorStructure::RBUS = 0;