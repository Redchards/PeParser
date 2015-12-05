#include <stdafx.h>

#include <FileStream.h>

std::unordered_map<std::string, typename FileStreamSelector<StreamGoal::read>::type> FileStreamSelector<StreamGoal::read>::streamMap_{};
std::unordered_map<std::string, typename FileStreamSelector<StreamGoal::write>::type> FileStreamSelector<StreamGoal::write>::streamMap_{};