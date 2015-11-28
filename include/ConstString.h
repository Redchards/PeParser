#pragma once

#include <stdexcept>

#include <Configuration.h>

template<class ArrayClass>
class ArrayIteratorPolicy
{
protected:
	template<bool constFlag>
	class BaseIterator : public std::iterator<std::bidirectional_iterator_tag, ArrayClass>
	{
	protected:
		using indexType = typename std::conditional<constFlag, const size_t, size_t>::type;
	public:
		constexpr BaseIterator() = delete;
		constexpr BaseIterator(const ArrayClass& rhs, indexType index) : it_(rhs), index_(index) {}
		constexpr BaseIterator(const BaseIterator<false>& other) : it_(other.it_), index_(other.index_) {}

		constexpr bool operator==(const BaseIterator& rhs) const { return ((index_ == rhs.index_) && (it_ == rhs.it_)); }
		constexpr bool operator!=(const BaseIterator& rhs) const { return !(*this == rhs); }

		constexpr char operator*() const { return it_[index_]; }
		constexpr const char* operator->() const { return &(it_[index_]); }

		constexpr operator BaseIterator<true>() const
		{
			return{ it_, index_ };
		}
	protected:
		const ArrayClass& it_;
		indexType index_;
	};

	template<bool constFlag>
	class Iterator : public BaseIterator<constFlag>
	{
		using Base = BaseIterator<constFlag>;
		using indexType = typename Base::indexType;
		friend Iterator<!constFlag>;
	public:
		constexpr Iterator(const ArrayClass& rhs, indexType index) : Base(rhs, index) {}
		constexpr Iterator(const Iterator<false>& other) : Base(other.it_, other.index_) {}
		constexpr Iterator& operator++() const
		{
			++(this->index_);
			return *this;
		}
		constexpr Iterator operator++(int) const
		{
			Iterator tmp{ *this };
			++(this->index_);
			return tmp;
		}
		constexpr Iterator& operator--() const
		{
			--(this->index_);
			return *this;
		}
		constexpr Iterator operator--(int) const
		{
			Iterator tmp{ *this };
			--(this->index_);
			return tmp;
		}
	};

	template<bool constFlag>
	class ReverseIterator : public BaseIterator<constFlag>
	{
		using Base = BaseIterator<constFlag>;
		using indexType = typename Base::indexType;
		friend ReverseIterator<!constFlag>;
	public:
		constexpr ReverseIterator(const ArrayClass& rhs, indexType index) : Base(rhs, index) {}
		constexpr ReverseIterator(const ReverseIterator<false>& other) : Base(other.it_, other.index_) {}
		constexpr ReverseIterator& operator++() const
		{
			--(this->index_);
			return *this;
		}
		constexpr ReverseIterator operator++(int) const
		{
			ReverseIterator tmp{ *this };
			--(this->index_);
			return tmp;
		}
		constexpr ReverseIterator& operator--() const
		{
			++(this->index_);
			return *this;
		}
		constexpr ReverseIterator operator--(int) const
		{
			ReverseIterator tmp{ *this };
			++(this->index_);
			return tmp;
		}
	};

public:
	using iterator = Iterator<false>;
	using reverse_iterator = ReverseIterator<false>;
	using const_iterator = Iterator<true>;
	using const_reverse_iterator = ReverseIterator<true>;
};

class ConstString
{
	using IterPolicy = ArrayIteratorPolicy<ConstString>;
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

	using iterator = IterPolicy::iterator;
	using reverse_iterator = IterPolicy::reverse_iterator;
	using const_iterator = IterPolicy::const_iterator;
	using const_reverse_iterator = IterPolicy::const_reverse_iterator;

	constexpr iterator begin() const noexcept { return{ *this, 0 }; }
	constexpr const_iterator cbegin() const noexcept { return begin(); }
	constexpr reverse_iterator rbegin() const noexcept { return{ *this, 0 }; }
	constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }

	constexpr iterator end() const noexcept { return{ *this, size() + 1 }; }
	constexpr const_iterator cend() const noexcept { return end(); }
	constexpr reverse_iterator rend() const noexcept { return{ *this, size() + 1 }; }
	constexpr const_reverse_iterator crend() const noexcept { return rend(); }

private:
	constexpr bool equalAux(size_type size, const ConstString& rhs) const
	{
		return size != 0 ? ((*this)[size - 1] == rhs[size - 1] && equalAux(size - 1, rhs)) : true;
	}

	const size_t size_;
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
	constexpr reverse_iterator rbegin() const noexcept { return{ *this, 0 }; }
	constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }

	constexpr iterator end() const noexcept { return{ *this, size() + 1 }; }
	constexpr const_iterator cend() const noexcept { return end(); }
	constexpr reverse_iterator rend() const noexcept { return{ *this, size() + 1 }; }
	constexpr const_reverse_iterator crend() const noexcept { return rend(); }

private:
	const char* cstr_;
};