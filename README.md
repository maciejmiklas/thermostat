This project creates a Thermostat based on Arduino, a temperature sensor, and a relay.
<img src="/doc/img/hardware_1.jpg" width="640px"/>

That gives you the possibility to drive multiple devices to control temperature. In my case, I have installed two fans in my attic to cool it down in summer. There are no windows, so I had to force airflow. The first fan starts when the temperature reaches 30 degrees and the second over 35.

You can control any reasonable amount of units, and it's all configurable. You also have access to basic statistics:

<img src="/doc/img/hardware_2.jpg" width="320px" title="Runtime of whole system"/>
<img src="/doc/img/hardware_3.jpg" width="320px" title="Running time for each relay"/>
<img src="/doc/img/hardware_5.jpg" width="320px" title="Statistics for 14 days" />
<img src="/doc/img/hardware_4.jpg" width="320px" title="Statistics for 7th day" />

# Hardware
<img src="/doc/fritzing/Thermostat_schem.png" width="640px" />
# Configuration
The whole configuration is stored in *Config.h*. You can change PINs controlling relays, reading temperature, thresholds, or timings. 

## Configuring Relays
Let's assume that we would like to have 3 relays:

| Relay ID | PIN | Temperature |  
| -------- | --- | ------------|  
| 0        |  1  |          20 |  
| 1        |  10 |          30 |  
| 2        |  11 |          40 |  

First, you have to ensure that your choice's PIN is not already taken. All pins can be found in *Config.h*. They are defined by variables starting with *DIG_PIN*.

Edit *Config.h* and configure PINs, thresholds, and amount of relays. 
```cpp
const static uint8_t DIG_PIN_RELAY_0 = 1;
const static uint8_t DIG_PIN_RELAY_1 = 10;
const static uint8_t DIG_PIN_RELAY_2 = 11;

const static uint8_t RELAYS_AMOUNT = 3;

const static int16_t RELAY_TEMP_SET_POINT_0 = 20;
const static int16_t RELAY_TEMP_SET_POINT_1 = 30;
const static int16_t RELAY_TEMP_SET_POINT_2 = 40;
```

Now we have to set up relays and controller. This happens in *RelayDriver.cpp* 
```cpp
void RelayDriver::init() {
    initRelayHysteresisController(0, DIG_PIN_RELAY_0, RELAY_TEMP_SET_POINT_0);
    initRelayHysteresisController(1, DIG_PIN_RELAY_1, RELAY_TEMP_SET_POINT_1);
    initRelayHysteresisController(2, DIG_PIN_RELAY_2, RELAY_TEMP_SET_POINT_2);
}
```

## Choosing Controller
These two controllers available Hysteresis and PID

<img src="/doc/img/Relay.png" width="640px" />

### Hysteresis Controller
It's the one chosen in the example above, and it has a few additional configurations:
```cpp
const static uint32_t RELAY_DELAY_AFTER_SWITCH_MS = 300000; // 5 minutes
const static uint32_t RHC_RELAY_MIN_SWITCH_MS = 3600000;
```
*RELAY_DELAY_AFTER_SWITCH_MS* gives a wait time for switching the next relay. Imagine that configuration from our example would start working in 40 degrees environment. It would result in enabling all three relays at the same time. That could eventually lead to high power consumption - depending on what you are controlling (an electric engine, for example, consumes more power during the start). In our case, switching relays have the following flow: the first relay starts, and waits 5 minutes. The second goes on, wait 5 minutes, third goes on.

*RHC_RELAY_MIN_SWITCH_MS* defines hysteresis. It's the minimum frequency for a particular relay to change its state. Once it's on, it will remain on for at least this period, ignoring temperature changes. It is helpful if you control electric motors since each switch negatively impacts live time.

# Software Design

## Message Bus
Different software modules have to communicate with each other, hopefully not both ways ;) 

For example: 
* statistics module has to know when a particular relay goes on and off,
* pressing a button has to change display content, and it also has to suspend services that would consume many CPU cycles, for example, temperature reading from a sensor,
* after some time temperature reading has to be renewed, 
* and so on....

Every module is connected to Message Bus and can register for a particular event and produce any events. 
<img src="/doc/img/Message_Bus.png" width="640px"/>

For example, pressing Next button results in the following flow:
<img src="/doc/img/Button_NEXT.png" width="640px"/>

Some components have some tasks that need to be executed periodically. We could call their corresponding methods from the main loop. Since we have Message Bus, it's only necessary to propagate the right event:
<img src="/doc/img/CYCLE_Event.png" width="640px"/>

# LIBS
The following libs are required to compile the Thermostat:
* https://github.com/milesburton/Arduino-Temperature-Control-Library
* https://github.com/maciejmiklas/ArdLog.git
* https://github.com/mmurdoch/arduinounit
