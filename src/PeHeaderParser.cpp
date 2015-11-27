// PeParser.cpp : définit les fonctions exportées pour l'application DLL.
//

#include <stdafx.h>

#include <PeHeaderParser.h>

//DataDirectoryTable PEHeaderParser::table_ = {};

PEHeaderParser::PEHeaderParser(PEHeaderParser&& other) : COFFHeaderParser(std::move(other)),
														 layout_(std::move(other.layout_)),
														 coffHeaderSize_(COFFHeaderParser::layout_->getHeaderSize())
{
	other.layout_ = nullptr;
}

PEHeaderParser::PEHeaderParser(std::string const& filename) : COFFHeaderParser(filename),
															  coffHeaderSize_(COFFHeaderParser::layout_->getHeaderSize())
{
	//std::cout << coffHeaderSize_ << std::endl;
	// Equivalent to do a std::bind(&PEHeaderParser::init, this); but more readable.
	events_.push_back([this] { this->init(); });
	//__hook(&EventSource::fileLoadEvent, &event_, &PEHeaderParser::init);
	init();
}

PEHeaderParser::~PEHeaderParser()
{
	//std::cout << "Unhoook pe" << std::endl;
	events_.pop_back();
	//__unhook(&EventSource::fileLoadEvent, &event_, &PEHeaderParser::init);
}

size_type PEHeaderParser::retrieveFieldValue(OptionalHeaderField field)
{
	if (lfanewValue_ == 0)
	{
		throw std::ios_base::failure("Error : attempt to read a field without the header parser fully constructed");
	}
	HeaderField tmp = layout_->get(field);
	//std::cout << "PE :" << tmp.offset << " : " << tmp.size << std::endl;
	return reader_.retrieveValue(getHeaderStart() + tmp.offset, tmp.size);
}

bool PEHeaderParser::hasValidMagicNumber()
{
	size_type magic = retrieveFieldValue(OptionalHeaderField::Magic);
	for (auto i : COFFHeaderLayout::magicNumberPossibleValues)
	{
		if (magic == i)
		{
			return true;
		}
	}
	return false;
}

size_t PEHeaderParser::getHeaderStart() const noexcept
{
	return COFFHeaderParser::getHeaderEnd();
}

size_type PEHeaderParser::getHeaderEnd() const noexcept
{
	// TODO : add checks to see if it's coherent (depending on the layout value etc etc)
	return getHeaderStart() + layout_->getHeaderSize();
}

size_type PEHeaderParser::getDataDirectoryEnd() const noexcept
{
	return getHeaderEnd() + dataDirectoryTable_.size()*sizeof(DataDirectoryEntry);
}

std::string PEHeaderParser::getVersionAsString(size_type major, size_type minor)
{
	std::ostringstream oss;
	oss << major << "." << minor;
	return oss.str();
}

size_type PEHeaderParser::getMajorOSVersion()
{
	return retrieveFieldValue(OptionalHeaderField::MajorOperatingSystemVersion);
}

size_type PEHeaderParser::getMinorOSVersion()
{
	return retrieveFieldValue(OptionalHeaderField::MinorOperatingSystemVersion);
}

size_type PEHeaderParser::getMajorImageVersion()
{
	return retrieveFieldValue(OptionalHeaderField::MajorImageVersion);
}

size_type PEHeaderParser::getMinorImageVersion()
{
	return retrieveFieldValue(OptionalHeaderField::MinorImageVersion);
}

size_type PEHeaderParser::getMajorSubsystemVersion()
{
	return retrieveFieldValue(OptionalHeaderField::MajorSubsystemVersion);
}

size_type PEHeaderParser::getMinorSubsystemVersion()
{
	return retrieveFieldValue(OptionalHeaderField::MinorSubsystemVersion);
}

GUISubsystem PEHeaderParser::getSubsystem()
{
	return static_cast<GUISubsystem>(retrieveFieldValue(OptionalHeaderField::Subsystem));
}

std::string PEHeaderParser::getSubsystemName()
{
	return GUISubsystemNameMap[getSubsystem()];
}

std::vector<DllCharacteristicFlag> PEHeaderParser::getDllCharacteristics() const
{
	std::vector<DllCharacteristicFlag> flagVector;
	for (auto i : DllCharacteristicsNameMap)
	{
		if (hasDllCharacteristicFlag(i.first))
		{
			flagVector.push_back(i.first);
		}
	}
	return flagVector;
}

std::string PEHeaderParser::getDllCharacteristicAsString(DllCharacteristicFlag flag) const
{
	return DllCharacteristicsNameMap[flag];
}

bool PEHeaderParser::hasDllCharacteristicFlag(DllCharacteristicFlag flag) const
{
	return hasFlag(dllFlags_, flag);
}

size_type PEHeaderParser::numberOfDataDirectories()
{
	return retrieveFieldValue(OptionalHeaderField::NumberOfRvaAndSizes);
}

/*size_type PEHeaderParser::retrieveFieldValue(HeaderField field)
{
	if (getLfanew() == 0)
	{
		throw std::ios_base::failure("Error : attempt to read a field without the header parser fully constructed");
	}
	return reader_.retrieveValue(getHeaderStart() + field.offset, field.size);
}*/

DataDirectoryEntry PEHeaderParser::retrieveDataDirectoryInfos(DataDirectory entry)
{
	// TODO : Maybe replace getHeaderEnd() by a const value.
	const size_t tableOffset = getHeaderEnd() + (dataDirectoryTable_.findPosition(entry)*sizeof(DataDirectoryEntry));
	return{
		static_cast<uint32_t>(reader_.retrieveValue(tableOffset, sizeof(DataDirectoryEntry::RVA))),
		static_cast<uint32_t>(reader_.retrieveValue(tableOffset + sizeof(DataDirectoryEntry::RVA), sizeof(DataDirectoryEntry::size)))
	};
	//reader_.retrieveValue(getHeaderEnd(), sizeof(DataDirectoryEntry));
}

void PEHeaderParser::init()
{
	//std::cout << "PE Init" << std::endl;
	// TODO : Adapt this to the file type;

	//lfanewValue_ = reader_.retrieveValue(layout_.lfanew.offset, layout_.lfanew.size);
	//std::cout << "lfanew" << lfanewValue_ << std::endl;
	if (getLfanew() == 0)
	{
		throw std::ios_base::failure("Error : bad lfanew value detected. If it is a PE file, it may be corrupted.");
	}

	if (is32bit())
	{
		layout_ = OptionalHeaderLayout<CPUSize::CPU32>::infos.getHolderPtr();
	}
	else
	{
		layout_ = OptionalHeaderLayout<CPUSize::CPU64>::infos.getHolderPtr();
	}
	bool objectFile = false;
	/*if (!hasDOSSignature())
	{
		throw std::ios_base::failure("Error : this file is either not a PE file, or is corrupted !");
	}*/
	if (!hasPESignature())
	{
		throw std::ios_base::failure("Error : No PE signature detected. If it is a PE file, it may be corrupted.");
	}
	else if (!hasValidMagicNumber())
	{
		throw std::ios_base::failure("Error : Bad magic number value detected. If it is a PE file, it may be corrupted.");
	}

	dllFlags_ = retrieveFieldValue(OptionalHeaderField::DllCharacteristics);
	//fileFlags_ = retrieveFieldValue(layout_.characteristics);
}


