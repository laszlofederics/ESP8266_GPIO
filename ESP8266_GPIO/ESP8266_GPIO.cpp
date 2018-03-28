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
    

bool ESP8266_GPIO::ms_bGpioInitCalled = false;

bool ESP8266_GPIO::initPin(unsigned char pinNr, bool output)
{   
    if (!ms_bGpioInitCalled)
    {
        gpio_init();
        ms_bGpioInitCalled = true;
    }
    
    bool bRet = true;
    
    switch (pinNr) {
    case 0:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 1:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_GPIO1);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 2:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 3:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_GPIO3);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 4:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 5:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 9:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_SD_DATA2_U, FUNC_GPIO9);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 10:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_SD_DATA3_U, FUNC_GPIO10);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 12:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 13:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 14:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 15:
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_GPIO15);
      output ? gpio_output_set(0, 0, BIT(GPIO_ID_PIN(pinNr)),0) : gpio_output_set(0, 0, 0, GPIO_ID_PIN(pinNr));
      break;
    case 16:
      if (output)
      {
        // mux configuration for XPD_DCDC to output rtc_gpio0
        WRITE_PERI_REG(PAD_XPD_DCDC_CONF, (READ_PERI_REG(PAD_XPD_DCDC_CONF) & 0xffffffbcUL) | 0x1UL); 

        //mux configuration for out enable
        WRITE_PERI_REG(RTC_GPIO_CONF, (READ_PERI_REG(RTC_GPIO_CONF) & 0xfffffffeUL) | 0x0UL); 
        
        //out enable
        WRITE_PERI_REG(RTC_GPIO_ENABLE, (READ_PERI_REG(RTC_GPIO_ENABLE) & 0xfffffffeUL) | 0x1UL); 
      }
      else
      {
        // mux configuration for XPD_DCDC and rtc_gpio0 connection
        WRITE_PERI_REG(PAD_XPD_DCDC_CONF, (READ_PERI_REG(PAD_XPD_DCDC_CONF) & 0xffffffbcUL) | 0x1UL); 

        //mux configuration for out enable
        WRITE_PERI_REG(RTC_GPIO_CONF, (READ_PERI_REG(RTC_GPIO_CONF) & 0xfffffffeUL) | 0x0UL); 
        //out disable
        WRITE_PERI_REG(RTC_GPIO_ENABLE, READ_PERI_REG(RTC_GPIO_ENABLE) & 0xfffffffeUL);  
      }
      break;
    default:
      bRet = false;
      trace("ERROR in ESP8266_GPIO::initPin(%d): wrong input", pinNr);  
  }
  
  return bRet;
}


void ESP8266_GPIO::setGpioValue(unsigned char pinNr, bool value)
{
    if (16 == pinNr)
    {
        WRITE_PERI_REG(RTC_GPIO_OUT, (READ_PERI_REG(RTC_GPIO_OUT) & 0xfffffffeUL) | (0x1UL & value));
    }
    else
    {
        GPIO_OUTPUT_SET(GPIO_ID_PIN(pinNr), value ? 1:0);
    }
}


bool ESP8266_GPIO::getGpioValue(unsigned char pinNr)
{
    if (16 == pinNr)
    {
        return (READ_PERI_REG(RTC_GPIO_IN_DATA) & 1UL);
    }
    else
    {
        return (0 != GPIO_INPUT_GET(GPIO_ID_PIN(pinNr)));
    }
}
        

