#pragma once

#include <Configuration.h>
#include <PeDataDirectory.h>
#include <HeaderField.h>
#include <IteratableEnum.h>
//template<size_t value>
//using HeaderField = std::integral_constant<size_t, value>;

#include <algorithm>
#include <array>
#include <utility>

// Some class used to define PE/COFF file header layout.
// More informations in ECMA-335 partition 2
// However, the documentation stay vague on some points. So this link gives a fair amount of details, listed here
// in the case it's important for the code.
// https://code.google.com/p/corkami/wiki/PE


// NOTE : the only PEFileField which has an offset starting from the beggining of the file
// is the "lfanew" field.
// Every other fields must have an offset starting from the "machineId" field.
// Hence, you should not try to use the "lfanew" field with the method
// PEHeaderParser::retrieveFieldValue (the field is protected for that reason)


//using HeaderFieldInfo = std::pair<size_t, size_t>;
class HeaderLayout;

enum class CPUSize : uint8_t
{
	CPU32 = 0,
	CPU64 = 1
};



class HeaderLayout {};

class PrimaryPEData;

template<CPUSize size>
class OptionalHeaderLayout;

// TODO : Remove magic numbers
// These two enumerations describe the PE file header as it's described in the WINNT.h.
// Enumerations are better for one thing : as C++ do not have reflection (yet), it's easier to manipulate enum than
// to manipulate structs. Here, I use enums as mere accessors to the field data.

enum class COFFHeaderField : flag_type
{
	// ID of the target machine. See the "MachineID.h" file for more informations.
	Machine = 0,
	NumberOfSections,
	TimeDateStamp,
	// Pointer to COFF symbol table. Set to 0 for managed file.
	PointerToSymboleTable,
	// Number of COFF debugging symbols. Set to 0 for managed file.
	NumberOfSymbols,
	// Defined in documentations as the size of optional header (called PE header in my parser, because it's not optional). It is not really true.
	// It's best described by "the delta between the optional header and the start of sections".
	// In practice, it is often the size of the optional header + the alignement.
	SizeOfOptionalHeader,
	// Flags describing the file features. See the "Characteristics.h" file for more informations.
	Characteristics,

	// Required for the IteratableEnum trick to work
	end,
	begin = Machine
};

enum class OptionalHeaderField : flag_type
{
	// Magic number. Different values are listed below
	Magic = 0,
	// Basically the linker version. As everything about version in this header, it might be highly unreliable.
	MajorLinkerVersion,
	MinorLinkerVersion,
	// Size of all the .text sections
	SizeOfCode,
	// Size of constant initialized data (section varying)
	SizeOfInitializedData,
	// Size of all the .bss sections
	SizeOfUninitializedData,
	// Can be 0 if unnmanaged. Points to the CLR invocation stub otherwise.
	AddressOfEntryPoint,
	// Beggining of the first .text section. Highly unreliable.
	BaseOfCode,
	// Beggining of the data section. Do not exist in 64 bit. Highly unreliable.
	BaseOfData,
	// Prefered starting address, aligned on 0x10000 bits (64kb) boundary.
	ImageBase,
	// Memory page size by default. Must be greater than or equal to SectionAlignment
	SectionAlignment,
	FileAlignment,
	// As every other version field, unreliable and not interesting.
	MajorOperatingSystemVersion,
	MinorOperatingSystemVersion,
	MajorImageVersion,
	MinorImageVersion,
	MajorSubsystemVersion,
	MinorSubsystemVersion,
	// Null in general. Otherwise, overrides MajorVersion,MinorVersion,BuildNumber,PlatformId and OperatingSystem PEB fields. 
	Win32VersionValue,
	// In general equal to sections + headers - alignement.
	SizeOfImage,
	// Completly unreliable, could be set to any value.
	SizeOfHeaders,
	// Required for driver only.
	CheckSum,
	// GUI subsystem. For more info, see the GUISubsystem.h
	Subsystem,
	// Dll characteristics. For more info, see the DllCharacteristics.h
	DllCharacteristics,
	// General values are given in the documentation. Do exactly what they say.
	SizeOfStackReserve,
	SizeOfStackCommit,
	SizeOfHeapReserve,
	SizeOfHeapCommit,
	// Obsolete, should always be 0.
	LoaderFlags,
	// Maximum value is 16. Basically the number od data directories.
	NumberOfRvaAndSizes,

	// Required for the IteratableEnum trick to work
	end,
	begin = Magic
};

enum class SectionHeaderField : flag_type
{
	Name,
	VirtualSize,
	VirtualAddress,
	SizeOfRawData,
	PointerToRawData,
	PointerToRelocations,
	PointerToLinenumbers,
	NumberOfRelocations,
	NumberOfLinenumbers,
	Characteristics,

	end,
	begin = Name
};

template<class Enum>
struct HeaderFieldInfoHolder
{
	static constexpr size_type size = IteratableEnum<Enum>::toUnderlying(IteratableEnum<Enum>::end());
	std::array<HeaderField, size> infos;

	constexpr HeaderField operator[](size_t index)
	{
		return infos[index];
	}

	constexpr HeaderField get(Enum field) const noexcept
	{
		return infos[std::underlying_type_t<Enum>(field)];
	}

	const size_type getHeaderSize() const noexcept
	{
		size_type sum = 0;
		sum += infos.back().offset;
		sum += infos.back().size;
		return sum;
	}
};

template<class Enum,
		 size_t ... sizes >
class HeaderFieldInfos
{
	static_assert(detail::IsStronglyTypedEnum<Enum>::value,
				 "The template parameter should be a strongly typed enum.");
	static_assert(IteratableEnum<Enum>::toUnderlying(IteratableEnum<Enum>::end()) == sizeof...(sizes),
				 "Error : more sizes than fields");
	using HolderType = HeaderFieldInfoHolder<Enum>;
	HolderType holder_;
	//constexpr std::array<size_t, sizeof...(sizes)> sizeArray{ sizes... };

public:
	HeaderFieldInfos() : holder_(init<sizes...>(std::make_index_sequence<sizeof...(sizes)>{})) {}

	template<size_t ... sizeSeq, size_t ... seq>
	HolderType init(std::index_sequence<seq...>) noexcept
	{
		return {init(seq, sizeSeq)...};
	}

	HeaderField init(size_t index, size_t size) noexcept
	{
		if (index == 0)
		{
			return { 0, size };
		}
		else
		{
			return { holder_[index - 1].offset + holder_[index - 1].size, size };
		}
	}

	constexpr size_t toUnderlying(Enum field) const noexcept
	{
		std::underlying_type_t<Enum>(field);
	}

	HolderType getHolderCpy() const noexcept
	{
		return holder_;
	}

	const HolderType* getHolderPtr() const noexcept
	{
		return &holder_;
	}

	const size_type getHeaderSize()
	{
		return holder_.getHeaderSize();
	}
};

// The COFFHeaderLayout must stay the same for 32 bit and 64 bit.
// If this header changes one day, and is different for different CPU sizes,
// be aware that this parser will not be valid anymore (not without a bit of work)
struct COFFHeaderLayout
{
	// TODO : Add OS/2 signatures ?
	static const uint16_t DOSSignature = IMAGE_DOS_SIGNATURE;
	static const uint32_t  PESignature = IMAGE_NT_SIGNATURE;

	static const HeaderField lfanew;
	static const std::array<flag_type, 3> magicNumberPossibleValues;

	// Starting from the machine signature or beggining of file for pure COFF files
	using InfoType =
		HeaderFieldInfos< COFFHeaderField,
		2, // Machine
		2, // NumberOfSections
		4, // TimeDateStamp
		4, // PointerToSymbolTable
		4, // NumberOfSymbolTable
		2, // SizeOfOptionalHeader
		2  // Characteristics
	>;

	static const InfoType infos;
};
		 

template<>
struct OptionalHeaderLayout<CPUSize::CPU32>
{
	using InfoType =
		HeaderFieldInfos< OptionalHeaderField,
		2, // Magic
		1, // MajorLinkerVersion
		1, // MinorLinkerVersion
		4, // SizeOfCode
		4, // SizeOfInitializedData
		4, // SizeOfUninitializedData
		4, // AddressOfEntryPoint
		4, // BaseOfCode
		4, // BaseOfData
		4, // ImageBase
		4, // SectionAlignment
		4, // FileAlignment
		2, // MajorOperatingSystemVersion
		2, // MinorOperatingSystemVersion
		2, // MajorImageVersion
		2, // MinorImageVersion
		2, // MajorSubsystemVersion
		2, // MinorSubsystemVersion
		4, // Win32VersionValue
		4, // SizeOfImage
		4, // SizeOfHeaders
		4, // CheckSum
		2, // Subsystem
		2, // DllCharacteristics
		4, // SizeOfStackReserve
		4, // SizeOfStackCommit
		4, // SizeOfHeapReserve
		4, // SizeOfHeapCommit
		4, // LoaderFlags
		4  // NumberOfRvaAndSizes
	>;
	static const InfoType infos;
};

template<>
struct OptionalHeaderLayout<CPUSize::CPU64>
{
	using InfoType =
		HeaderFieldInfos< OptionalHeaderField,
		2, // Magic
		1, // MajorLinkerVersion
		1, // MinorLinkerVersion
		4, // SizeOfCode
		4, // SizeOfInitializedData
		4, // SizeOfUniinitializedData
		4, // AddressOfEntryPoint
		4, // BaseOfCode
		0, // BaseOfData, not present in 64bit
		8, // ImageBase
		4, // SectionAlignment
		4, // FileAlignment
		2, // MajorOperatingSystemVersion
		2, // MinorOperatingSystemVersion
		2, // MajorImageVersion
		2, // MinorImageVersion
		2, // MajorSubsystemVersion
		2, // MinorSubsystemVersion
		4, // Win32VersionValue
		4, // SizeOfImage
		4, // SizeOfHeaders
		4, // CheckSum
		2, // Subsystem
		2, // DllCharacteristics
		8, // SizeOfStackReserve
		8, // SizeOfStackCommit
		8, // SizeOfHeapReserve
		8, // SizeOfHeapCommit
		4, // LoaderFlags
		4  // NumberOfRvaAndSizes
	>;
	static const InfoType infos;
};

struct SectionHeaderLayout
{
	using InfoType =
		HeaderFieldInfos< SectionHeaderField,
		8, // Name
		4, // VirtualSize
		4, // VirtualAddress
		4, // SizeOfRawData
		4, // PointerToRawData
		4, // PointerToRelocations
		4, // PointerToLinenumbers
		2, // NumberOfRelocations
		2, // NumberOfLinenumbers
		4  // Characteristics
	>;
	static const InfoType infos;
};
