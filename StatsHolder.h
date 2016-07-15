#ifndef STATSHOLDER_H_
#define STATSHOLDER_H_

#import "Stats.h"

class StatsHolder: public Stats {
public:
	StatsHolder();
	virtual ~StatsHolder();

	void relayChange(boolean enabled, uint8_t id);
};

#endif /* STATSHOLDER_H_ */
