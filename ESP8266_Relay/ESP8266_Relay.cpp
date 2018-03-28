// Enabled tracing via serial (currently only with Arduino)
//#define ENABLE_TRACE

#include "ESP8266_Relay.h"

#ifdef ENABLE_TRACE
    #include "HardwareSerial.h"
    #define trace Serial.printf
#else
    #define trace 
#endif



    
ESP8266_Relay::ESP8266_Relay() : ESP8266_GPIO(), m_iPinNr(-1), m_gpioLowSwitchOnRelay(true)
{
    trace(">>> ESP8266_Relay::ESP8266_Relay()\n");
    trace("<<< ESP8266_Relay::ESP8266_Relay()\n");
}

ESP8266_Relay::~ESP8266_Relay()
{
    trace(">>> ESP8266_Relay::~ESP8266_Relay()\n");
    trace("<<< ESP8266_Relay::~ESP8266_Relay()\n");
}
    
bool ESP8266_Relay::init(int pinNr, bool gpioLowSwitchOnReleay)
{
    trace(">>> ESP8266_Relay::init(%d, %s)\n", pinNr, gpioLowSwitchOnReleay ?"true":"false");
    
    m_iPinNr = pinNr;
    m_gpioLowSwitchOnRelay = gpioLowSwitchOnReleay;
    
    bool bRet = initPin(pinNr, true /* output */);
    
    trace("<<< ESP8266_Relay::init(%d) returns %s\n", pinNr, bRet ? "true":"false");
    
    return bRet;
}
    
    
void ESP8266_Relay::setState(bool on)
{
    trace(">>> ESP8266_Relay[%d]::setState(%s)\n", m_iPinNr, on?"ON":"OFF");
    
    if (on)
    {
        setGpioValue(m_iPinNr, !m_gpioLowSwitchOnRelay);
    }
    else
    {
        setGpioValue(m_iPinNr, m_gpioLowSwitchOnRelay);
    }
    
    trace("<<< ESP8266_Relay[%d]::setState(%s)\n", m_iPinNr, on?"ON":"OFF");
}
    
    