#include <stdafx.h>

#include <HeaderLayout.h>

// Obligated static initialization, until C++ got a more convenient way to do this ... (in class initialization !!!)

const HeaderField COFFHeaderLayout::lfanew{ 60, 4 };
const std::array<flag_type, 3> COFFHeaderLayout::magicNumberPossibleValues{ 0x010b, 0x020b, 0x0107 };
const COFFHeaderLayout::InfoType COFFHeaderLayout::infos{};
const OptionalHeaderLayout<CPUSize::CPU32>::InfoType OptionalHeaderLayout<CPUSize::CPU32>::infos{};
const OptionalHeaderLayout<CPUSize::CPU64>::InfoType OptionalHeaderLayout<CPUSize::CPU64>::infos{};
const SectionHeaderLayout::InfoType SectionHeaderLayout::infos{};