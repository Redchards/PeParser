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
	export = 0,
	import,
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
	begin = export
};

using DataDirectoryTable = IteratableEnum<DataDirectory>;

/*class DataDirectoryTable
{
	enum class DataDirectory : flag_type;

public:
	constexpr DataDirectoryTable() = default;

	DataDirectory operator*(DataDirectoryTable falsePointer) const;
	static DataDirectory begin();
	static DataDirectory end();
	size_type findPosition(DataDirectory dir) const;
	DataDirectory const at(size_type const position) const;
	size_type size() const;

	enum class DataDirectory : flag_type
	{
		export = 0,
		import,
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

		begin_ = export,
		end_ = reserved
	};
};*/
