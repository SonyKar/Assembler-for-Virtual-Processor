#include "pch.h"
#include "Processor.h"
#include <ProcessorStructure.h>

int Processor::state = 0;

void Processor::RunStep()
{
	if (ProcessorStructure::BPO)
	{
		Run();
	}
}

void Processor::RunHalt()
{
	while (ProcessorStructure::BPO)
	{
		Run();
	}
}

void Processor::Run()
{
	switch (state)
	{
	case 0:
		ProcessorStructure::MIR = ProcessorStructure::MPM[ProcessorStructure::MAR];
		state++;
		break;
	case 1:
		Decode();
		if (G())
		{
			LdMAR();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		if (!(ProcessorStructure::MIR & 786432))
		{
			state = 0;
		}
		else
		{
			state = 2;
		}
		break;
	case 2:
		state = 3;
		break;
	case 3:
		MemoryOperation();
		state = 0;
		break;
	default:
		break;
	}
	
}

void Processor::Decode()
{
	DecodeDBus();
	DecodeSBus();
	DecodeALU();
	DecodeRBus();
	DecodeOther();
}

void Processor::DecodeDBus()
{
	switch ((ProcessorStructure::MIR & 64424509440) >> 32)
	{
	case 0:
		break;
	case 1: // PdFlagd
		ProcessorStructure::DBUS = ProcessorStructure::flags;
		break;
	case 2: // PdRgd
		ProcessorStructure::DBUS = ProcessorStructure::RG[ProcessorStructure::IR & 15];
		break;
	case 3: // PdSpd
		ProcessorStructure::DBUS = ProcessorStructure::SP;
		break;
	case 4: //PdTd
		ProcessorStructure::DBUS = ProcessorStructure::T;
		break;
	case 5: //PdPcd
		ProcessorStructure::DBUS = ProcessorStructure::PC;
		break;
	case 6: //PdIVRd
		break;
	case 7: // PdADRd
		ProcessorStructure::DBUS = ProcessorStructure::ADR;
		break;
	case 8: // PdMDRd
		ProcessorStructure::DBUS = ProcessorStructure::MDR;
		break;
	case 9: // !PdMDRd
		ProcessorStructure::DBUS = ~ProcessorStructure::MDR;
		break;
	case 10: // PdIR[7..0]d
		ProcessorStructure::DBUS = ProcessorStructure::IR & 255;
		break;
	case 11: // Pd0d
		ProcessorStructure::DBUS = 0;
		break;
	case 12: // Pd-1d
		ProcessorStructure::DBUS = -1;
		break;
	default:
		break;
	}
}

void Processor::DecodeSBus()
{
	switch ((ProcessorStructure::MIR & 4026531840) >> 28)
	{
	case 0:
		break;
	case 1: // PdFlags
		ProcessorStructure::SBUS = ProcessorStructure::flags;
		break;
	case 2: // PdRgS
		ProcessorStructure::SBUS = ProcessorStructure::RG[(ProcessorStructure::IR & 960) >> 6];
		break;
	case 3: // PdSps
		ProcessorStructure::SBUS = ProcessorStructure::SP;
		break;
	case 4: //PdTd
		ProcessorStructure::SBUS = ProcessorStructure::T;
		break;
	case 5: //!PdTS
		ProcessorStructure::SBUS = ~ProcessorStructure::T;
		break;
	case 6: //PdPcs
		ProcessorStructure::SBUS = ProcessorStructure::PC;
		break;
	case 7: //PdIVRs
		break;
	case 8: // PdADRs
		ProcessorStructure::SBUS = ProcessorStructure::ADR;
		break;
	case 9: // PdMDRs
		ProcessorStructure::SBUS = ProcessorStructure::MDR;
		break;
	case 10: // PdIR[7..0]s
		ProcessorStructure::SBUS = ProcessorStructure::IR & 255;
		break;
	case 11: // Pd0s
		ProcessorStructure::SBUS = 0;
		break;
	case 12: // Pd-1s
		ProcessorStructure::SBUS = -1;
		break;
	default:
		break;
	}
}

void Processor::DecodeALU()
{
	ProcessorStructure::flags = 0;
	int signBit = (ProcessorStructure::RBUS & 8000) >> 15;
	switch ((ProcessorStructure::MIR & 251658240) >> 24)
	{
	case 0:
		break;
	case 1: // SBUS
		ProcessorStructure::RBUS = ProcessorStructure::SBUS;
		break;
	case 2: // DBUS
		ProcessorStructure::RBUS = ProcessorStructure::DBUS;
		break;
	case 3: // ADD
		ProcessorStructure::RBUS = ProcessorStructure::DBUS + ProcessorStructure::SBUS;
		break;
	case 4: // SUB
		ProcessorStructure::RBUS = ProcessorStructure::DBUS - ProcessorStructure::SBUS;
		break;
	case 5: // AND
		ProcessorStructure::RBUS = ProcessorStructure::DBUS & ProcessorStructure::SBUS;
		break;
	case 6: // OR
		ProcessorStructure::RBUS = ProcessorStructure::DBUS | ProcessorStructure::SBUS;
		break;
	case 7: // XOR
		ProcessorStructure::RBUS = ProcessorStructure::DBUS ^ ProcessorStructure::SBUS;
		break;
	case 8: // ASL
		ProcessorStructure::RBUS = (ProcessorStructure::DBUS << ProcessorStructure::SBUS) & 65535;
		break;
	case 9: // ASR
		ProcessorStructure::RBUS = ProcessorStructure::DBUS >> ProcessorStructure::SBUS;
		break;
	case 10: // LSR
	{
		unsigned int unsignedTmp = ProcessorStructure::DBUS;
		ProcessorStructure::RBUS = unsignedTmp >> ProcessorStructure::SBUS;
	}
		break;
	case 11: // ROL
	{
		short firstBit = ProcessorStructure::DBUS >> 15;
		ProcessorStructure::RBUS = (ProcessorStructure::DBUS << ProcessorStructure::SBUS) & 65535;
		ProcessorStructure::RBUS += firstBit;
	}
		break;
	case 12: // ROR
	{
		short lastBit = ProcessorStructure::DBUS & 1;
		ProcessorStructure::RBUS = ProcessorStructure::DBUS >> ProcessorStructure::SBUS;
		ProcessorStructure::RBUS += (lastBit << 15);
	}
		break;
	
	case 13: // RLC ??
		break;
	case 14: // RRC ??
		break;
	default:
		break;
	}
	
	if (ProcessorStructure::RBUS == 0)
	{
		ProcessorStructure::flags = 0b100;
	}
	else if (ProcessorStructure::RBUS < 0)
	{
		ProcessorStructure::flags = 0b1000;
	}
	if ((ProcessorStructure::RBUS & 65536) && (signBit != (ProcessorStructure::RBUS & 8000) >> 15)) // overflow & carry
	{
		ProcessorStructure::flags |= 0b11;
	}
	else if (ProcessorStructure::RBUS & 65536) // carry
	{
		ProcessorStructure::flags |= 0b1;
	}
	else if (signBit != (ProcessorStructure::RBUS & 8000) >> 15) // overflow
	{
		ProcessorStructure::flags |= 0b10;
	}
}

void Processor::DecodeRBus()
{
	switch ((ProcessorStructure::MIR & 15728640) >> 20)
	{
	case 0:
		break;
	case 1: // PmFlag
		ProcessorStructure::flags = ProcessorStructure::RBUS;
		break;
	case 2: // PmFlag[3..0]
		ProcessorStructure::flags = ProcessorStructure::RBUS;
		break;
	case 3: // PmRg
		ProcessorStructure::RG[ProcessorStructure::IR & 15] = ProcessorStructure::RBUS;
		break;
	case 4: // PmSp
		ProcessorStructure::SP = ProcessorStructure::RBUS;
		break;
	case 5: //PmT
		ProcessorStructure::T = ProcessorStructure::RBUS;
		break;
	case 6: //PmPc
		ProcessorStructure::PC = ProcessorStructure::RBUS;
		break;
	case 7: //PmIVR
		break;
	case 8: // PmADR
		ProcessorStructure::ADR = ProcessorStructure::RBUS;
		break;
	case 9: // PmMDR
		ProcessorStructure::MDR = ProcessorStructure::RBUS;
		break;
	default:
		break;
	}
}

void Processor::DecodeOther()
{
	switch ((ProcessorStructure::MIR & 245760) >> 14)
	{
	case 0:
		break;
	case 1: // +2SP
		ProcessorStructure::SP += 1;
		break;
	case 2: // -2SP
		ProcessorStructure::SP -= 1;
		break;
	case 3: // +2PC
		ProcessorStructure::PC += 1;
		break;
	case 4: // A(1)BE0
		ProcessorStructure::BE0 = 1;
		break;
	case 5: // A(1)BE1
		ProcessorStructure::BE1 = 1;
		break;
	case 6: // CONDa  (NZVC)
		ProcessorStructure::C = ProcessorStructure::flags & 1;
		ProcessorStructure::V = (ProcessorStructure::flags & 10) >> 1;
		ProcessorStructure::Z = (ProcessorStructure::flags & 100) >> 2;
		ProcessorStructure::N = (ProcessorStructure::flags & 1000) >> 3;
		break;
	case 7: // Cin, CONDa  (NZVC)
		ProcessorStructure::C = ProcessorStructure::flags & 1;
		ProcessorStructure::V = (ProcessorStructure::flags & 10) >> 1;
		ProcessorStructure::Z = (ProcessorStructure::flags & 100) >> 2;
		ProcessorStructure::N = (ProcessorStructure::flags & 1000) >> 3;
		ProcessorStructure::flags |= 1;
		break;
	case 8: // CONDl  (NZVC)
		ProcessorStructure::V = (ProcessorStructure::flags & 10) >> 1;
		ProcessorStructure::Z = (ProcessorStructure::flags & 100) >> 2;
		break;
	case 9: // A(1)BVI
		ProcessorStructure::BVI = 1;
		break;
	case 10: // A(0)BVI
		ProcessorStructure::BVI = 0;
		break;
	case 11: // A(0)BPO
		ProcessorStructure::BPO = 0;
		break;
	case 12: // INTA, -2sp
		break;
	case 13: // A(0)BE, A(0)BI
		ProcessorStructure::BE0 = 0;
		ProcessorStructure::BE1 = 0;
		break;
	default:
		break;
	}
}

bool Processor::G()
{
	return (ProcessorStructure::MIR & 14336) != 0;
}

void Processor::LdMAR()
{
	switch (ProcessorStructure::MIR & 14336)//#succesor
	{
	case 2048: // jump 0b001 
		ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		break;
	case 4096:// jump 0b010
		if (!((ProcessorStructure::MIR & 128) >> 7) && ProcessorStructure::BE0)//if ac_low
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !ProcessorStructure::BE0)//if nac_low
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		break;
	case 6144:// jump 0b011
		if (!((ProcessorStructure::MIR & 128) >> 7) && ProcessorStructure::BE1)//if cil
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !ProcessorStructure::BE1)//if ncil
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		break;
	case 8192:// jump 0b100
		if (!((ProcessorStructure::MIR & 128) >> 7) && (ProcessorStructure::flags & 1))//if carry
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !(ProcessorStructure::flags & 1))//if not carry
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		break;
	case 10240:// jump 0b101
		if (!((ProcessorStructure::MIR & 128) >> 7) && ((ProcessorStructure::flags & 100) >> 2))//if zero
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !((ProcessorStructure::flags & 100) >> 2))//if not zero
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		break;
	case 12288:// jump 0b110
		if (!((ProcessorStructure::MIR & 128) >> 7) && ((ProcessorStructure::flags & 1000) >> 3))//if negative
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !((ProcessorStructure::flags & 1000) >> 3))//if not negative
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		break;
	case 14336:// jump 0b111
		if (!((ProcessorStructure::MIR & 128) >> 7) && ((ProcessorStructure::flags & 10) >> 1))//if overflow
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !((ProcessorStructure::flags & 10) >> 1))//if not overflow
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 127) + GetIndexValue();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		break;
	default:
		break;
	}
}

int Processor::GetIndexValue()
{
	switch (ProcessorStructure::MIR & 1792)//#index
	{
	case 0:
		return 0;
	case 256://001
		return GetInstructionClass();
	case 512://010
		return (ProcessorStructure::IR & 3072) >> 10;
	case 768://011
		return (ProcessorStructure::IR & 48) >> 4;
	case 1024://100
		return (ProcessorStructure::IR & 28672) >> 12;//opcode class b1
	case 1280://101
		return (ProcessorStructure::IR & 960) >> 6;//opcode class b2
	case 1536://110
		if (GetInstructionClass() == 2)
		{
			return (ProcessorStructure::IR & 3840) >> 7;//opcode class b3
		}
		else
		{
			return (ProcessorStructure::IR & 31) << 1;//opcode class b4
		}
	case 1792://111
		return 0;
	default:
		break;
	}
}

int Processor::GetInstructionClass()
{
	//class b1
	if ((ProcessorStructure::IR & 32768) == 0)
	{
		return 0;
	}
	//class b4
	if ((ProcessorStructure::IR & 57344) == 57344)
	{
		return 3;
	}
	//class b3
	if ((ProcessorStructure::IR & 49152) == 49152)
	{
		return 2;
	}
	//class b2
	if ((ProcessorStructure::IR & 32768) == 32768)
	{
		return 1;
	}

}

void Processor::MemoryOperation()
{
	switch ((ProcessorStructure::MIR & 786432) >> 18)
	{
	case 1:
		ProcessorStructure::IR = ProcessorStructure::MEM[ProcessorStructure::ADR];
		break;
	case 2:
		ProcessorStructure::MDR = ProcessorStructure::MEM[ProcessorStructure::ADR];
		break;
	case 3:
		ProcessorStructure::MEM[ProcessorStructure::ADR] = ProcessorStructure::MDR;
		break;
	default:
		break;
	}
}
