// Enabled tracing via serial (currently only with Arduino)
//#define ENABLE_TRACE

#include "ESP8266_GPIO.h"

extern "C" {
  #include "user_interface.h"
}


#ifdef ENABLE_TRACE
	#include "HardwareSerial.h"
	#define trace Serial.printf
#else
	#define trace 
#endif



	
ESP8266_GPIO::ESP8266_GPIO()
{
	
}

ESP8266_GPIO::~ESP8266_GPIO()
{
	
}
	

unsigned int ESP8266_GPIO::getGpioName(unsigned char pinNr)
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

unsigned int ESP8266_GPIO::getGpioFunc(unsigned char pinNr)
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


bool ESP8266_GPIO::ms_bGpioInitCalled = false;

bool ESP8266_GPIO::initPin(unsigned char pinNr, bool output)
{
	unsigned int gpioName = getGpioName(pinNr);
	unsigned int gpioFunc = getGpioFunc(pinNr);
	
	if (!ms_bGpioInitCalled)
	{
		gpio_init();
		ms_bGpioInitCalled = true;
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


void ESP8266_GPIO::setGpioValue(unsigned char pinNr, bool value)
{
	GPIO_OUTPUT_SET(GPIO_ID_PIN(pinNr), value ? 1:0);
}


bool ESP8266_GPIO::getGpioValue(unsigned char pinNr)
{
	return (0 != GPIO_INPUT_GET(GPIO_ID_PIN(pinNr)));
}
		

