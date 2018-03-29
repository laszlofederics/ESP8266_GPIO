#ifndef ESP8266_BUTTON_H_INCLUDED
#define ESP8266_BUTTON_H_INCLUDED

#include "ESP8266_GPIO.h"

extern "C" {
  #include "user_interface.h"
}

void gpioButtonPressed(uint32 mask, void* argument);
void gpioButtonReleased(uint32 mask, void* argument);
void buttonLongPressTimeout(void*);
void buttonShortPressTimeout(void*);


class ESP8266_Button :  public ESP8266_GPIO
{
public:

	ESP8266_Button();
	~ESP8266_Button();
	
	bool init(int iPinNr);
	
	virtual void onPress();
	virtual void onDoublePress();
	virtual void onTriplePress();
	virtual void onLongPress();
	virtual void onVeryLongPress();

private:

	ESP8266_Button(const ESP8266_Button&);
	ESP8266_Button& operator=(const ESP8266_Button&);
	
	/* Counts the number of button presses */
	uint8 m_uButtonPressedCounter ;

	/* Counts the number of button releases */
	uint8 m_uButtonReleasedCounter;

	/* Timer to detect 
	- single press, double press, triple press within 2 sec
	- long press, very long press within 5 sec
	- start the callback functions
	*/
	os_timer_t m_timerButton;
	
	int m_iPinNr;
	
	void waitForButtonPress();
	void waitForButtonRelease();
	
	void resetButtonPressCounters();
	
	int m_iState;
	
	friend void gpioButtonPressed(uint32 mask, void* argument);
	friend void gpioButtonReleased(uint32 mask, void* argument);
	friend void buttonLongPressTimeout(void*);
	friend void buttonShortPressTimeout(void*);
};



#endif
