//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\mde_Relay.h"
#include "..\..\pbc\pbc_sysTick\pbc_sysTick.h"
//------------------------------E N D-------------------------------------------
typedef struct
{
    void(*relay_on)(void);
    void(*relay_off)(void);
}relay_drive_t;

typedef enum
{
    RELAY_MODE_OFF  = (uint8_t)0,
    RELAY_MODE_ON  ,
}relay_mode_t;

typedef struct
{    
    void(*relay_on)(void);
    void(*relay_off)(void);
    relay_mode_t    mode;
    bool            onEnableFlag;  
    uint16_t        onDelay;
    uint16_t        offDelay;
    uint16_t        onEnableDelay;
 }relay_t;

static void _relay_on(relay_t *me,uint16_t _in_100msOnDelay)
{
    if(RELAY_MODE_ON == me->mode)
    {
        return;
    }
    me->mode = RELAY_MODE_ON; 
    me->onDelay = _in_100msOnDelay;
}

static void _relay_off(relay_t*me,uint16_t _in_100msOffDelay)
{
    if(RELAY_MODE_OFF == me->mode)
    {
        return;
    }
    else
    {
        me->onEnableDelay = RELAY_RECLOSE_TIME;
        me->onEnableFlag = false;
    }
    me->mode = RELAY_MODE_OFF; 
    me->offDelay = _in_100msOffDelay; 
}

static void _relay_run_handle(relay_t*me)
{  
        if(RELAY_MODE_ON == me->mode)
        {
            if(me->onDelay > 0)
            {
                me->onDelay--;
            }
            else
            {
                if(me->onEnableFlag)
                {
                    me->relay_on();
                }
            }
        }
        else if(RELAY_MODE_OFF == me->mode)
        {
            if(me->offDelay > 0)
            {
                me->offDelay--;
            }
            else
            {
                me->relay_off();
            }
        }
        if(me->onEnableDelay > 0)
        {
           me->onEnableDelay--; 
        }
        else
        {
            me->onEnableFlag = true;
        }
     
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\sod_Relay.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mde_relay_on(uint8_t in_solidNum,uint16_t _in_100msOnDelay)
{
    if(in_solidNum < max_solid)
    {
        _relay_on(&relay[in_solidNum],_in_100msOnDelay);
    }
    else
    {
        while(1);
    }
}


void mde_relay_off(uint8_t in_solidNum,uint16_t _in_100msOffDelay)
{
    if(in_solidNum < max_solid)
    {
        _relay_off(&relay[in_solidNum],_in_100msOffDelay);
    }
    else
    {
        while(1);
    }
}

void mde_relay_task(void)
{
    static bool  cfged = false;
    static uint8_t i = 0;
	macro_createTimer(measure_delay,timerType_millisecond,100);
    pbc_timerClockRun_task(&measure_delay);
	if(cfged)
    {
        if(pbc_pull_timerIsCompleted(&measure_delay))
        {
            pbc_reload_timerClock(&measure_delay,100);
            for(i = 0; i < max_solid;i++)
            {
                _relay_run_handle(&relay[i]);
            }
        }
    }
    else
    {
        cfged = true;
        mde_relay_solid_cfg();
    }
}
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************继电器扫描函数**************************************
//------------------------------E N D-------------------------------------------



