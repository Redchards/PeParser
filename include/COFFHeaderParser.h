#pragma once

#include <BasicParser.h>
#include <Configuration.h>
#include <Characteristics.h>
#include <EpochConverter.h>
#include <HeaderLayout.h>
#include <FileValueReader.h>
#include <MachineID.h>
#include <Utility.h>

#include <string>
#include <vector>

class PE_API COFFHeaderParser : public BasicParser
{
public:
	COFFHeaderParser() = delete;
	COFFHeaderParser(COFFHeaderParser&& other);
	COFFHeaderParser(std::string const& filename);
	~COFFHeaderParser() = default;
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
	ConstString const getMachineName();

	std::vector<CharacteristicFlag> getCharacteristics() const;
	std::vector<ConstString> getCharacteristicsAsString() const;
	ConstString characteristicToString(CharacteristicFlag flag) const;
	bool hasCharacteristicFlag(CharacteristicFlag flag) const;

	bool isObjectFile() const noexcept;
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
	void init();

	size_type fileFlags_;
	size_type lfanewValue_;
	bool isObjectFile_;

	using LayoutType = HeaderFieldInfoHolder<COFFHeaderField>;
	static const LayoutType* layout_;
};