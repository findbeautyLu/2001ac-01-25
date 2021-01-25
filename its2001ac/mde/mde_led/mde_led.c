//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\mde_Led.h"
#include ".\stdio.h"
#include "..\..\pbc\pbc_sysTick\pbc_sysTick.h"
//------------------------------E N D-------------------------------------------
typedef enum
{  
    LED_OFF  =  ((uint8_t)0x00), //关闭
    LED_ON,                      //开启
    LED_BLINK,                   //闪烁
}led_mode_t;

typedef struct
{  
  	 /*hal*/	
     void(*led_on)(void);    
     void(*led_off)(void);
	 /*parameter*/
	 bool             ledonFlag;
     led_mode_t 	  mode;
    // uint16_t         handleTickCfg;
	 uint16_t         ledOnTimesCfg;
	 uint16_t         ledOffTimesCfg;
	 uint16_t 		  ledOnTimes;
     uint16_t 		  ledOffTimes;
     uint16_t 		  ledBlinkDelayTimes;
 }led_device_t;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\sod_Led.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*! \brief   
 *! \param  
 *! \retval  
 *! \retval  
 */
static void led_set(uint8_t in_solidNum, led_mode_t mode,\
                 uint16_t ledOnTimes,uint16_t ledOffTimes)
{
    /*avoid set the same mode*/    
    if((LED_BLINK == mde_led[in_solidNum].mode)
        &&(ledOnTimes == mde_led[in_solidNum].ledOnTimesCfg)
            &&(ledOffTimes == mde_led[in_solidNum].ledOffTimesCfg))
    {
        return;
    }                 
    /**/                 
    mde_led[in_solidNum].mode = mode;    
    if(LED_OFF==mde_led[in_solidNum].mode)
    {
       mde_led[in_solidNum].ledonFlag = false;
    }
    else if(LED_ON==mde_led[in_solidNum].mode)
    {
       mde_led[in_solidNum].ledonFlag = true;    
    }
    else if(LED_BLINK==mde_led[in_solidNum].mode)
    {
        mde_led[in_solidNum].ledonFlag = true;   
        mde_led[in_solidNum].ledOnTimesCfg = ledOnTimes;
        mde_led[in_solidNum].ledOffTimesCfg = ledOffTimes; 
        mde_led[in_solidNum].ledOnTimes = ledOnTimes;
        mde_led[in_solidNum].ledOffTimes = ledOffTimes;
     }    
}

 /*! \brief   
 *! \param  
 *! \retval  
 *! \retval  
 */
static void led_run_handle(uint8_t in_solidNum)
{
    if(LED_BLINK==mde_led[in_solidNum].mode)
    {   
	    if(mde_led[in_solidNum].ledonFlag)
        {
		    mde_led[in_solidNum].led_on();
			if(mde_led[in_solidNum].ledOnTimes > 0)
            {
				mde_led[in_solidNum].ledOnTimes --; 
			}  
			else
            {
				mde_led[in_solidNum].ledonFlag = false;
				mde_led[in_solidNum].ledOffTimes = mde_led[in_solidNum].ledOffTimesCfg;
			}
		}		
		else
        {
			mde_led[in_solidNum].led_off();
			if(mde_led[in_solidNum].ledOffTimes > 0)
            {
				mde_led[in_solidNum].ledOffTimes --; 
			}  
			else
            {
				mde_led[in_solidNum].ledonFlag = true;
				mde_led[in_solidNum].ledOnTimes = mde_led[in_solidNum].ledOnTimesCfg;
			}
		} 
        if(mde_led[in_solidNum].ledBlinkDelayTimes> 0)
        {
            mde_led[in_solidNum].ledBlinkDelayTimes--;
            if(mde_led[in_solidNum].ledBlinkDelayTimes == 0)
            {
               mde_led[in_solidNum].mode = LED_OFF;
            }
        }
    }
    else if(LED_OFF==mde_led[in_solidNum].mode)
    {
        mde_led[in_solidNum].led_off();
		mde_led[in_solidNum].ledonFlag = false;
    }
    else if(LED_ON==mde_led[in_solidNum].mode)
    {
        mde_led[in_solidNum].led_on();
		mde_led[in_solidNum].ledonFlag = true;
    }   
}

//static bool led_return_blink_mode(uint8_t in_solidNum)
//{
//    if(mde_led[in_solidNum].mode == LED_BLINK)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//
//static void led_biink_delay_set(uint8_t in_solidNum,uint16_t ledBlinkDelayTimes)
//{
//    mde_led[in_solidNum].ledBlinkDelayTimes = ledBlinkDelayTimes;
//}

void mde_led_on(uint8_t in_solidNum)
{
    if(in_solidNum < max_solid)
    {
        led_set(in_solidNum,LED_ON,0,0);
    }
    else
    {
        while(1);
    }
}

void mde_led_off(uint8_t in_solidNum)
{
    if(in_solidNum < max_solid)
    {
        led_set(in_solidNum,LED_OFF,0,0);
    }
    else
    {
        while(1);
    }
}

void mde_led_blink(uint8_t in_solidNum,uint16_t _in_100msOnDelay,uint16_t _in_100msOffDelay)
{
    if(in_solidNum < max_solid)
    {
        led_set(in_solidNum,LED_BLINK,_in_100msOnDelay,_in_100msOffDelay);
    }
    else
    {
        while(1);
    }
}

void mde_led_task(void)
{
    uint8_t i = 0;
    static bool cfged = false;
    macro_createTimer(measure_delay,timerType_millisecond,100);
    pbc_timerClockRun_task(&measure_delay);
	if(cfged)	
    {
        if(pbc_pull_timerIsCompleted(&measure_delay))
        {
            pbc_reload_timerClock(&measure_delay,100);
            for(i = 0; i < max_solid;i++)
            {
                led_run_handle(i);
            }	
        }
    }
    else
    {
        cfged = true;
        mde_led_solid_cfg();
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


