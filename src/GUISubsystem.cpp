#include <stdafx.h>

#include <GUISubsystem.h>

std::map<GUISubsystem, std::string> GUISubsystemNameMap
{
	{ GUISubsystem::unknown, "Unknown subsystem" },
	{ GUISubsystem::native, "No subsystem" },
	{ GUISubsystem::gui, "Windows GUI" },
	{ GUISubsystem::cui, "Windows CUI" },
	{ GUISubsystem::os2Cui, "OS/2 CUI" },
	{ GUISubsystem::posixCui, "Posix CUI" },
	{ GUISubsystem::nativeWindows, "Windows driver" },
	{ GUISubsystem::winCEGui, "WinCE GUI" },
	{ GUISubsystem::efi, "EFI" },
	{ GUISubsystem::efiBootDriver, "EFI boot driver" },
	{ GUISubsystem::efiRuntimeDriver, "EFI runtime driver" },
	{ GUISubsystem::efiRom, "EFI ROM" },
	{ GUISubsystem::xbox, "XBox" },
	{ GUISubsystem::windowsBoot, "Windows boot driver" }
};