#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "EventBus.h"

class Display: public BusListener {
public:
	Display();

private:
	void onEvent(BusEvent event, va_list ap);
	inline void showNextScreen();
	inline void showPrevScreen();
	inline void showStartScreen();
};

#endif /* DISPLAY_H_ */
