#pragma once

#include <DataDirectoryTable.h>
#include <HeaderLayout.h>

// TODO, refactor it, PEHeaderParser should be able to read sections infos. This class should be an interpreter of these informations only !

class __declspec(dllexport) DataTableParser : public BasicParser
{
public:
	DataTableParser(const std::string& filename);
	DataTableParser(const PEHeaderParser& parser);

	DataDirectoryEntry retrieveTableInfos(DataDirectory table);

private:
	DataDirectoryTable dataDirectoryTable_;
	std::string filename_;
	size_type dataDirectoryStart_;
};