#include "pch.h"
#include "ProcessorStructure.h"

int ProcessorStructure::T = 0;
int ProcessorStructure::PC = 32;
int ProcessorStructure::SP = 65536;
int ProcessorStructure::RG[16] = { 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // initialize all elements with 0
int ProcessorStructure::MEM[65536] = {};
int ProcessorStructure::MAR = 0;
long int ProcessorStructure::MIR = 0;
int ProcessorStructure::ADR = 0;
int ProcessorStructure::MDR = 0;
int ProcessorStructure::IR  = 0;
bool ProcessorStructure::BE0 = false;
bool ProcessorStructure::BE1 = false;
bool ProcessorStructure::BVI = false;
bool ProcessorStructure::BPO = true;
int ProcessorStructure::SBUS = 0;
int ProcessorStructure::DBUS = 0;
int ProcessorStructure::RBUS = 0;
int ProcessorStructure::flags = 0; // NZVC
short ProcessorStructure::C = 0;
short ProcessorStructure::N = 0;
short ProcessorStructure::Z = 0;
short ProcessorStructure::V = 0;
long int ProcessorStructure::MPM[116] = {
	0b000001100001100001001101000000000011,
	0b000000000000000000000001100110001010,
	0b000000000000000000010100100000000100,
	0b000000000000000000010000100000000100,
	0b000000110001100000110000000000000000,
	0b000000010001100111001000000000000000,
	0b000000110001100000000000000000000000,
	0b000001100001100111000000000000000000,
	0b000001110001100010000000000000000000,
	0b000010010001011000110100100000000000,
	0b000000000000000000000000101000001110,
	0b000000000000000000000000101100011001,
	0b000000000000000000000000111000111000,
	0b000000000000000000000000111001011010,
	0b000000100001010100000000101100010100,
	0b000001100001100010001100100000010011,
	0b000000100001100010000000100000010011,
	0b000001100001100010001100000000000000,
	0b100000100011100010000000000000000000,
	0b000010010001010100000000101100010100,
	0b001000000010100100000000110000011110,
	0b000000000000000000000000100000000010,
	0b001000000010100010000000110000011110,
	0b010100000010100010001100000000000000,
	0b001010010011100010000000110000011110,
	0b001000000010100100000000110100100101,
	0b000000000000000000000000100000000010,
	0b001000000010100010000000110000011110,
	0b010100000010100010001100000000000000,
	0b001010010011100010000000110100100101,
	0b000001000001100100000000101100110100,
	0b100001000011100100011000101100110100,
	0b100001010011100100011100101100110100,
	0b100001010011000000011100111100000000,
	0b100001000101100100100000101100110100,
	0b100001000110100100100000101100110100,
	0b100001000111100100100000101100110100,
	0b101100000010100100100000101100110100,
	0b100100000010100100100000101100110100,
	0b100010110011100100011100101100110100,
	0b100011000011100100011000101100110100,
	0b100000001000100100011000101100110100,
	0b100000001001100100011000101100110100,
	0b100000001010100100100000101100110100,
	0b100000001011100100100000101100110100,
	0b100000001100100100100000101100110100,
	0b100000001101100100100000101100110100,
	0b100000001110100100100000101100110100,
	0b000000110001100011001000111100000000,
	0b000010000001010100000000000000000000,
	0b000000110001100010000000000000000000,
	0b000001000001100000000100101100110100,
	0b000010010001001100000000111100000000,
	0b000000000000000000000000100000000010,
	0b000000000000000011000000111100000000,
	0b000000000000000011000000111100000000,
	0b000000000000000000000010101101010100,
	0b000000000000000000000000111100000000,
	0b000000000000000000000010101111010100,
	0b000000000000000000000000111100000000,
	0b000000000000000000000011001101010100,
	0b000000000000000000000000111100000000,
	0b000000000000000000000011001111010100,
	0b000000000000000000000000111100000000,
	0b000000000000000000000010001101010100,
	0b000000000000000000000000111100000000,
	0b000000000000000000000010001111010100,
	0b000000000000000000000000111100000000,
	0b000000000000000000000011101101010100,
	0b000000000000000000000000111100000000,
	0b000000000000000000000011101111010100,
	0b000000000000000000000000111100000000,
	0b000000000000000000000000101101010100,
	0b000000000000000000000000000000000000,
	0b000000000000000000000000101101001011,
	0b000000000000000000000000100000000010,
	0b000001100001100010001100100001010000,
	0b001000000010010100000000100001010001,
	0b000001100001100010001100000000000000,
	0b001010010011010100000000100001010001,
	0b010110010011010100000000000000000000,
	0b000000110001100000001000000000000000,
	0b000001100001100111000000000000000000,
	0b000001000001011000000000111100000000,
	0b000000000000000000000000100000000010,
	0b000001100001100010001100100001011001,
	0b001000000010011000000000111100000000,
	0b000001100001100010001100000000000000,
	0b001010010011011000000000111100000000,
	0b010110010011011000000000111100000000,
	0b101000010101000100000000111100000000,
	0b000000000000000000000000000000000000,
	0b101000010110000100000000111100000000,
	0b000000000000000000000000000000000000,
	0b000000000000000000000000111100000000,
	0b000000000000000000000000000000000000,
	0b000000000000000000101100000000000000,
	0b000000000000000000000000000000000000,
	0b000000000000000000100100111100000000,
	0b000000000000000000000000000000000000,
	0b000000000000000000101000100000000000,
	0b000000000000000000000000000000000000,
	0b000000110001100000001000000000000000,
	0b000001100001100111000000111100000000,
	0b000000110001100010000000000000000000,
	0b000010010001011000000100111100000000,
	0b000000110001100000001000000000000000,
	0b000000010001100111000000111100000000,
	0b000000110001100010000000000000000000,
	0b000010010001000100000100111100000000,
	0b000000110001100010000000000000000000,
	0b000010010001011000000100111100000000,
	0b000000110001100010000000000000000000,
	0b000010010001011000000100000000000000,
	0b000000110001100010000000000000000000,
	0b000010010001000100000100111100000000
};