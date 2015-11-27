#pragma once


// No forward here because constexpr disallow move semantic, at least for now
// With C++14, we'll have a constexpr implementation of std::move (and thus std::forward)
// More infos here : http://en.cppreference.com/w/cpp/utility/move
// Anyway, that will likely not influence performances in our case, because we will do the majority
// of computation at compile time.

template<typename T1,
	     typename T2>
struct ConstexprPair
{
	explicit constexpr ConstexprPair() : first(), second()
	{};

	template<typename U1,
		     typename U2>
	explicit constexpr ConstexprPair(U1&& first_, U2&& second_) : first(first_), second(second_)
	{};

	template<typename U1>
	explicit constexpr ConstexprPair(U1&& first_) : first(first_), second()
	{};

	explicit constexpr ConstexprPair(ConstexprPair const& other) : ConstexprPair(other.first, other.second)
	{};

	explicit constexpr ConstexprPair(ConstexprPair&& other) : ConstexprPair(std::move(other.first), std::move(other.second))
	{};

	template<typename U1,
			 typename U2>
	constexpr ConstexprPair& operator=(ConstexprPair<U1, U2>&& other)
	{
		first = std::forward<U1>(other.first);
		second = std::forward<U2>(other.second);
		return *this;
	};

	T1 first;
	T2 second;
};