#pragma once

#include <ConstString.h>

#include <vector>

namespace Detail
{

template<class Container>
class ContainerIteratorReverser
{
public:
	ContainerIteratorReverser(Container& container) : ref_(container)
	{}

	typename Container::reverse_iterator begin()
	{
		return ref_.rbegin();
	}

	typename Container::reverse_iterator end()
	{
		return ref_.rend();
	}

private:
	Container& ref_;
};

template<class Container>
ContainerIteratorReverser<Container> makeReverseIteratorRange(Container& container)
{
	return ContainerIteratorReverser<Container>(container);
}

template<typename TFlag>
bool hasFlag(size_type allFlags, TFlag flag)
{
	static_assert(std::is_integral<typename std::underlying_type<TFlag>::type>::value, "Flag type must be integral");
	return ((allFlags & static_cast<size_type>(flag)) == (static_cast<size_type>(flag)));
}

template<typename TFlag>
std::vector<TFlag> getAllFlags(size_type allFlags, std::map<TFlag, ConstString> nameMap)
{
	static_assert(std::is_integral<typename std::underlying_type<TFlag>::type>::value, "Flag type must be integral");
	std::vector<TFlag> flagVector;
	for (auto i : nameMap)
	{
		if (hasFlag(allFlags, i.first))
		{
			flagVector.push_back(i.first);
		}
	}
	return flagVector;
}

template<typename TFlag>
std::vector<ConstString> getAllFlagsAsString(size_type allFlags, std::map<TFlag, ConstString> nameMap)
{
	static_assert(std::is_integral<typename std::underlying_type<TFlag>::type>::value, "Flag type must be integral");
	std::vector<ConstString> flagVector;
	for (auto i : nameMap)
	{
		if (hasFlag(allFlags, i.first))
		{
			flagVector.push_back(i.second);
		}
	}
	return flagVector;
}

}