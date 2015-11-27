#pragma once

#include <cstdint>
#include <winnt.h>

#ifdef __WIN64
#	define X64
#else
#	define x86
#endif

#ifdef X86
	typedef unsigned long size_type;
#else
	typedef size_t size_type;
#endif

typedef uint16_t flag_type;
typedef DWORD image_data_type;

#define stringify(x) stringify_(x)
#define stringify_(x) #x

enum class Endianess : flag_type
{
	little,
	big
};

