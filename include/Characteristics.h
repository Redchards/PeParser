#pragma once

#include <Configuration.h>
#include <ConstString.h>

#include <map>

// As for MachineID, we are creating a strongly typed enum to ensure we are dealing with the right data
// Values and meaning found here :
// https://msdn.microsoft.com/fr-fr/library/windows/desktop/ms680313%28v=vs.85%29.aspx

enum class CharacteristicFlag : flag_type
{
	noReloc			  = IMAGE_FILE_RELOCS_STRIPPED,
	executable		  = IMAGE_FILE_EXECUTABLE_IMAGE,
	noLineNumber	  = IMAGE_FILE_LINE_NUMS_STRIPPED,
	noLocalSymbols	  = IMAGE_FILE_LOCAL_SYMS_STRIPPED,
	largeAddress	  = IMAGE_FILE_LARGE_ADDRESS_AWARE,
	is32bit			  = IMAGE_FILE_32BIT_MACHINE,
	noDebug			  = IMAGE_FILE_DEBUG_STRIPPED,
	onRemovableDevice = IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP,
	onNetwork		  = IMAGE_FILE_NET_RUN_FROM_SWAP,
	system			  = IMAGE_FILE_SYSTEM, // Seems to be completly obsolete today.
	dll				  = IMAGE_FILE_DLL,
	unicoreOnly		  = IMAGE_FILE_UP_SYSTEM_ONLY,

	// Should never happen, marked as obsolete.
	// I will however keep them just in case.
	// I'll however remove them if needed (very unlikely we will parse old PE files)
	bigEndian		  = IMAGE_FILE_BYTES_REVERSED_HI,
	littleEndian	  = IMAGE_FILE_BYTES_REVERSED_LO,
	aggressiveStrip   = IMAGE_FILE_AGGRESIVE_WS_TRIM // The mistype is present in the winnt.h macro
};

extern const std::map<CharacteristicFlag, ConstString> characteristicsNameMap;