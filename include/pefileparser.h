#pragma once

#include <Configuration.h>

#include <PeHeaderParser.h>

// I should make DataDirectoryParser and SectionParser with a std::string constructor, and a rvalue reference to PEHeaderParser constructor


class __declspec(dllexport) PEFileParser final : public PEHeaderParser
{
	public:
	PEFileParser(const std::string& filename);
	~PEFileParser();

	private:
	void init();
};