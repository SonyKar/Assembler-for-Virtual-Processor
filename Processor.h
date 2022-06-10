#pragma once
class Processor
{
public:
	static void RunStep();
	static void RunHalt();
	static void Run();
	static bool G();
	static void LdMAR();
	static int GetIndexValue();
	static int GetInstructionClass();
	static void MemoryOperation();
	static void Decode();
	static void DecodeDBus();
	static void DecodeSBus();
	static void DecodeALU();
	static void DecodeRBus();
	static void DecodeOther();
	static int state;
};

