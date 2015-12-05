#pragma once

#include <stdexcept>

#include <Configuration.h>

// There's a warning about "multiple copy constructor specified".
// We can safely silence them out, as it should not be like this.
// The warning is due to the fact that a non-const iterator can be copied to a const iterator, but not
// the other way. So, a non-const iterator has a copy constructor AND a constructor accepting a const
// iterator argument. But for const iterator class, this constructor is disabled.
// It does work as expected in clang and gcc, but not un visual studio yet. At least, this is only
// a warning and not an error (constructor implementations are the same anyway).

#pragma warning(push)
#pragma warning(disable : 4521)
template<class ArrayClass>
class ArrayIteratorPolicy
{
protected:
	template<bool constFlag>
	class BaseIterator : public std::iterator<std::bidirectional_iterator_tag, ArrayClass>
	{
		friend BaseIterator<!constFlag>;
	protected:
		using indexType = typename std::conditional<constFlag, const size_t, size_t>::type;
	public:
		using value_type = typename ArrayClass::value_type;
		using reference = typename std::conditional<constFlag, const typename std::remove_cv<value_type>::type&, value_type&>::type;
		using pointer = typename std::conditional<constFlag, const typename std::remove_cv<value_type>::type*, value_type*>::type;

	public:
		constexpr BaseIterator() = delete;
		constexpr BaseIterator(const ArrayClass& rhs, indexType index) : it_(rhs), index_(index) {}
		constexpr BaseIterator(const BaseIterator& other) : it_(other.it_),
															index_(other.index_)
		{}
		template<bool TconstFlag = constFlag,
			typename std::enable_if<TconstFlag == true, bool>::type = false>
			constexpr BaseIterator(const BaseIterator<false>& other) : it_(other.it_),
																	   index_(other.index_)
		{}

		template<bool TconstFlag>
		constexpr bool operator==(const BaseIterator<TconstFlag>& rhs) const noexcept { return ((index_ == rhs.index_) && (it_ == rhs.it_)); }
		template<bool TconstFlag>
		constexpr bool operator!=(const BaseIterator<TconstFlag>& rhs) const noexcept { return !(*this == rhs); }

		constexpr typename std::remove_cv<value_type>::type operator*() const { return it_[index_]; }
		constexpr pointer operator->() const { return &(it_[index_]); }

		constexpr operator BaseIterator<true>() const noexcept
		{
			return{ it_, index_ };
		}
	protected:
		const ArrayClass& it_;
		indexType index_;
	};

	template<bool constFlag>
	class ReverseIterator;

	template<bool constFlag>
	class Iterator : public BaseIterator<constFlag>
	{
	protected:
		using Base = BaseIterator<constFlag>;
		using indexType = typename Base::indexType;
		friend Iterator<!constFlag>;

	public:
		using value_type = typename Base::value_type;
		using reference = typename Base::reference;
		using pointer = typename Base::pointer;

	public:
		constexpr Iterator(const ArrayClass& rhs, indexType index) noexcept : Base(rhs, index)
		{}
		constexpr Iterator(const Iterator& other) noexcept : Base(other)
		{}
		template<bool TconstFlag = constFlag,
			typename std::enable_if<TconstFlag == true, bool>::type = false>
			constexpr Iterator(const Iterator<false>& other) noexcept : Base(other)
		{}
		constexpr Iterator(const ReverseIterator<constFlag>& other) noexcept : Base(other)
		{}
		template<bool TconstFlag = constFlag,
			typename std::enable_if<TconstFlag == true, bool>::type = false>
			constexpr Iterator(const ReverseIterator<false>& other) noexcept : Base(other)
		{}

		constexpr Iterator& operator++() const noexcept
		{
			++(this->index_);
			return *this;
		}

		constexpr Iterator operator++(int) const noexcept
		{
			Iterator tmp{ *this };
			++(this->index_);
			return tmp;
		}

		constexpr Iterator& operator--() const noexcept
		{
			--(this->index_);
			return *this;
		}

		constexpr Iterator operator--(int) const noexcept
		{
			Iterator tmp{ *this };
			--(this->index_);
			return tmp;
		}
	};

	template<bool constFlag>
	class ReverseIterator : public Iterator<constFlag>
	{
	protected:
		using Base = Iterator<constFlag>;
		using indexType = typename Base::indexType;
		friend ReverseIterator<!constFlag>;

	public:
		using value_type = typename Base::value_type;
		using reference = typename Base::reference;
		using pointer = typename Base::pointer;

	public:
		// Right now, this constructor do not work (visual studio 2015 update 1). I guess it's again the bad implementation of templates
		// and two pass lookup that do not play in our favor
		//constexpr ReverseIterator(const ArrayClass& rhs, indexType index) noexcept : Base(rhs, index) {}
		constexpr ReverseIterator(const ReverseIterator& other) noexcept : Base(other)
		{}
		template<bool TconstFlag = constFlag,
			typename std::enable_if<TconstFlag == true, bool>::type = false>
		constexpr ReverseIterator(const ReverseIterator<false>& other) noexcept : Base(other.it_, other.index_)
		{}
		constexpr ReverseIterator(const Iterator<constFlag>& other) noexcept : Base(other)
		{}
		template<bool TconstFlag = constFlag,
			typename std::enable_if<TconstFlag == true, bool>::type = false>
		constexpr ReverseIterator(const Iterator<false>& other) noexcept : Base(other)
		{}

		constexpr ReverseIterator& operator++() const noexcept
		{
			return static_cast<ReverseIterator&>(Base::operator--());
		}

		constexpr ReverseIterator operator++(int) const noexcept
		{
			return Base::operator--(0);
		}

		constexpr ReverseIterator& operator--() const noexcept
		{
			return static_cast<ReverseIterator&>(Base::operator++());
		}

		constexpr ReverseIterator operator--(int) const noexcept
		{
			return Base::operator++(0);
		}

		constexpr typename std::remove_cv<value_type>::type operator*() const { return this->it_[this->index_ - 1]; }
		constexpr pointer operator->() const { return &(this->it_[this->index_ - 1]); }
	};

public:
	using iterator = Iterator<false>;
	using reverse_iterator = ReverseIterator<false>;
	using const_iterator = Iterator<true>;
	using const_reverse_iterator = ReverseIterator<true>;
};
#pragma warning(pop)

class ConstString
{
	using IterPolicy = ArrayIteratorPolicy<ConstString>;
public:
	using value_type = const char;
	using reference = const char&;
	using pointer = const char*;
public:
	// Serve for the sole purpose of begin able to be literal type even with default constructor
	constexpr ConstString() : size_(0), cstr_("")
	{}
	template<size_t size>
	constexpr ConstString(const char(&cstr)[size]) noexcept : size_(size - 1), cstr_(cstr)
	{}

	constexpr char at(size_t index) const
	{
		return (index < size_ ? cstr_[index] : throw std::out_of_range("Attempt to access a non-existing index of a constant string"));
	}

	constexpr char operator[](size_t index) const
	{
		return at(index);
	}

	// Modified to build with visual studio. Gcc and clang do not need recursion, simple if branches and for loop are working.
	constexpr bool operator==(const ConstString& rhs) const
	{
		return size() != rhs.size() ? false : equalAux(size(), rhs);
	}

	constexpr operator const char*() const noexcept
	{
		return cstr_;
	}

	constexpr size_t size() const noexcept { return size_; }

	using iterator = typename IterPolicy::iterator;
	using reverse_iterator = typename IterPolicy::reverse_iterator;
	using const_iterator = typename IterPolicy::const_iterator;
	using const_reverse_iterator = typename IterPolicy::const_reverse_iterator;

	constexpr iterator begin() const noexcept { return{ *this, 0 }; }
	constexpr const_iterator cbegin() const noexcept { return begin(); }
	constexpr iterator end() const noexcept { return{ *this, size() }; }
	constexpr const_iterator cend() const noexcept { return end(); }

	constexpr reverse_iterator rbegin() const noexcept { return end(); }
	constexpr const_reverse_iterator crbegin() const noexcept { return end(); }
	constexpr reverse_iterator rend() const noexcept { return begin(); }
	constexpr const_reverse_iterator crend() const noexcept { return begin(); }

private:
	constexpr bool equalAux(size_type size, const ConstString& rhs) const
	{
		return size != 0 ? ((*this)[size - 1] == rhs[size - 1] && equalAux(size - 1, rhs)) : true;
	}

private:
	size_t size_;
	const char* cstr_;
};

template<size_t Tsize>
class StaticString
{
	using IterPolicy = ArrayIteratorPolicy<StaticString<Tsize>>;
public:
	// Serve for the sole purpose of begin able to be literal type even with default constructor
	constexpr StaticString() : cstr_("")
	{}
	template<size_t otherSize>
	constexpr StaticString(const char(&cstr)[otherSize]) : cstr_(cstr)
	{}

	constexpr char operator[](size_t index) const
	{
		return (index < Tsize ? cstr_[index] : throw std::out_of_range("Attempt to access a non-existing index of a constant string"));
	}

	constexpr bool operator==(StaticString<Tsize> rhs) const
	{
		for (uint8_t i = 0; i < size(); ++i)
		{
			if ((*this)[i] != rhs[i])
			{
				return false;
			}
		}
		return true;
	}

	template<size_t otherSize>
	constexpr bool operator==(StaticString<otherSize>) const
	{
		return false;
	}

	constexpr operator const char*() const noexcept
	{
		return cstr_;
	}

	constexpr size_t size() const noexcept { return Tsize; }

	using iterator = typename IterPolicy::iterator;
	using reverse_iterator = typename IterPolicy::reverse_iterator;
	using const_iterator = typename IterPolicy::const_iterator;
	using const_reverse_iterator = typename IterPolicy::const_reverse_iterator;

	constexpr iterator begin() const noexcept { return{ *this, 0 }; }
	constexpr const_iterator cbegin() const noexcept { return begin(); }
	constexpr iterator end() const noexcept { return{ *this, size() }; }
	constexpr const_iterator cend() const noexcept { return end(); }

	constexpr reverse_iterator rbegin() const noexcept { return end(); }
	constexpr const_reverse_iterator crbegin() const noexcept { return end(); }
	constexpr reverse_iterator rend() const noexcept { return begin(); }
	constexpr const_reverse_iterator crend() const noexcept { return begin(); }

private:
	const char* cstr_;
};