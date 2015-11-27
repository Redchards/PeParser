#include <stdafx.h>

#include <COFFHeaderParser.h>

const COFFHeaderParser::LayoutType* COFFHeaderParser::layout_{ COFFHeaderLayout::infos.getHolderPtr() };

COFFHeaderParser::COFFHeaderParser(COFFHeaderParser&& other) : BasicParser(std::move(other)),
															   fileFlags_(std::move(other.fileFlags_)),
															   lfanewValue_(std::move(other.lfanewValue_)),
															   isObjectFile_(std::move(other.isObjectFile_))
{}

COFFHeaderParser::COFFHeaderParser(std::string const& filename) : BasicParser(filename)
{
	// Equivalent to do a std::bind(&COFFHeaderParser::init, this); but more readable.
	events_.push_back([this] { this->init(); });
	init();
}

COFFHeaderParser::~COFFHeaderParser()
{
	events_.pop_back();
}

bool COFFHeaderParser::hasDOSSignature()
{
	return (reader_.retrieveValue(0, sizeof(COFFHeaderLayout::DOSSignature)) == COFFHeaderLayout::DOSSignature);
}

size_type COFFHeaderParser::getHeaderStart() const noexcept
{
	return (getLfanew() + sizeof(COFFHeaderLayout::PESignature)*(!isObjectFile_));
}

size_type COFFHeaderParser::getHeaderEnd() const noexcept
{
	return (getHeaderStart() + layout_->getHeaderSize());
}

bool COFFHeaderParser::hasPESignature()
{
	return (reader_.retrieveValue(lfanewValue_, sizeof(COFFHeaderLayout::PESignature)) == COFFHeaderLayout::PESignature);
}

MachineID const COFFHeaderParser::getMachineID()
{
	return static_cast<MachineID>(retrieveFieldValue(COFFHeaderField::Machine));
}

size_type const COFFHeaderParser::getNumberOfSections()
{
	return retrieveFieldValue(COFFHeaderField::NumberOfSections);
}

size_type COFFHeaderParser::getLfanew() const noexcept
{
	return lfanewValue_;
}

// Go to PeHeaderParser

std::string const COFFHeaderParser::getFileCreationDate()
{
	EpochConverter conv(retrieveFieldValue(COFFHeaderField::TimeDateStamp), "%Y-%m-%d %H:%M:%S");
	return conv.ToDate();
}

std::string const COFFHeaderParser::getMachineName()
{
	return machineNameMap.at(getMachineID());
}

std::vector<CharacteristicFlag> COFFHeaderParser::getCharacteristics() const
{
	std::vector<CharacteristicFlag> flagVector;
	for (auto i : CharacteristicsNameMap)
	{
		if (hasCharacteristicFlag(i.first))
		{
			flagVector.push_back(i.first);
		}
	}
	return flagVector;
}

std::string COFFHeaderParser::getCharacteristicAsString(CharacteristicFlag flag) const
{
	return CharacteristicsNameMap[flag];
}

bool COFFHeaderParser::hasCharacteristicFlag(CharacteristicFlag flag) const
{
	return hasFlag(fileFlags_, flag);
	//return ((fileFlags_ & static_cast<size_type>(flag)) == static_cast<size_type>(flag));
}

bool COFFHeaderParser::is32bit()
{
	return hasCharacteristicFlag(CharacteristicFlag::is32bit);
}

bool COFFHeaderParser::is64bit()
{
	return !is32bit();
}

bool COFFHeaderParser::isSystemFile()
{
	return hasCharacteristicFlag(CharacteristicFlag::system);
}

bool COFFHeaderParser::isDll()
{
	return hasCharacteristicFlag(CharacteristicFlag::dll);
}

bool COFFHeaderParser::isExecutable()
{
	return (!hasCharacteristicFlag(CharacteristicFlag::dll) && hasCharacteristicFlag(CharacteristicFlag::executable));
}

bool COFFHeaderParser::isExecutableImage()
{
	return hasCharacteristicFlag(CharacteristicFlag::executable);
}

bool COFFHeaderParser::isLargeAddressAware()
{
	return hasCharacteristicFlag(CharacteristicFlag::largeAddress);
}

bool COFFHeaderParser::hasRelocations()
{
	return !hasCharacteristicFlag(CharacteristicFlag::noReloc);
}

bool COFFHeaderParser::hasLineNumber()
{
	return !hasCharacteristicFlag(CharacteristicFlag::noLineNumber);
}


bool COFFHeaderParser::hasDebugSymbols()
{
	return !hasCharacteristicFlag(CharacteristicFlag::noLocalSymbols);
}

size_type COFFHeaderParser::retrieveFieldValue(COFFHeaderField field)
{
	if (lfanewValue_ == 0)
	{
		throw std::ios_base::failure("Error : attempt to read a field without the header parser fully constructed, file had an erroneous lfanew field or is not a PE file.");
	}
	HeaderField tmp = layout_->get(field);
	//std::cout << "COFF : " << tmp.offset << " : " << tmp.size << std::endl;
	return reader_.retrieveValue(getHeaderStart() + tmp.offset, tmp.size);
}

void COFFHeaderParser::init()
{
	//std::cout << "COFF Init" << std::endl;
	if (hasDOSSignature())
	{
		lfanewValue_ = reader_.retrieveValue(COFFHeaderLayout::lfanew.offset, COFFHeaderLayout::lfanew.size);
		isObjectFile_ = false;
		fileFlags_ = retrieveFieldValue(COFFHeaderField::Characteristics);
	}
	else
	{
		lfanewValue_ = 0;
		isObjectFile_ = true;
		fileFlags_ = 0;
	}
	// TODO : Check if fileFlags_ is not 0
}