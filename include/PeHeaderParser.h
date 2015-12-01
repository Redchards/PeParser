#pragma once

#include <CharToInteger.h>
#include <COFFHeaderParser.h>
#include <Configuration.h>
#include <DataDirectoryTable.h>
#include <EpochConverter.h>
#include <FileValueReader.h>
#include <GUISubsystem.h>
#include <HeaderLayout.h>
#include <MachineID.h>
#include <HeaderField.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>




// TODO : Check if magic number is valid, accordingly with x86 or x64
// TODO add noexcept specifier

// NOTE : you may have notice that the function retrieveFieldValue access the disk
// each time it is called. In fact, I think that, given the size of PE/COFF headers,
// this method is more than good enough. Nevertheless, if you find performances problems
// related to this, feel free to use a buffer instead.

// TODO : Use a buffer for optional header ?
// TODO : Better exception management
// TODO : Add check in "retrieveFieldValue" if the offset is correct (using optionalHeaderSize)
// TODO : Section hard limite is 96, add a check for that

// TODO : loadFile and unloadFile won't work properly without virtual init method
class PE_API PEHeaderParser : public COFFHeaderParser
{
	public: 
	PEHeaderParser() = delete;
	PEHeaderParser(PEHeaderParser&&);
	PEHeaderParser(std::string const& filename);
	PEHeaderParser(COFFHeaderParser&&);
	~PEHeaderParser();

	size_type retrieveFieldValue(OptionalHeaderField field);

	bool hasValidMagicNumber();
	size_type getHeaderStart() const noexcept;
	size_type getHeaderEnd() const noexcept;
	size_type getDataDirectoryEnd() const noexcept;

	std::string getVersionAsString(size_type major, size_type minor);
	size_type getMajorOSVersion();
	size_type getMinorOSVersion();
	size_type getMajorImageVersion();
	size_type getMinorImageVersion();
	size_type getMajorSubsystemVersion();
	size_type getMinorSubsystemVersion();

	GUISubsystem getSubsystem();
	std::string getSubsystemName();

	// If not a dll, simply returns an empty vector (field should be empty anyway)
	std::vector<DllCharacteristicFlag> getDllCharacteristics() const;
	std::vector<ConstString> getDllCharacteristicsAsString() const;
	ConstString dllCharacteristicToString(DllCharacteristicFlag flag ) const;
	bool hasDllCharacteristicFlag(DllCharacteristicFlag flag) const;

	size_type numberOfDataDirectories();

	DataDirectoryEntry retrieveDataDirectoryInfos(DataDirectory entry);

	private:
	void init();

	using LayoutType = HeaderFieldInfoHolder<OptionalHeaderField>;
	const LayoutType* layout_;

	const size_type coffHeaderSize_;
	size_type dllFlags_;
	DataDirectoryTable dataDirectoryTable_;
};
