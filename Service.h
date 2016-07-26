#ifndef SERVICE_H_
#define SERVICE_H_

#include "Arduino.h"
#include "EventBus.h"

class Service : public BusListener {
public:
	Service();
	virtual ~Service();
	virtual void cycle() = 0;

protected:
	void onEvent(BusEvent event, va_list ap);
	boolean enabled;
};

#endif /* SERVICE_H_ */
