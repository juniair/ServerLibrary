#ifndef _LOG_CLASS_
#define _LOG_CLASS_
#include "System_Library.hpp"
namespace System {

	Log::Log(TAG tag, MESSAGE message)
	{
		this->print(tag, message);
	}

	Log::Log(MESSAGE message)
	{
		this->print(message);
	}

	void Log::_make_file(std::string log)
	{
		_file.open("Log.txt", std::ios::app);
		_file << log.c_str() << std::endl;
	}


	void Log::print(TAG tag, MESSAGE message)
	{
		std::stringstream logInfo;
#ifdef WINDOWS
		logInfo << __TIMESTAMP__ << " " << tag << " : " << message;

#else
		time_t now = time(NULL);
		logInfo << ctime(&now) << " " << tag << " : " << message;
#endif

		std::cout << logInfo.str() << std::endl;
		_make_file(logInfo.str());

	}

	void Log::print(MESSAGE message)
	{
		std::stringstream logInfo;
#ifdef WINDOWS
		logInfo << __TIMESTAMP__ << message;
#else
		time_t now = time(NULL);
		logInfo << ctime(&now) << message;
#endif
		_make_file(logInfo.str());
	}

	Log::~Log()
	{
		_file.close();
	}
}


#endif // !_LOG_CLASS_
