#include "HardwareSerial.h"


#include "ESP8266_GPIO.h"

extern "C" {
  #include "user_interface.h"
}


#define trace Serial.printf
//#define trace 


	
ESP8266_GPIO::ESP8266_GPIO()
{
	
}

ESP8266_GPIO::~ESP8266_GPIO()
{
	
}
	

uint32_t ESP8266_GPIO::getGpioName(uint8_t pinNr)
{
  switch (pinNr) {
    case 0:
      return PERIPHS_IO_MUX_GPIO0_U;
    case 1:
      return PERIPHS_IO_MUX_U0TXD_U;
    case 2:
      return PERIPHS_IO_MUX_GPIO2_U;
    case 3:
      return PERIPHS_IO_MUX_U0RXD_U;
    case 4:
      return PERIPHS_IO_MUX_GPIO4_U;
    case 5:
      return PERIPHS_IO_MUX_GPIO5_U;
    case 9:
      return PERIPHS_IO_MUX_SD_DATA2_U;
    case 10:
      return PERIPHS_IO_MUX_SD_DATA3_U;
    case 12:
      return PERIPHS_IO_MUX_MTDI_U;
    case 13:
      return PERIPHS_IO_MUX_MTCK_U;
    case 14:
      return PERIPHS_IO_MUX_MTMS_U;
    case 15:
      return PERIPHS_IO_MUX_MTDO_U;
    default:
	  trace("ERROR in ESP8266_GPIO::getGpioName(%d): wrong input", pinNr);	
      return 0xffff;
  }
}

uint32_t ESP8266_GPIO::getGpioFunc(uint8_t pinNr)
{
  switch (pinNr) {
    case 0:
      return FUNC_GPIO0;
    case 1:
      return FUNC_GPIO1;
    case 2:
      return FUNC_GPIO2;
    case 3:
      return FUNC_GPIO3;
    case 4:
      return FUNC_GPIO4;
    case 5:
      return FUNC_GPIO5;
    case 9:
      return FUNC_GPIO9;
    case 10:
      return FUNC_GPIO10;
    case 12:
      return FUNC_GPIO12;
    case 13:
      return FUNC_GPIO13;
    case 14:
      return FUNC_GPIO14;
    case 15:
      return FUNC_GPIO15;
    default:
      trace("ERROR in ESP8266_GPIO::getGpioFunc(%d): wrong input", pinNr);	
      return 0xffff;
  }
}


bool ESP8266_GPIO::m_bGpioInitCalled = false;

bool ESP8266_GPIO::initPin(uint8_t pinNr, bool output)
{
	uint8_t gpioName = getGpioName(pinNr);
	uint8_t gpioFunc = getGpioFunc(pinNr);
	
	if (!m_bGpioInitCalled)
	{
		gpio_init();
		m_bGpioInitCalled = true;
	}
	
	if (gpioName != 0xffff && gpioFunc != 0xff)
	{
		PIN_FUNC_SELECT(gpioName, gpioFunc);
		if (output)
		{
			gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0);  
		}
		else
		{
			gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr)); 
		}
		return true;
	}
	else
	{
		return false;
	}
}


void ESP8266_GPIO::setGpioValue(uint8_t pinNr, bool value)
{
	GPIO_OUTPUT_SET(GPIO_ID_PIN(pinNr), value ? 1:0);
}


bool ESP8266_GPIO::getGpioValue(uint8_t pinNr)
{
	return (0 != GPIO_INPUT_GET(GPIO_ID_PIN(pinNr)));
}
		

