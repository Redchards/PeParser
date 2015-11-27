#pragma once

#include <Configuration.h>

#include <map>
#include <ostream>
#include <string>
#include <utility>

// A very simple logging system, shamelessly inspired by :
// https://github.com/CppCon/CppCon2014/blob/master/Lightning%20Talks/Cheap,%20Simple,%20and%20Safe%20Logging%20Using%20Expression%20Templates/Cheap,%20Simple,%20and%20Safe%20Logging%20Using%20Expression%20Templates%20-%20Marc%20Eaddy.pdf


enum class LogLevel : flag_type
{
	info = 0,
	warning,
	error,
	fatal
};

static std::map<LogLevel, std::string> LogLevelName {
	{LogLevel::info, "Info"},
	{LogLevel::warning, "Warning"},
	{LogLevel::error, "Error"},
	{LogLevel::fatal, "Fatal"}
};

template<typename List>
struct LogData {
	typedef List type;
	List list;
};

struct DummyLogData {};

template<LogLevel level>
class LogBase
{
	public:
	LogBase(std::ostream& os) : os_(os) {};
	~LogBase() = default;

	protected:
	std::ostream& os_;
};

template<>
class LogBase<LogLevel::fatal>
{
	public:
	LogBase(std::ostream& os) : os_(os){}
	~LogBase()
	{
		std::terminate();
	}

	protected:
	std::ostream& os_;
};

template<LogLevel level>
class LogIntermediate : public LogBase<level>
{
	static std::ostream& DefaultOstream;

	public:
	LogIntermediate(const char* file, size_type line, std::ostream& os = DefaultOstream) : LogBase(os)
	{
		os_ << "(" << file << ":" << line << ") " << LogLevelName[level] << " : ";
	};

	~LogIntermediate() {};

	template<typename TLogData>
	void operator<<(TLogData&& data)
	{
		write(std::forward<TLogData::type>(data.list));
	}


	private:
	template<typename TLogDataPair>
	void write(TLogDataPair&& pair)
	{
		write(std::forward<typename TLogDataPair::first_type>(pair.first));
		os_ << std::forward<typename TLogDataPair::second_type>(pair.second);
	}

	template<>
	void write<DummyLogData>(DummyLogData&&) {}
};

template<LogLevel level>
std::ostream& LogIntermediate<level>::DefaultOstream = std::cout;

#define LOG(level, data) LogIntermediate<level>(__FILE__, __LINE__) << (LogData<DummyLogData>() << data)
#define LOGTO(level, out, data) LogIntermediate<level(__FILE__, __LINE__, out) << (LogData<DummyLogData>() << data)

template<typename T, typename U>
LogData<std::pair<T&&, U&&>> operator<<(LogData<T>&& orig, U&& input)
{
	return {{ std::forward<T>(orig.list), std::forward<U>(input) }};
}