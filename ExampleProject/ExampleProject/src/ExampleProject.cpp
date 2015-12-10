// OBfister.cpp : définit le point d'entrée pour l'application console.
//

#include <stdafx.h>

#include <PeHeaderParser.h>
#include <Log.h>

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 2)
		{
			std::cout << "No file input. Exiting now ..." << std::endl;
			system("pause");
			return 1;
		}

		PEHeaderParser a(argv[1]);
		std::cout << "The number of section is : " << a.getNumberOfSections() << std::endl;
		std::cout << "The machine name is : " << a.getMachineName() << std::endl;
		std::cout << "Creation date of the file is : " << a.getFileCreationDate() << std::endl;
		std::cout << "This file is a system file ? : " << a.isSystemFile() << std::endl;
		std::cout << "This file is a dll ? : " << a.isDll() << std::endl;
		std::cout << "This file is 64 bit ? : " << a.is64bit() << std::endl;
		std::cout << "This file is 32 bit ? : " << a.is32bit() << std::endl;
		std::cout << "This file is large address aware ? : " << a.isLargeAddressAware() << std::endl;
		std::cout << "This file has debug line number ? : " << a.hasLineNumber() << std::endl;
		std::cout << "Subsystem name is : " << a.getSubsystemName() << std::endl;
		std::cout << std::endl;
		std::cout << "Listing all file characteristics : " << std::endl << std::endl;
		for (auto i : a.getCharacteristicsAsString())
		{
			std::cout << "-" << i << std::endl;
		}
		std::cout << std::endl;
		if (a.isDll())
		{
			std::cout << "The dll characteristics are : " << std::endl;
			for (auto i : a.getDllCharacteristics())
			{
				std::cout << "-" << a.dllCharacteristicToString(i) << std::endl;
			}
		}
		system("pause");
	}
	catch (std::ios_base::failure e)
	{
		LOG(LogLevel::error, e.what() << '\n');
		system("pause");
	}
	catch (std::out_of_range e)
	{
		LOG(LogLevel::error, e.what() << "\n");
		LOG(LogLevel::warning, "The accessed field is non valid\n");
		system("pause");
	}
	catch (...)
	{
		LOG(LogLevel::fatal, "Unhandled exception ...");
	}
	return 0;
}

