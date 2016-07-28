#ifndef SERVICE_H_
#define SERVICE_H_

#include "Arduino.h"
#include "EventBus.h"

class Service {

public:
	Service();
	virtual ~Service();
	void cycle();

protected:
	virtual void onCycle() = 0;
	virtual uint8_t deviceId() = 0;

private:
	class ServiceBusListener: public BusListener {
	public:
		ServiceBusListener(Service* service);
	protected:
		void onEvent(BusEvent event, va_list ap);
	private:
		Service* service;
	};

	boolean enabled;
	ServiceBusListener serviceBusListener;
};

#endif /* SERVICE_H_ */
