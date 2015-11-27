#pragma once

#include <Configuration.h>

#include <map>

// All defined values are actually ordered (0 to 16), so all the "=" stuff
// is a bit unecessary. However, I prefer to keep it just in case a value changes
// in a future version.

enum class GUISubsystem : flag_type
{
	unknown = IMAGE_SUBSYSTEM_UNKNOWN,
	native = IMAGE_SUBSYSTEM_NATIVE,
	gui = IMAGE_SUBSYSTEM_WINDOWS_GUI,
	cui = IMAGE_SUBSYSTEM_WINDOWS_CUI,
	os2Cui = IMAGE_SUBSYSTEM_OS2_CUI,
	posixCui = IMAGE_SUBSYSTEM_POSIX_CUI,
	nativeWindows = IMAGE_SUBSYSTEM_NATIVE_WINDOWS,
	winCEGui = IMAGE_SUBSYSTEM_WINDOWS_CE_GUI,
	efi = IMAGE_SUBSYSTEM_EFI_APPLICATION,
	efiBootDriver = IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER,
	efiRuntimeDriver = IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER,
	efiRom = IMAGE_SUBSYSTEM_EFI_ROM,
	xbox = IMAGE_SUBSYSTEM_XBOX,
	windowsBoot = IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION
};

extern std::map<GUISubsystem, std::string> GUISubsystemNameMap;