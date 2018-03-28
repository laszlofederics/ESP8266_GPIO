#ifndef ESP8266_LED_H_INCLUDED
#define ESP8266_LED_H_INCLUDED

extern "C" {
  #include "user_interface.h"
}

#include "ESP8266_GPIO.h"

void ledBlinkingCB(void * args);

/*! \class ESP8266_LED
    \brief Class to control a LED driven by a GPIO pin of an ESP8266

    The class can turn on or off an LED, or make it blink in different ways.
*/
class ESP8266_LED : public ESP8266_GPIO
{
public:
    
    ESP8266_LED();
    virtual ~ESP8266_LED();
    
    /*! Initializes an LED on the specified pin of the ESP8266. 
        Set gpioLowTurnsOnLED to true, pin-low turns on the LED.
        Set gpioLowTurnsOnLED to false, pin-high turns on the LED
        Returns true, if the function succeeded; otherwise returns false.
    */
    bool init(int pinNr, bool gpioLowTurnsOnLED = true);
    
    /*! \enum LedState
        \brief Different states of an LED (on / off / different types of blinking)
    */
    enum LedState
    {
        /* Swith off the LED */
        Off = -1,

        /* Swith on the LED */
        On = 0,

        /* The LED is continuously blinking: one short flash and a long pause */
        OneShortFlashLongPause = 1,

        /* The LED is continuously blinking: one short flash and a short pause */
        OneShortFlashShortPause = 2,

        /* The LED is continuously blinking: two short flash and a long pause */
        TwoShortFlashLongPause = 3,

        /* The LED is continuously blinking: one long flash and a long pause */
        LongFlashLongPause = 4,

        /* The LED is continuously blinking: three short flash and a long pause */
        ThreeShortFlashLongPause = 5,

        /* The LED makes ten short flash and turns off */
        TenShortFlash = 10
    };
    
    
    /*! Sets the state of the LED (on / off / different types of blinking).
    */
    void setState(LedState nState);
    
private:
    
    ESP8266_LED(const ESP8266_LED&);
    ESP8266_LED& operator=(const ESP8266_LED&);
    
    
    const char* ledStateStr(LedState ledstate);
    
    // Pin Nr of the LED
    int m_iPinNr;
    
    // If true, then GPIO-low will light the LED; otherwise GPIO-high will light the LED
    bool m_gpioLowTurnsOnLED;
    
    // Timer to turn the led on/off to make it blink
    os_timer_t m_timerLed;

    // Actual flashing mode
    LedState m_ledFlashType;

    // Helper variable for blinking LED
    int m_iLedBlinkCounter;
    
    void turnOnLed(bool bON);
    bool isLedOn();
    void toggleLed();
    
    friend void ledBlinkingCB(void * args);
};

#endif
