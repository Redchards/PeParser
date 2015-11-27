#pragma once

#include <Configuration.h>

// structure equivalent to the _IMAGE_DATA_DIRECTORY structure in "winnt.h"
// I may want to change constructors at one point, that's the only reason (add constexpr constructor maybe)

struct PEDataDirectory
{
	constexpr PEDataDirectory() : va(0), size(0)
	{}

	constexpr PEDataDirectory(image_data_type va_, image_data_type size_) : va(va_), size(size_)
	{}

	image_data_type va;
	image_data_type size;
};