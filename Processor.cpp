#include "pch.h"
#include "Processor.h"
#include <ProcessorStructure.h>

void Processor::Run()
{
	int state = 0;
	ProcessorStructure::IR = ProcessorStructure::MEM[ProcessorStructure::PC];
	while (true)
	{

		switch (state)
		{
		case 0:
			ProcessorStructure::MIR = ProcessorStructure::MPM[ProcessorStructure::MAR];
			state++;
			break;
		case 1:
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

			break;
		default:
			break;
		}
	}
}

bool Processor::G()
{
	return (ProcessorStructure::MIR & 14336) == 14336;
}

void Processor::LdMAR()
{
	switch (ProcessorStructure::MIR & 14336)//#succesor
	{
	case 2048: // jump 0b001 
		ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		break;
	case 4096:// jump 0b010
		if (!((ProcessorStructure::MIR & 128) >> 7) && ProcessorStructure::BE0)//if ac_low
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !ProcessorStructure::BE0)//if nac_low
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		break;
	case 6144:// jump 0b011
		if (!((ProcessorStructure::MIR & 128) >> 7) && ProcessorStructure::BE1)//if cil
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !ProcessorStructure::BE1)//if ncil
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		break;
	case 8192:// jump 0b100
		if (!((ProcessorStructure::MIR & 128) >> 7) && ProcessorStructure::flags.C)//if carry
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !ProcessorStructure::flags.C)//if not carry
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		break;
	case 10240:// jump 0b101
		if (!((ProcessorStructure::MIR & 128) >> 7) && ProcessorStructure::flags.Z)//if zero
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !ProcessorStructure::flags.Z)//if not zero
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		break;
	case 12288:// jump 0b110
		if (!((ProcessorStructure::MIR & 128) >> 7) && ProcessorStructure::flags.N)//if negative
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !ProcessorStructure::flags.N)//if not negative
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		}
		else
		{
			ProcessorStructure::MAR++;
		}
		break;
	case 14336:// jump 0b111
		if (!((ProcessorStructure::MIR & 128) >> 7) && ProcessorStructure::flags.V)//if overflow
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
		}
		else if (((ProcessorStructure::MIR & 128) >> 7) && !ProcessorStructure::flags.V)//if not overflow
		{
			ProcessorStructure::MAR = (ProcessorStructure::MIR & 63) + GetIndexValue();
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
		return (ProcessorStructure::IR & 48) >> 4;
	case 768://011
		return (ProcessorStructure::IR & 3072) >> 10;
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
	if ((ProcessorStructure::MIR & 32768) == 0)
	{
		return 0;
	}
	//class b4
	if ((ProcessorStructure::MIR & 57344) == 57344)
	{
		return 3;
	}
	//class b3
	if ((ProcessorStructure::MIR & 49152) == 49152)
	{
		return 2;
	}
	//class b2
	if ((ProcessorStructure::MIR & 32768) == 32768)
	{
		return 1;
	}

}

void Processor::MemoryOperation()
{
	switch ((ProcessorStructure::MIR & 786432) >> 18)
	{
	case 1:
		ProcessorStructure::IR = ProcessorStructure::MEM[ProcessorStructure::PC];
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
