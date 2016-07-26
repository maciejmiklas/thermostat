#ifndef EVENTBUS_H_
#define EVENTBUS_H_

#import "Arduino.h"
#import "ArdLog.h"

typedef enum {
	/** Parameters: 0 - relay ID */
	RELAY_ON = 10,

	/** Parameters: 0 - relay ID */
	RELAY_OFF = 11,

	/** Parameters: none */
	BUTTON_NEXT = 20,

	/** Parameters: none */
	BUTTON_PREV = 21,

	/** Parameters: none */
	SERVICE_SUSPEND = 30,

	/** Parameters: none */
	SERVICE_RESUME = 31
} BusEvent;

typedef enum {
	RELAY, BUTTON, SERVICE
} BusEventGroup;

class BusListener {
public:
	virtual void onEvent(BusEvent event, va_list ap) = 0;
protected:
	virtual ~BusListener();
	BusListener();
};

boolean eb_inGroup(BusEvent event, BusEventGroup group);
void eb_register(BusListener* listener);
void eb_fire(BusEvent event, ...);

#endif /* EVENTBUS_H_ */
