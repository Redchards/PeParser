#pragma once

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