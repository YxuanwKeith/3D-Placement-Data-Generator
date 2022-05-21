#pragma once

#include <iostream>

#include "Common/TypeDef.h"

using namespace std;

namespace WorkSpace {

class Log {
public:
	using Manipulator = std::ostream & (*)(std::ostream&);

	enum Level {
		On,
		Off,

		Fatal = On,
		Error = On,
		Warning = On,
		Debug = Off,
		Info = On
	};

	static bool isTurnedOn(int level) { return (level == On); }
	static bool isTurnedOff(int level) { return !isTurnedOn(level); }

	Log(int logLevel, ostream& logFile) : level(logLevel), os(&logFile) {}
	Log(int logLevel) : Log(logLevel, std::cerr) {}

	template<typename T>
	Log& operator<<(const T& obj) {
		if (isTurnedOn(level)) { *os << obj; }
		return *this;
	}
	Log& operator<<(Manipulator manip) {
		if (isTurnedOn(level)) { *os << manip; }
		return *this;
	}

	int level;
	ostream* os;
};

} //WorkSpace