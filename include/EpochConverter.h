#pragma once

#include <ctime>
#include <string>

// TODO : Modify date depending on meridian (this only give GMT date, great meridian time)
class EpochConverter
{
public:
	explicit EpochConverter(time_t epoch);
	explicit EpochConverter(time_t epoch, const std::string& formatString);

	std::string ToDate();

private:
	time_t epoch_;
	std::string formatString_;
};