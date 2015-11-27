#pragma once

#include <Configuration.h>

struct HeaderField
{
	// Compiler generated constructors are not constexpr right now.
	// I can't use constexpr field for now anyway, as the compiler as a bug right now (VS 2015 Preview Edition)
	// All constexpr object fields are initialized to zero, even if you initialize them.
	// For instance, in PeParser, if I replace "const" by "static constexpr", and I initialize the fields as I would do
	// for a normal static field, offset and size would be zero. If I remove the "constexpr" keyword, everything works as expected,
	// but it's not constexpr :'(
	/*constexpr HeaderField() : offset(0), size(0)
	{}

	constexpr HeaderField(size_type offset_, size_type size_) : offset(offset_), size(size_)
	{}*/

	bool operator==(HeaderField const& other)
	{

		return (offset == other.offset && size == other.size);
	}

	bool operator!=(HeaderField const& other)
	{
		return !(*this == other);
	}

	size_type offset;
	size_type size;
};