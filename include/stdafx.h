// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

#include <targetver.h>

#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
// Fichiers d'en-tête Windows :
#include <windows.h>

#include <BasicParser.h>
#include <Characteristics.h>
//#include <CharToInteger.h>
#include <COFFHeaderParser.h>
#include <DataDirectoryTable.h>
#include <DllCharacteristics.h>
#include <EpochConverter.h>
#include <FileValueReader.h>
#include <GUISubsystem.h>
#include <HeaderLayout.h>
#include <MachineID.h>
#include <PeDataDirectory.h>
#include <PEFileParser.h>
#include <HeaderField.h>
#include <PeHeaderParser.h>
#include <SectionHeaderParser.h>
#include <SectionParser.h>
