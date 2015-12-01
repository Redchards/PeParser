#pragma once

#include <BasicParser.h>
#include <IteratableEnum.h>
#include <PEHeaderParser.h>
#include <SectionCharacteristics.h>

// TODO Should definitly find a better way to construct layout.
// NOTE : Could optimize this with a vector of section position, but will not do until it becomes a necessity (bad performances).
// But once again, we will likely buffer informations, this is not meant to be realtime or something like this.

class PE_API SectionHeaderParser : public BasicParser
{
public:
	SectionHeaderParser(const std::string& filename);
	SectionHeaderParser(PEHeaderParser& parser);

	std::streampos getSectionsBegin() const noexcept;

	// No checks needed, as we know, at this point, that at least one PEHeaderParser with this file constructed correctly.
	// So all the checks was already done.
	size_type retrieveFieldValue(size_type index, SectionHeaderField field);
	size_type getSectionSize(size_type index);
	size_type getSectionSize(const std::string& name);
	std::string getSectionName(size_type index);
	std::vector<SectionCharacteristicFlag> getCharacteristics(size_type index);
	std::vector<ConstString> getCharacteristicsAsString(size_type index);
	ConstString characteristicToString(SectionCharacteristicFlag flag) const;
	bool hasCharacteristic(size_type index, SectionCharacteristicFlag flag);

	size_type getNumberOfSections() const noexcept;

	void goToSection(size_type index);

private:
	void init(COFFHeaderParser& parser);
	void init(PEHeaderParser& parser);

	// Too costly without buffering right now :/
	void initCharacteristics();
	void ensureSectionExists(size_type index);

	std::streampos sectionBegin_;
	size_type numberOfSections_;
	//std::vector<uint8_t> rawBuffer_;
	size_type currentSectionIndex_;
	std::vector<size_type> characteristicFlags_;

	using LayoutType = HeaderFieldInfoHolder<SectionHeaderField>;
	static const LayoutType* layout_;
};