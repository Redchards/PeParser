#pragma once

#include <Configuration.h>

#include <map>
#include <string>

#include <winnt.h>

// First 4 values are derived from :
// http://research.microsoft.com/en-us/um/redmond/projects/invisible/include/loaders/pe_image.h.htm
// The meaning of the others is described in the winnt.h header, or at the address :
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms680339%28v=vs.85%29.aspx

// Defining the first 4 values, as I didn't find anything about them in the winnt.h, but
// according to the documentation, they can be present.
#define IMAGE_LIBRARY_PROCESS_INIT 1
#define IMAGE_LIBRARY_PROCESS_TERM 2
#define IMAGE_LIBRARY_THREAD_INIT 4
#define IMAGE_LIBRARY_THREAD_TERM 8

enum class DllCharacteristicFlag : flag_type
{
	processInit    = IMAGE_LIBRARY_PROCESS_INIT,
	processTerm    = IMAGE_LIBRARY_PROCESS_TERM,
	threadInit	   = IMAGE_LIBRARY_THREAD_INIT,
	threadTerm	   = IMAGE_LIBRARY_THREAD_TERM,

	dynamicBase    = IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE,
	forceIntegrity = IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY,
	nxCompatible   = IMAGE_DLLCHARACTERISTICS_NX_COMPAT,
	noIsolation	   = IMAGE_DLLCHARACTERISTICS_NO_ISOLATION,
	noSeh		   = IMAGE_DLLCHARACTERISTICS_NO_SEH,
	noBind		   = IMAGE_DLLCHARACTERISTICS_NO_BIND,
	appContainer   = IMAGE_DLLCHARACTERISTICS_APPCONTAINER,
	wdmDriver      = IMAGE_DLLCHARACTERISTICS_WDM_DRIVER,
	guardCF		   = IMAGE_DLLCHARACTERISTICS_GUARD_CF,
	serverAware    = IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE
};

extern std::map<DllCharacteristicFlag, std::string> DllCharacteristicsNameMap;