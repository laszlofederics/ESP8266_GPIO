// Enabled tracing via serial (currently only with Arduino)
#define ENABLE_TRACE

#include "ESP8266_Button.h"

#ifdef ENABLE_TRACE
    #include "HardwareSerial.h"
    #define trace Serial.printf
#else
    #define trace 
#endif


ESP8266_Button::ESP8266_Button() : ESP8266_GPIO(), m_uButtonPressedCounter(0), m_uButtonReleasedCounter(0), m_iPinNr(-1), m_iState(0)
{
	trace(">>> ESP8266_Button::ESP8266_Button()\n");
	trace("<<< ESP8266_Button::ESP8266_Button()\n");
}

ESP8266_Button::~ESP8266_Button()
{
	trace(">>> ESP8266_Button::~ESP8266_Button()\n");
	trace("<<< ESP8266_Button::~ESP8266_Button()\n");
}
	
bool ESP8266_Button::init(int iPinNr)
{
	trace("%p >>> ESP8266_Button::init(%d)\n", this, iPinNr);
	
	m_iPinNr = iPinNr;
	
	bool bRet = initPin(iPinNr, false /* pin will be input */);
	
	m_iState = 0;
    
	waitForButtonPress();
	
	trace("%p <<< ESP8266_Button::init(%d) returns %s\n", this, iPinNr, bRet ? "true":"false");
	
	return bRet;
}

void ESP8266_Button::onPress()
{
	trace("%p ESP8266_Button::onPress()\n", this);
}

void ESP8266_Button::onDoublePress()
{
	trace("%p ESP8266_Button::onDoublePress()\n", this);
}

void ESP8266_Button::onTriplePress()
{
	trace("%p ESP8266_Button::onTriplePress()\n", this);
}

void ESP8266_Button::onLongPress()
{
	trace("%p ESP8266_Button::onLongPress()\n", this);
}

void ESP8266_Button::onVeryLongPress()
{
	trace("%p ESP8266_Button::onVeryLongPress()\n", this);
}


void gpioButtonPressed(uint32 mask, void* argument);
void gpioButtonReleased(uint32 mask, void* argument);


/* Set the interrupts to call gpio0ButtonPressed(), if the button on GPIO0 is pressed 
*/
void ICACHE_FLASH_ATTR ESP8266_Button::waitForButtonPress()
{
	trace("%p >>> ESP8266_Button[%d]::waitForButtonPress()\n", this, m_iPinNr);
	
	setInterruptForNegativeEdge(m_iPinNr, gpioButtonPressed, this);
	
	trace("%p <<< ESP8266_Button[%d]::waitForButtonPress()\n", this, m_iPinNr);
}


/* Set the interrupts to call gpio0ButtonReleased(), if the button on GPIO0 is pressed 
*/
void ICACHE_FLASH_ATTR ESP8266_Button::waitForButtonRelease()
{
	trace("%p >>> ESP8266_Button[%d]::waitForButtonRelease()\n", this, m_iPinNr);
		
	setInterruptForPositiveEdge(m_iPinNr, gpioButtonReleased, this);
	
	trace("%p <<< ESP8266_Button[%d]::waitForButtonRelease()\n", this, m_iPinNr);
}



/* Resets the button press/release counters to 0. This function must be called, after a button event is successfully detected; 
   or to recover the detection logic after an error.*/
void ICACHE_FLASH_ATTR ESP8266_Button::resetButtonPressCounters()
{
	trace("%p >>> ESP8266_Button[%d]::resetButtonPressCounters()\n", this, m_iPinNr);
	
	m_uButtonPressedCounter = 0;
	m_uButtonReleasedCounter = 0;
	m_iState = 0;
	
	trace("%p <<< ESP8266_Button[%d]::resetButtonPressCounters()\n", this, m_iPinNr);
}



void ICACHE_FLASH_ATTR callHandlerOfSinglePress(void * args)
{
	ESP8266_Button* p = (ESP8266_Button*)args;
	p->onPress();
}


void ICACHE_FLASH_ATTR callHandlerOfDoublePress(void * args)
{
	ESP8266_Button* p = (ESP8266_Button*)args;
	p->onDoublePress();
}


void ICACHE_FLASH_ATTR callHandlerOfTriplePress(void * args)
{
	ESP8266_Button* p = (ESP8266_Button*)args;
	p->onTriplePress();
}


void ICACHE_FLASH_ATTR callHandlerOfLongPress(void * args)
{
	ESP8266_Button* p = (ESP8266_Button*)args;
	p->onLongPress();
}


void ICACHE_FLASH_ATTR callHandlerOfVeryLongPress(void * args)
{
	ESP8266_Button* p = (ESP8266_Button*)args;
	p->onVeryLongPress();
}


/* This function is called, if the timer for long button press finished
*/
void ICACHE_FLASH_ATTR buttonLongPressTimeout(void * args)
{
#ifdef TRACE_BUTTON
	uint32 t = system_get_time();
	uint32 usec = t % 1000;
	uint32 msec = (t/1000) % 1000;
	uint32 sec = (t/1000000) % 1000;
	trace("%02d:%03d.%03d >>> buttonLongPressTimeout()\n", sec, msec, usec); 
#endif

	ESP8266_Button* p = (ESP8266_Button*)args;

	p->m_iState = 2;
	
	trace("<<< buttonLongPressTimeout()\n");
}

void ICACHE_FLASH_ATTR buttonShortPressTimeout(void * args)
{
#ifdef TRACE_BUTTON
	uint32 t = system_get_time();
	uint32 usec = t % 1000;
	uint32 msec = (t/1000) % 1000;
	uint32 sec = (t/1000000) % 1000;
	trace("%02d:%03d.%03d >>> buttonShortPressTimeout()\n", sec, msec, usec); 
#endif

    ESP8266_Button* p = (ESP8266_Button*)args;
		
	if (1 == p->m_uButtonPressedCounter && 1 == p->m_uButtonReleasedCounter)
	{
		/* The button was pressed and released once within 2 seconds */
		
		trace("Call button press callback function\n");
		p->resetButtonPressCounters();
		
		os_timer_disarm(&(p->m_timerButton));
		os_timer_setfn(&(p->m_timerButton), callHandlerOfSinglePress, args);
		os_timer_arm(&(p->m_timerButton), 100, false);
	}
	else if (2 == p->m_uButtonPressedCounter && 2 == p->m_uButtonReleasedCounter)
	{
		/* The button was pressed and released twice within 2 seconds */
		
		trace("Call button double press callback function\n");
		p->resetButtonPressCounters();
		
		os_timer_disarm(&(p->m_timerButton));
		os_timer_setfn(&(p->m_timerButton), callHandlerOfDoublePress, args);
		os_timer_arm(&(p->m_timerButton), 100, false);
	}
	else if (3 == p->m_uButtonPressedCounter && 3 == p->m_uButtonReleasedCounter)
	{
		/* The button was pressed and released four times within 2 seconds */
		
		trace("Call button triple press callback function\n");
		p->resetButtonPressCounters();
		
		os_timer_disarm(&(p->m_timerButton));
		os_timer_setfn(&(p->m_timerButton), callHandlerOfTriplePress, args);
		os_timer_arm(&(p->m_timerButton), 100, false);
	}
	else if (1 == p->m_uButtonPressedCounter && 0 == p->m_uButtonReleasedCounter)
	{
		/* The button is held for 2 seconds. Start another timer to check how long it will be held */
		
		trace("Button is hold for 2 secs. Start timer for long button press.\n");
		
		p->m_iState = 1;
		
		os_timer_disarm(&(p->m_timerButton));
		os_timer_setfn(&(p->m_timerButton), buttonLongPressTimeout, args);
		os_timer_arm(&(p->m_timerButton), 3000, false);
	}
	else
	{
		trace("buttonShortPressTimeout, unexpected status %d/%d\n", p->m_uButtonPressedCounter, p->m_uButtonReleasedCounter);
		p->resetButtonPressCounters();
	}
	
	trace("<<< buttonShortPressTimeout()\n");
}



void gpioButtonPressed(uint32 mask, void* argument)
{
#ifdef TRACE_BUTTON
	uint32 t = system_get_time();
	uint32 usec = t % 1000;
	uint32 msec = (t/1000) % 1000;
	uint32 sec = (t/1000000) % 1000;
	trace("%02d:%03d.%03d >>> gpioButtonPressed(0x%08x, %p)\n", sec, msec, usec, mask, argument); 
#endif

	ESP8266_Button* p = (ESP8266_Button*)argument;

	if (0 == p->m_uButtonPressedCounter)
	{
		// start detecting a button press event
		
		os_timer_disarm(&p->m_timerButton);
		os_timer_setfn(&p->m_timerButton, buttonShortPressTimeout, p);
		os_timer_arm(&p->m_timerButton, 2000, false);
	}
	
	++(p->m_uButtonPressedCounter);
	
	gpio_intr_ack(mask);
	
	p->waitForButtonRelease();
	
	trace("<<< gpioButtonPressed(0x%08x)\n", mask);
}

void gpioButtonReleased(uint32 mask, void* argument)
{
#ifdef TRACE_BUTTON
	uint32 t = system_get_time();
	uint32 usec = t % 1000;
	uint32 msec = (t/1000) % 1000;
	uint32 sec = (t/1000000) % 1000;
	trace("%02d:%03d.%03d >>> gpioButtonReleased(0x%08x, %p)\n", sec, msec, usec, mask, argument); 
#endif

	ESP8266_Button* p = (ESP8266_Button*)argument;
	
	if (0 == p->m_iState)
	{
		++(p->m_uButtonReleasedCounter);
		gpio_intr_ack(mask);
		p->waitForButtonPress();
	}
	else
	{
		os_timer_disarm(&p->m_timerButton);
		
		if (1 == p->m_iState)
		{
			/* The button was hold for more than 2 secs, but released before 5 secs.*/
			
			trace("Call button long press callback function\n");
			p->resetButtonPressCounters();
			
			os_timer_disarm(&(p->m_timerButton));
			os_timer_setfn(&(p->m_timerButton), callHandlerOfLongPress, argument);
			os_timer_arm(&(p->m_timerButton), 100, false);	
		}
		else
		{
			/* The button was hold for more than 5 secs */
			
			trace("Button is hold for 5 secs. Call button very long press callback function.\n");
			p->resetButtonPressCounters();
			
			os_timer_disarm(&(p->m_timerButton));
			os_timer_setfn(&(p->m_timerButton), callHandlerOfVeryLongPress, argument);
			os_timer_arm(&(p->m_timerButton), 100, false);			
		}
		
		p->m_iState = 0;
		gpio_intr_ack(mask);
		p->waitForButtonPress();
	}
	
	trace("<<< gpioButtonReleased(0x%08x)\n", mask);
}

	
