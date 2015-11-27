#include <stdafx.h>

#include <DllCharacteristics.h>

std::map<DllCharacteristicFlag, std::string> DllCharacteristicsNameMap{
	{DllCharacteristicFlag::processInit, "Process initialisation routine"},
	{DllCharacteristicFlag::processTerm, "Process termination routine"},
	{DllCharacteristicFlag::threadInit, "Thread initialisation routine"},
	{DllCharacteristicFlag::threadTerm, "Thread termination routine"},
	{DllCharacteristicFlag::dynamicBase, "Relocation at load time"},
	{DllCharacteristicFlag::forceIntegrity, "Code integrity checked forced"},
	{DllCharacteristicFlag::nxCompatible, "Data execution prevention compatible"},
	{DllCharacteristicFlag::noIsolation, "No isolation"},
	{DllCharacteristicFlag::noSeh, "No exception handling"},
	{DllCharacteristicFlag::noBind, "No binding"},
	{DllCharacteristicFlag::appContainer, "Execution inside AppContainer"},
	{DllCharacteristicFlag::wdmDriver, "WDM driver"},
	{DllCharacteristicFlag::guardCF, "Controle flow guard"},
	{DllCharacteristicFlag::serverAware, "Server aware"}
};