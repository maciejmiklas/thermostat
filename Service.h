#ifndef SERVICE_H_
#define SERVICE_H_

#include "Arduino.h"
#include "EventBus.h"

class Service : public BusListener {
public:
	Service();
	virtual ~Service();

protected:
	void onEvent(BusEvent event, va_list ap);
	boolean enabled;
	virtual uint8_t deviceId() = 0;
};

#endif /* SERVICE_H_ */
