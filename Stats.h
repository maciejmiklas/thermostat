#ifndef STATS_H_
#define STATS_H_

#include "Arduino.h"

class Stats {
public:
	virtual void relayChange(boolean enabled, uint8_t id) = 0;
	virtual ~Stats();
};

#endif /* STATS_H_ */
