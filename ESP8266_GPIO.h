#ifndef ESP8266_GPIO_H_INCLUDED
#define ESP8266_GPIO_H_INCLUDED

/*! \class ESP8266_GPIO
    \brief Basic functions to access the pins of an esp8266
	
	Basic functions to access the pins of an esp8266. This class in intended to be used a base class.
*/
class ESP8266_GPIO
{
public:
	
	ESP8266_GPIO();
	virtual ~ESP8266_GPIO();
	
protected:

	/*! Returns the name of the pin
	*/
	static unsigned int getGpioName(unsigned char pinNr);
	
	/*! Returns the funtion of the pin
	*/
	static unsigned int getGpioFunc(unsigned char pinNr);
	
	/*! Initializes the pin as an input or as an output.
	    It also calls the SDK function gpio_init(), if it hasn't been called so far.
	*/
	static bool initPin(unsigned char pinNr, bool output /* true: output; false: input */);
	
	/*! Sets the value of the pin
	*/
	static void setGpioValue(unsigned char pinNr, bool value);
		
	/*! Reads the value of the pin
	*/
	static bool getGpioValue(unsigned char pinNr);
	
	
private:

	/*! The variable shows, if the SDK function gpio_init() has been called so far.
	*/
	static bool ms_bGpioInitCalled;
	
	// disable copy constructor and operator=
	ESP8266_GPIO(const ESP8266_GPIO&);
	ESP8266_GPIO& operator=(const ESP8266_GPIO&);
};

#endif
