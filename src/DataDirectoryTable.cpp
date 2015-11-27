#include <stdafx.h>

#include <DataDirectoryTable.h>

/*inline DataDirectoryTable::DataDirectory operator++(DataDirectoryTable::DataDirectory const& lrhs)
{
	return static_cast<DataDirectoryTable::DataDirectory>(std::underlying_type<DataDirectoryTable::DataDirectory>::type(lrhs) + 1);
}

inline DataDirectoryTable::DataDirectory operator--(DataDirectoryTable::DataDirectory const& lrhs)
{
	return static_cast<DataDirectoryTable::DataDirectory>(std::underlying_type<DataDirectoryTable::DataDirectory>::type(lrhs) - 1);
}

inline DataDirectoryTable::DataDirectory operator*(DataDirectoryTable::DataDirectory falsePointer)
{
	return falsePointer;
}


inline DataDirectoryTable::DataDirectory DataDirectoryTable::operator*(DataDirectoryTable falsePointer) const
{
	return begin();
}

DataDirectoryTable::DataDirectory DataDirectoryTable::begin()
{
	return DataDirectory::begin_;
}

DataDirectoryTable::DataDirectory DataDirectoryTable::end()
{
	return DataDirectory::end_;
}

size_type DataDirectoryTable::findPosition(DataDirectoryTable::DataDirectory dir) const
{
	// Warning, we are changing the "current_" value
	size_type count = 0;
	for (auto i : *this)
	{
		if (i == dir)
		{
			return count;
		}
		++count;
	}
	throw std::out_of_range("The requested value do not exist in DataDirectoryTable. Please do not typecast from int when using this function !!!");
}

DataDirectoryTable::DataDirectory const DataDirectoryTable::at(size_type const position) const
{
	DataDirectory dir;
	for (size_type i = 0; i < position; ++dir);
	return dir;
}

size_type DataDirectoryTable::size() const
{
	size_type count = 0;
	for (auto i : *this)
	{
		++count;
	}
	return count;
}*/
