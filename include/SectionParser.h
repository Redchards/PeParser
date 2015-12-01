#pragma once

#include <SectionHeaderParser.h>

class PE_API SectionParser : public SectionHeaderParser
{
public:
	SectionParser(const std::string& filename);
	SectionParser(PEHeaderParser& parser);

	std::vector<unsigned char> retrieveSectionData(size_type index);
	//std::vector<unsigned char> getSectionRawData(const std::string& name);
};