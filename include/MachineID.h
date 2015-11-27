#pragma once

#include <Configuration.h>

#include <map>
#include <string>

// The way machine type enum is presented is not satisfying. So here is a little enum to make everything clearer
// Also, strongly typed enum ennsure that we are dealing with the right type of data
// Every architectures that support PE/COFF file format is little endian, so we don't have to make distiction
// about endianess, we assume little endian in parsing.
// Please remember that some of the architecture below may or may not be little endian in some case (ARM for instance).
// But in this case, it wouldn't be compatible with PE/COFF.
// So all the cases listed below are the little endian version of the architecture.

// All values comes from the "winnt.h" header

#define IMAGE_FILE_MACHINE_ARM64 0xaa64 // Not present in some winnt.h, but according the microsoft's COFF specification, you could meet this flag 
// You could eventually see 0x14d for i486 and 0x14e for pentium, but coff specification do not say anything about that
#define IMAGE_FILE_MACHINE_I486 0x14d
#define IMAGE_FILE_MACHINE_PENTIUM 0x14e
// There was also a flag for big endian r3000 mips cpu, but as everything else is big endian, but as windows is only little endian today,
// I don't plan to support big endian parsing
// If you want to, you can use the specialization of FileValueReader<Endianess::big> in parsers, which will give you big endian value retrieval
// (Acutally, windows NT kernel run on big endian, so it might be your case)
enum class MachineID : flag_type
{
	// Assumed general purpose PE file
	unknown = IMAGE_FILE_MACHINE_UNKNOWN,

	// Intel 386 or later.
	i386 = IMAGE_FILE_MACHINE_I386,
	i486 = IMAGE_FILE_MACHINE_I486,
	pentium = IMAGE_FILE_MACHINE_PENTIUM,

	// MIPS architecture
	r3000 = IMAGE_FILE_MACHINE_R3000,
	r4000 = IMAGE_FILE_MACHINE_R4000,
	r10000 = IMAGE_FILE_MACHINE_R10000,
	WCEMipsV2 = IMAGE_FILE_MACHINE_WCEMIPSV2,
	mips16 = IMAGE_FILE_MACHINE_MIPS16,
	// FPU versions
	mipsfpu = IMAGE_FILE_MACHINE_MIPSFPU,
	mipsfpu16 = IMAGE_FILE_MACHINE_MIPSFPU16,

	// Alpha architecture
	// Almost extinct
	alpha = IMAGE_FILE_MACHINE_ALPHA,

	// SuperH architecture
	// Very uncommon
	sh3 = IMAGE_FILE_MACHINE_SH3,
	sh3dsp = IMAGE_FILE_MACHINE_SH3DSP,
	sh3e = IMAGE_FILE_MACHINE_SH3E,
	sh4 = IMAGE_FILE_MACHINE_SH4,
	sh5 = IMAGE_FILE_MACHINE_SH5,

	// ARM architecture
	arm = IMAGE_FILE_MACHINE_ARM,
	// ARM CPU with Thumb decompressor
	thumb = IMAGE_FILE_MACHINE_THUMB,
	armnt = IMAGE_FILE_MACHINE_ARMNT,
	arm64 = IMAGE_FILE_MACHINE_ARM64,

	// AM33 architecture
	// Suitable for SOC.
	am33 = IMAGE_FILE_MACHINE_AM33,

	// PowerPC architecture
	powerpc = IMAGE_FILE_MACHINE_POWERPC,
	// FPU version
	powerpcfp = IMAGE_FILE_MACHINE_POWERPCFP,

	// Intel Itanium 64 architecture
	// Server side CPUs
	ia64 = IMAGE_FILE_MACHINE_IA64,
	alpha64 = IMAGE_FILE_MACHINE_ALPHA64,

	// Infineon's architecture
	tricore = IMAGE_FILE_MACHINE_TRICORE,

	// Basically an other name for x86_64
	amd64 = IMAGE_FILE_MACHINE_AMD64,

	// M32R architecture
	m32r = IMAGE_FILE_MACHINE_M32R,

	// EFI Bytecode
	ebc = IMAGE_FILE_MACHINE_EBC,

	// No documentation on this, no idea what it could be...
	// Feel free to add a description to the MachineNameMap in "MachineID.cpp"
	cee = IMAGE_FILE_MACHINE_CEE,
	cef = IMAGE_FILE_MACHINE_CEF
};

extern std::map<MachineID, std::string> machineNameMap;