#pragma once

#include <SectionHeaderParser.h>

class __declspec(dllexport) SectionParser : public SectionHeaderParser
{
public:
	SectionParser(const std::string& filename);
	SectionParser(const PEHeaderParser& parser);

	std::vector<unsigned char> getSectionRawData(size_type index);
	std::vector<unsigned char> getSectionRawData(const std::string& name);
};