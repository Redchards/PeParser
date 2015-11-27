#pragma once

#include <BasicParser.h>
#include <Configuration.h>
#include <HeaderLayout.h>
#include <FileValueReader.h>
#include <MachineID.h>

#include <string>
#include <vector>

// TODO : Implement a function "getCharacteristicsAsString" (all characteristic as string directly in the vector), to avoid calling repeatedly "getCharacteristicAsString"

class __declspec(dllexport) COFFHeaderParser : public BasicParser
{
public:
	COFFHeaderParser() = delete;
	COFFHeaderParser(COFFHeaderParser&& other);
	COFFHeaderParser(std::string const& filename);
	virtual ~COFFHeaderParser();
	// the lfanew file is actually in the DOS header, but as PE file do almost not use this header
	// (it basically only contains the dos signature, lfanew and a stub), I will not create an other
	// class only to parse this kind of datas
	bool hasDOSSignature();
	size_type getHeaderStart() const noexcept;
	size_type getHeaderEnd() const noexcept;
	bool hasPESignature();

	MachineID const getMachineID();
	size_type const getNumberOfSections();
	size_type getLfanew() const noexcept;

	std::string const getFileCreationDate();
	std::string const getMachineName();

	std::vector<CharacteristicFlag> getCharacteristics() const;
	std::string getCharacteristicAsString(CharacteristicFlag flag) const;
	bool hasCharacteristicFlag(CharacteristicFlag flag) const;

	bool is32bit();
	bool is64bit();
	bool isSystemFile();
	bool isDll();
	bool isExecutable();
	bool isExecutableImage();
	bool isLargeAddressAware();
	bool hasRelocations();
	bool hasLineNumber();
	bool hasDebugSymbols();

	size_type retrieveFieldValue(COFFHeaderField field);

protected:

	template<typename T>
	bool hasFlag(size_type allFlags, T flag) const;
	void init();

	size_type fileFlags_;
	size_type lfanewValue_;
	bool isObjectFile_;

	using LayoutType = HeaderFieldInfoHolder<COFFHeaderField>;
	static const LayoutType* layout_;
};

template<typename T>
bool COFFHeaderParser::hasFlag(size_type allFlags, T flag) const
{
	return ((allFlags & static_cast<size_type>(flag)) == (static_cast<size_type>(flag)));
}