// Enabled tracing via serial (currently only with Arduino)
//#define ENABLE_TRACE

#include "ESP8266_LED.h"

#ifdef ENABLE_TRACE
    #include "HardwareSerial.h"
    #define trace Serial.printf
#else
    #define trace 
#endif

#define LED_TIMER_PERIOD 200


    
ESP8266_LED::ESP8266_LED() : ESP8266_GPIO(), m_iPinNr(-1), m_gpioLowTurnsOnLED(true), m_ledFlashType(Off), m_iLedBlinkCounter(0)
{
    trace(">>> ESP8266_LED::ESP8266_LED()\n");
    trace("<<< ESP8266_LED::ESP8266_LED()\n");
}

ESP8266_LED::~ESP8266_LED()
{
    trace(">>> ESP8266_LED::~ESP8266_LED()\n");
    trace("<<< ESP8266_LED::~ESP8266_LED()\n");
}
    
bool ESP8266_LED::init(int pinNr, bool gpioLowTurnsOnLED)
{
    trace(">>> ESP8266_LED::init(%d, %s)\n", pinNr, gpioLowTurnsOnLED ?"true":"false");
    
    m_iPinNr = pinNr;
    m_gpioLowTurnsOnLED = gpioLowTurnsOnLED;
    
    bool bRet = initPin(pinNr, true /* pin will be output */);
    
    trace("<<< ESP8266_LED::init(%d) returns %s\n", pinNr, bRet ? "true":"false");
    
    return bRet;
}
    
    
void ESP8266_LED::setState(LedState flashType)
{
    trace("ESP8266_LED[%d]::setState(%s) (prev. led state=%s)\n", m_iPinNr, ledStateStr(flashType), ledStateStr(m_ledFlashType));
        
    if (flashType == m_ledFlashType) { return; } else { m_ledFlashType = flashType; }
    
    os_timer_disarm(&m_timerLed);
    
    switch (m_ledFlashType)
    {
    case On:
    case Off:
    case OneShortFlashShortPause:
    case OneShortFlashLongPause:   
    case TwoShortFlashLongPause:   
    case ThreeShortFlashLongPause: 
    case LongFlashLongPause:
        m_iLedBlinkCounter = 0;
        break;
        
    case TenShortFlash:
        turnOnLed(false);
        m_iLedBlinkCounter = 20;
        break;
    }
    
    os_timer_setfn(&m_timerLed, ledBlinkingCB, this);
    os_timer_arm(&m_timerLed, LED_TIMER_PERIOD, true);
}
    
    
const char* ESP8266_LED::ledStateStr(LedState ledstate)
{
    switch (ledstate)
    {
    case Off: return "Off";
    case On: return "On";
    case OneShortFlashLongPause: return "OneShortFlashLongPause";
    case OneShortFlashShortPause: return "OneShortFlashShortPause";
    case TwoShortFlashLongPause: return "TwoShortFlashLongPause";  
    case LongFlashLongPause: return "LongFlashLongPause";
    case ThreeShortFlashLongPause: return "ThreeShortFlashLongPause";
    case TenShortFlash: return "TenShortFlash";
    default: return "UNKNOWN";
    }
}


void ICACHE_FLASH_ATTR ESP8266_LED::turnOnLed(bool bON)
{
    if (bON)
    {
        trace("ESP8266_LED[%d]::turnOnLed(true)\n", m_iPinNr);
        setGpioValue(m_iPinNr, !m_gpioLowTurnsOnLED);
    }
    else
    {
        trace("ESP8266_LED[%d]::turnOnLed(false)\n", m_iPinNr);
        setGpioValue(m_iPinNr, m_gpioLowTurnsOnLED);
    }
}

bool ICACHE_FLASH_ATTR ESP8266_LED::isLedOn()
{
    bool gpioval = getGpioValue(m_iPinNr);
    bool off = m_gpioLowTurnsOnLED ? gpioval : !gpioval;
    trace("ESP8266_LED[%d]::isLedOn() returns %s\n", m_iPinNr, !off ? "ON":"OFF");
    return !off;
}

void ICACHE_FLASH_ATTR ESP8266_LED::toggleLed()
{
    turnOnLed(!isLedOn());
}


void ledBlinkingCB(void * args)
{
    ESP8266_LED* p = (ESP8266_LED*)args;
    
//#ifdef TRACE_GPIO_LED
    uint32 t = system_get_time();
    uint32 usec = t % 1000;
    uint32 msec = (t/1000) % 1000;
    uint32 sec = (t/1000000) % 1000;
    trace("%02d:%03d.%03d >>> ledBlinkingCB() m_iPinNr=%d, m_iLedBlinkCounter=%d, m_ledFlashType=%d\n", sec, msec, usec, p->m_iPinNr, p->m_iLedBlinkCounter, p->m_ledFlashType); 
//#endif

    switch (p->m_ledFlashType)
    {
    case ESP8266_LED::On:
        if (!p->isLedOn()) { p->turnOnLed(true); }
        break;
    
    case ESP8266_LED::Off:
        if (p->isLedOn()) { p->turnOnLed(false); }
        break;
        
    case ESP8266_LED::OneShortFlashShortPause:
        p->toggleLed();
        break;
    
    case ESP8266_LED::OneShortFlashLongPause:   /* value: 1 */
    case ESP8266_LED::TwoShortFlashLongPause:   /* value: 3 */
    case ESP8266_LED::ThreeShortFlashLongPause: /* value: 5 */ 
        if (p->m_iLedBlinkCounter == 0)
        {
            p->turnOnLed(true);
            p->m_iLedBlinkCounter = p->m_ledFlashType;
        }
        else if (0 < p->m_iLedBlinkCounter)
        {
            p->toggleLed();
            if (!--(p->m_iLedBlinkCounter)) { p->m_iLedBlinkCounter = -4; }
        }
        else
        {
            (p->m_iLedBlinkCounter)++;
        }
        break;
    
    case ESP8266_LED::LongFlashLongPause:
        if (p->m_iLedBlinkCounter <= 0)
        {
            p->toggleLed();
            p->m_iLedBlinkCounter = 4;
        }
        else 
        {
            (p->m_iLedBlinkCounter)--;
        }
        break;
    
    case ESP8266_LED::TenShortFlash:
        if (0 < p->m_iLedBlinkCounter) 
        {
            (p->m_iLedBlinkCounter)--;
            p->toggleLed();
        }
        break;
    }
    
    trace("<<< ledBlinkingCB()\n");
}
