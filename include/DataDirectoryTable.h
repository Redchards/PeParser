#pragma once

#include <Configuration.h>

#include <IteratableEnum.h>
#include <type_traits>

// Redefinition of the the IMAGE_FILE_MACHINE_IMAGE_DATA_DIRECTORY structure to blend better with the code


struct DataDirectoryEntry
{
	uint32_t RVA;
	uint32_t size;
};

// A wrapper around an enum class to represent all data directory in the data directory table.
// The enum can be iterated with conventional methods such as range-based loops, or simple operator++
// If you want to add values in the DataDirectory enum, please also change "begin" and "end" values accordingly

enum class DataDirectory : flag_type
{
	exports = 0, // exports instead of export, because visual studio won't let me do this right know. Sounds like a bug (was working in previous version, and other compilers)
	imports,
	resource,
	exception,
	certificate,
	relocation,
	debug,
	architecture,
	globalPointer,
	tls,
	WinNTConfig,
	boundImport,
	importAddress,
	importDelay,
	clr,
	reserved,

	end,
	begin = exports
};

using DataDirectoryTable = IteratableEnum<DataDirectory>;
