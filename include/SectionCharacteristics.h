#pragma once

#include <Configuration.h>
#include <ConstString.h>

#include <map>

// Values taken from the winnth.h header
// TODO : import ConstString to use it instead of std::string in the name map
enum class SectionCharacteristicFlag : uint32_t
{
	noPad			    = IMAGE_SCN_TYPE_NO_PAD, // Reserved, but can technically appear
	code			    = IMAGE_SCN_CNT_CODE,
	initializedData	    = IMAGE_SCN_CNT_INITIALIZED_DATA,
	uninitializedData   = IMAGE_SCN_CNT_UNINITIALIZED_DATA,
	linkOther		    = IMAGE_SCN_LNK_OTHER, // Reserved, but can technically appear
	linkInfo		    = IMAGE_SCN_LNK_INFO,
	linkRemove		    = IMAGE_SCN_LNK_REMOVE,
	linkComdat		    = IMAGE_SCN_LNK_COMDAT,
	noExceptionDefer    = IMAGE_SCN_NO_DEFER_SPEC_EXC,
	gpRelative		    = IMAGE_SCN_GPREL,
	farData			    = IMAGE_SCN_MEM_FARDATA,
	purgeable		    = IMAGE_SCN_MEM_PURGEABLE,
	is16bit			    = IMAGE_SCN_MEM_16BIT,
	locked			    = IMAGE_SCN_MEM_LOCKED,
	preload			    = IMAGE_SCN_MEM_PRELOAD,
	align1byte		    = IMAGE_SCN_ALIGN_1BYTES,
	align2bytes         = IMAGE_SCN_ALIGN_2BYTES,
	align4bytes         = IMAGE_SCN_ALIGN_4BYTES,
	align8bytes         = IMAGE_SCN_ALIGN_8BYTES,
	align16bytes        = IMAGE_SCN_ALIGN_16BYTES,
	align32bytes        = IMAGE_SCN_ALIGN_32BYTES,
	align64bytes        = IMAGE_SCN_ALIGN_64BYTES,
	align128bytes       = IMAGE_SCN_ALIGN_128BYTES,
	align256bytes       = IMAGE_SCN_ALIGN_256BYTES,
	align512bytes       = IMAGE_SCN_ALIGN_512BYTES,
	align1024bytes      = IMAGE_SCN_ALIGN_1024BYTES,
	align2048bytes      = IMAGE_SCN_ALIGN_2048BYTES,
	align4096bytes      = IMAGE_SCN_ALIGN_4096BYTES,
	align8192bytes      = IMAGE_SCN_ALIGN_8192BYTES,
	alignMask           = IMAGE_SCN_ALIGN_MASK,
	extendedRelocations = IMAGE_SCN_LNK_NRELOC_OVFL,
	discardable			= IMAGE_SCN_MEM_DISCARDABLE,
	notCached			= IMAGE_SCN_MEM_NOT_CACHED,
	notPaged			= IMAGE_SCN_MEM_NOT_PAGED,
	shared				= IMAGE_SCN_MEM_SHARED,
	executable			= IMAGE_SCN_MEM_EXECUTE,
	readable			= IMAGE_SCN_MEM_READ,
	writable			= IMAGE_SCN_MEM_WRITE,
	tlsIndexScaled		= IMAGE_SCN_SCALE_INDEX
};

extern const std::map<SectionCharacteristicFlag, ConstString> sectionCharacteristicsNameMap;