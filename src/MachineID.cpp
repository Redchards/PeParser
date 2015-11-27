#include <stdafx.h>

#include <MachineID.h>

std::map<MachineID, std::string> machineNameMap{
	{ MachineID::unknown, "Unknown" },
	{ MachineID::i386, "Intel CPU" },
	{ MachineID::i486, "Intel CPU" },
	{ MachineID::pentium, "Intel Pentium CPU" },
	{ MachineID::r3000, "R3000 MIPS CPU" },
	{ MachineID::r4000, "R4000 MIPS CPU" },
	{ MachineID::r10000, "R10000 MIPS CPU" },
	{ MachineID::WCEMipsV2, "Windows CE compatible MIPS CPU" },
	{ MachineID::mips16, "MIPS16 CPU" },
	{ MachineID::mipsfpu, "MIPS FPU" },
	{ MachineID::mipsfpu16, "MIPS16 FPU" },
	{ MachineID::alpha, "Alpha CPU" },
	{ MachineID::sh3, "SuperH 3 CPU" },
	{ MachineID::sh3dsp, "SuperH 3 CPU with DSP support" },
	{ MachineID::sh3e, "SuperH 3e CPU" },
	{ MachineID::sh4, "SuperH 4 CPU" },
	{ MachineID::sh5, "SuperH 5 CPU" },
	{ MachineID::arm, "ARM CPU" },
	{ MachineID::thumb, "ARM Thumb CPU" },
	{ MachineID::armnt, "ARM Thumb-2 CPU"},
	{ MachineID::arm64, "ARM64" },
	{ MachineID::am33, "AM33 CPU" },
	{ MachineID::powerpc, "PowerPC CPU" },
	{ MachineID::powerpcfp, "PowerPC FPU" },
	{ MachineID::ia64, "Intel Itanium CPU" },
	{ MachineID::alpha64, "Intel Itanium CPU" },
	{ MachineID::tricore, "Infineon CPU" },
	{ MachineID::amd64, "Intel CPU" },
	{ MachineID::m32r, "M32R CPU" },
	{ MachineID::ebc, "EFI Bytecode" },
	{ MachineID::cee, "No documentation" },
	{ MachineID::cef, "No documentation" }
};