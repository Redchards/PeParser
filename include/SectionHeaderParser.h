#pragma once

#include <BasicParser.h>
#include <IteratableEnum.h>
#include <PEHeaderParser.h>

// TODO Should definitly find a better way to construct layout.
// NOTE : Could optimize this with a vector of section position, but will not do until it becomes a necessity (bad performances).
// But once again, we will likely buffer informations, this is not meant to be realtime or something like this.

class __declspec(dllexport) SectionHeaderParser : public BasicParser
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

	size_type getNumberOfSections() const noexcept;

	void goToSection(size_type index);

private:
	void init(PEHeaderParser& parser);
	void ensureSectionExists(size_type index);

	std::streampos sectionBegin_;
	size_type numberOfSections_;
	std::vector<uint8_t> rawBuffer_;
	size_type currentSectionIndex_;

	using IteratableEnumType = IteratableEnum< SectionHeaderField >;
	using LayoutType = HeaderFieldInfoHolder< SectionHeaderField, IteratableEnumType::toUnderlying(IteratableEnumType::end()) >;
	static const LayoutType* layout_;
};