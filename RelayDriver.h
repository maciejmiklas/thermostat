#ifndef RELAYDRIVER_H_
#define RELAYDRIVER_H_

#import "TempSensor.h"
#import "Stats.h"
#import "Relay.h"
#import "Service.h"
#import "EventBus.h"
#import "Config.h"

/**
 * We are supporting two fans: first will go on after reaching first temperature threshold, and second one after
 * reaching second threshold.
 */
class RelayDriver: public Service {
public:
	RelayDriver(TempSensor* ts);


protected:
	uint8_t deviceId();

private:
	void onCycle();
	Relay relay1;
	Relay relay2;

	inline void driveRelay(Relay* relay, uint8_t id);
};

#endif /* RELAYDRIVER_H_ */
