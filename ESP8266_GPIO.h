#ifndef ESP8266_GPIO_H_INCLUDED
#define ESP8266_GPIO_H_INCLUDED


class ESP8266_GPIO
{
public:
	
	ESP8266_GPIO();
	virtual ~ESP8266_GPIO();
	
protected:

	static uint32_t getGpioName(uint8_t pinNr);
	static uint32_t getGpioFunc(uint8_t pinNr);
	
	static bool initPin(uint8_t pinNr, bool output);
	
	static void setGpioValue(uint8_t pinNr, bool value);
	static bool getGpioValue(uint8_t pinNr);
	
	
private:

	static bool m_bGpioInitCalled;
	
	ESP8266_GPIO(const ESP8266_GPIO&);
	ESP8266_GPIO& operator=(const ESP8266_GPIO&);
};

#endif
