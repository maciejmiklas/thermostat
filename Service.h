#ifndef SERVICE_H_
#define SERVICE_H_

#include "Arduino.h"

class Service {
public:
	Service();
	virtual ~Service();
	void setEnabled(boolean enabled);
	virtual void cycle() = 0;

protected:
	boolean enabled;
};

#endif /* SERVICE_H_ */
