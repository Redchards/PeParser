#pragma once

#include <Configuration.h>

#include <array>
#include <type_traits>

template<size_type size, class Enabler = void>
class CharToIntegerConverter
{};

template<size_type size>
class CharToIntegerConverter<size, typename std::enable_if<(size <= sizeof(size_type))>::type>
{
public:

	CharToIntegerConverter() : in_{}, out_(0)
	{}
	~CharToIntegerConverter() = default;

	char* in()
	{
		return reinterpret_cast<char*>(in_);
	}

	auto out()
	{
		return charArrayToInteger();
	}
private:
	unsigned char in_[size];
	size_type out_ : (size * 8);

	auto charArrayToInteger()
	{
		for (int64_t i = size - 1; i >= 0; --i)
		{
			out_ |= (in_[i] << (i * 8));
		}
		return out_;
	}
};


template<size_type size>
class CharToIntegerConverter<size, typename std::enable_if<(size > sizeof(size_type))>::type>
{
public:

	CharToIntegerConverter() : in_{}, out_{}
	{};
	~CharToIntegerConverter() = default;
	char* in()
	{
		return reinterpret_cast<char*>(in_);
	}

	auto out()
	{
		return charArrayToInteger();
	}

private:
	unsigned char in_[size];
	std::array<size_type, (size / sizeof(size_t))> out_;

	auto charArrayToInteger()
	{
		for (size_t i = 0; i < sizeof(out_); ++i)
		{
			for (int64_t j = size - 1; j >= 0; --j)
			{
				out_[i] |= (in_[j] << (j * 8));
			}
		}
		return out_;
	}
};