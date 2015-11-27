#include <stdafx.h>

#include <DataTableParser.h>

// Very dumb constructor, you should always prefer using the second if you alreay have a PEHeaderParser instantiated.
DataTableParser::DataTableParser(const std::string& filename) : BasicParser(filename)
{
	PEHeaderParser tmp(filename);
	dataDirectoryStart_ = tmp.getHeaderEnd();
}

DataTableParser::DataTableParser(const PEHeaderParser& parser) : BasicParser(parser.getFileName()),
																 dataDirectoryStart_(parser.getHeaderEnd())
{}

DataDirectoryEntry DataTableParser::retrieveTableInfos(DataDirectory table)
{
	const size_t tableOffset = dataDirectoryStart_ + (dataDirectoryTable_.findPosition(table)*sizeof(DataDirectoryEntry));
	return {
			 static_cast<uint32_t>(reader_.retrieveValue(tableOffset, sizeof(DataDirectoryEntry::RVA))),
		     static_cast<uint32_t>(reader_.retrieveValue(tableOffset + sizeof(DataDirectoryEntry::RVA), sizeof(DataDirectoryEntry::size)))
		   };
	//reader_.retrieveValue(getHeaderEnd(), sizeof(DataDirectoryEntry));
}