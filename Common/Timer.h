#pragma once

#include "Common/TypeDef.h"

namespace WorkSpace {

class TimeManager {
private:
	TimePoint endTime;
public:
	void initial(Millisecond secTimeout) {
		endTime = Clock::now() + chrono::seconds(secTimeout);
	}
	bool is_timeout() {
		return endTime < Clock::now();
	}
};

} //WorkSpace