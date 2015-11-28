#include<stdafx.h>

#include <Characteristics.h>

const std::map<CharacteristicFlag, ConstString> characteristicsNameMap
{
	{CharacteristicFlag::noReloc, "No Relocations"},
	{CharacteristicFlag::executable, "Executable image file"},
	{CharacteristicFlag::noLineNumber, "No line number"},
	{CharacteristicFlag::noLocalSymbols, "No debug symbols"},
	{CharacteristicFlag::largeAddress, "Large address aware"},
	{CharacteristicFlag::is32bit, "32 bit"},
	{CharacteristicFlag::noDebug, "Release version"},
	{CharacteristicFlag::onRemovableDevice, "Run on removable device"},
	{CharacteristicFlag::onNetwork, "Run on network"},
	{CharacteristicFlag::system, "System file"},
	{CharacteristicFlag::dll, "DLL"},
	{CharacteristicFlag::unicoreOnly, "Run only on unicore CPU"},
	{CharacteristicFlag::bigEndian, "Big endian"},
	{CharacteristicFlag::littleEndian, "Little endian"},
	{CharacteristicFlag::aggressiveStrip, "Aggressive stripping"}
};