#ifndef ESP8266_RELAY_H_INCLUDED
#define ESP8266_RELAY_H_INCLUDED

#include "ESP8266_GPIO.h"

/*! \class ESP8266_Relay
    \brief Class to control a relay driven by a GPIO pin of an ESP8266

    The class can turn on or off a relay
*/
class ESP8266_Relay : public ESP8266_GPIO
{
public:
    
    ESP8266_Relay();
    virtual ~ESP8266_Relay();
    
    bool init(int pinNr, bool gpioLowSwitchOnRelay = true);
    
    void setState(bool on);
    
private:
    
    ESP8266_Relay(const ESP8266_Relay&);
    ESP8266_Relay& operator=(const ESP8266_Relay&);
        
    // Pin Nr of the relay
    int m_iPinNr;
    
    // If true, then GPIO-low will switch on the relay; otherwise GPIO-high will switch on the relay
    bool m_gpioLowSwitchOnRelay;

};

#endif
