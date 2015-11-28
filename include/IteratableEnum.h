#pragma once

#include <exception>
#include <type_traits>
#include <string>
#include <sstream>
#include <utility>

class IndexOutOfRange
{
public:
	IndexOutOfRange(const char* msg, size_t index) : msg_(msg), index_(index) {}

	std::string what() const noexcept
	{
		std::ostringstream sstr(std::ios_base::ate);
		sstr << msg_ << ". At index : " << index_;
		return sstr.str();
	}

private:
	const char* msg_;
	size_t index_;
};


namespace detail
{
	template<class T>
	class HasEnumBeginEnd
	{
	private:
		template<typename U>
		static std::integral_constant<bool, (U::begin, U::end, true)> check(int);
		template<typename U>
		static std::false_type										  check(...);

	public:
		static constexpr bool value = decltype(check<T>(0))::value;
	};

	template<class Enum>
	struct IsStronglyTypedEnum
	{
		static constexpr bool value = (std::is_enum<Enum>::value && !std::is_convertible<Enum, int>::value);
	};
}

template<class Enum>
class IteratableEnum
{
	static_assert(detail::IsStronglyTypedEnum<Enum>::value,
		"This type only support strongly typed enum, whereas the passed parameted wasn't one");
	//static_assert(detail::HasEnumBeginEnd<Enum>::value,
	//	"Can't apply this type to an enum without 'begin' and 'end' field !");

public:
	using iterator = Enum;

public:
	constexpr IteratableEnum() noexcept = default;

	Enum constexpr operator*() const { return begin(); }

	static constexpr typename std::underlying_type<Enum>::type toUnderlying(Enum param)
	{
		return typename std::underlying_type<Enum>::type(param);
	}

	static constexpr Enum begin() noexcept { return Enum::begin; };
	static constexpr Enum end() noexcept { return Enum::end; };

	static Enum at(size_t index)
	{
		if (index > toUnderlying(Enum::end))
		{
			throw IndexOutOfRange("Index out of enumeration range", index);
		}
		else
		{
			return begin() + index;
		}
	}

	size_t findPosition(Enum enumerator) const
	{
		size_t count = 0;
		for (auto elem : *this)
		{
			if (elem == enumerator)
			{
				return count;
			}
			++count;
		}
		throw IndexOutOfRange("Element not in enumeration.\
				       Please do not typecast from integer when using this function !!!", count);
	}

	size_t size() const noexcept
	{
		size_t tmp = 0;
		for (auto elem : *this)
		{
			++tmp;
		}
		return tmp;
	}
};

template<class Enum>
typename std::enable_if<detail::IsStronglyTypedEnum<Enum>::value, Enum>::type
operator*(Enum falsePointer) { return falsePointer; }

template<class Enum>
typename std::enable_if<detail::IsStronglyTypedEnum<Enum>::value, Enum>::type
operator++(Enum& lhs) 
{ 
	Enum tmp = lhs;
	lhs = static_cast<Enum>(IteratableEnum<Enum>::toUnderlying(lhs) + 1);
	return tmp;
}

template<class Enum>
typename std::enable_if<detail::IsStronglyTypedEnum<Enum>::value, Enum>::type
operator++(Enum& rhs, int) { return rhs = static_cast<Enum>(IteratableEnum<Enum>::toUnderlying(rhs) + 1); }

template<class Enum>
typename std::enable_if<detail::IsStronglyTypedEnum<Enum>::value, Enum>::type
operator--(Enum& lhs) 
{ 
	Enum tmp = lhs;
	lhs = static_cast<Enum>(IteratableEnum<Enum>::toUnderlying(lhs) - 1);
	return tmp; 
}

template<class Enum>
typename std::enable_if<detail::IsStronglyTypedEnum<Enum>::value, Enum>::type
operator--(Enum& rhs, int) { return rhs = static_cast<Enum>(IteratableEnum<Enum>::toUnderlying(rhs) - 1); }

template<class Enum>
typename std::enable_if<detail::IsStronglyTypedEnum<Enum>::value, Enum>::type
operator+(Enum rhs, size_t offset) { return static_cast<Enum>(IteratableEnum<Enum>::toUnderlying(rhs) + offset); }

template<class Enum>
typename std::enable_if<detail::IsStronglyTypedEnum<Enum>::value, Enum>::type
operator-(Enum rhs, size_t offset) { return static_cast<Enum>(IteratableEnum<Enum>::toUnderlying(rhs) - offset); }