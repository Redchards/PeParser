#include <stdafx.h>

#include <EpochConverter.h>

EpochConverter::EpochConverter(time_t epoch) : EpochConverter(epoch, "%a %Y-%m-%d %H:%M:%S %Z")
{}

EpochConverter::EpochConverter(time_t epoch, const std::string& formatString) : epoch_(epoch), 
																		        formatString_(formatString)
{}

std::string EpochConverter::ToDate()
{
	tm timeStamp {};
	char res[64];

	localtime_s(&timeStamp, &epoch_);
	strftime(res, sizeof(res), &formatString_[0],  &timeStamp);
	return{ res };
}