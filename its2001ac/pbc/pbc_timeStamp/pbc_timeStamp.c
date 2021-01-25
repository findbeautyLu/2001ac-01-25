#include "pbc_timeStamp.h"



static uint32_t currentTimeStamp = 0;

static bool leapYearCheck(uint8_t year)
{
    if(year % 4)
    {
        return false;
    }
    else
    {
        return true;
    }
}

static bool time_legal(uint8_t *rtc)
{
    if(rtc[YEAR_ADDR] > START_YEAR)
    {
        return true;
    }
    else if(rtc[YEAR_ADDR] == START_YEAR)
    {      
        if(rtc[MONHT_ADDR] > START_MONTH)
        {
            return true;
        }
        else if(rtc[MONHT_ADDR] == START_MONTH)
        {
            if(rtc[DAY_ADDR] < START_DAY)
            {
                return false;
            }           
            else
            {
                return true;
            }   
        }
        else
        {
            return false;
        }      
    }
    else
    {
        return  false;
    }
}

static uint32_t day_timeStamp(uint8_t hour,uint8_t minute,uint8_t second)
{
    uint32_t dat_stamp    = 0;
    uint32_t hour_stamp   = 0;
    uint32_t minute_stamp = 0;
    uint32_t second_stamp = 0;
    
    hour_stamp   = hour * 36000;
    minute_stamp = minute * 600;
    second_stamp = second * 10 ;
    dat_stamp = hour_stamp + minute_stamp + second_stamp; 
    return dat_stamp;
}



void pbc_timeStamp_get_absolutely_time(uint8_t *rtc)
{
   if(!time_legal(rtc))
   {
       currentTimeStamp = 0;
   }
   else
   {
      uint8_t pass_year = 0;
      pass_year = (rtc[YEAR_ADDR] - START_YEAR);  
      uint32_t passDay = 0;
      if(pass_year)
      {
          while(pass_year > 10)
          {
              if(pass_year > 10)
              {
                  pass_year -= 10;
              }
          }
          //////////////////////过去的完整年//////////////////////
          passDay += 61;                       //2020年剩下的天数
          passDay += ((pass_year - 1) * 365) ; //完整年比如2021 结果为0  2022 结果为1 2021年为完整年 2022没有处理 
          passDay += ((pass_year - 1) >> 2)  ; //计算完整年中的闰年 20 21 22 23 24 25  2025年pass_year = 5,5 >> 2 = 1(2024闰年)
          //////////////////////过去的完整年//////////////////////
          
                   
          ////////////////////////当前年//////////////////////////
          bool leapYear = false;         //未过完年是否是闰年
          leapYear = leapYearCheck(rtc[YEAR_ADDR]);
          switch(rtc[MONHT_ADDR])
          {
              case 1 :break;                       
              case 2 :passDay += 31;break;
              case 3 :passDay += (59  + (uint8_t)leapYear);break; 
              case 4 :passDay += (90  + (uint8_t)leapYear);break; 
              case 5 :passDay += (120 + (uint8_t)leapYear);break; 
              case 6 :passDay += (151 + (uint8_t)leapYear);break; 
              case 7 :passDay += (181 + (uint8_t)leapYear);break;
              case 8 :passDay += (212 + (uint8_t)leapYear);break;
              case 9 :passDay += (243 + (uint8_t)leapYear);break;
              case 10:passDay += (273 + (uint8_t)leapYear);break;
              case 11:passDay += (304 + (uint8_t)leapYear);break;
              case 12:passDay += (334 + (uint8_t)leapYear);break;
          }
          passDay += (rtc[DAY_ADDR] - START_DAY);         
          ////////////////////////当前年//////////////////////////
      }
      else
      {
          ////////////////////2020没有过完////////////////////////
          if(rtc[MONHT_ADDR] > START_MONTH)
          {
              passDay += 30;             
          }
          passDay += (rtc[DAY_ADDR] - START_DAY);  
          ////////////////////2020没有过完////////////////////////
      }
      uint32_t debug_result = 0;                 
      debug_result = passDay * 864000;
      debug_result += day_timeStamp(rtc[HOUR_ADDR],rtc[MINUTE_ADDR],rtc[SECOND_ADDR]); 
      currentTimeStamp = debug_result;
   }     
}

void pbc_timeStamp_task(uint16_t millsecond)
{
    static uint16_t  lastTick = 0;
    static uint16_t  stoTick  = 0;
    if(currentTimeStamp)
    {
        uint16_t passTick = 0;
        passTick = millsecond - lastTick; 
        passTick += stoTick;
        if(passTick >= 100)    
        {
            uint8_t time = 0;
            uint8_t unit = 0;
            time = (uint8_t)(passTick / 100);
            unit = (uint8_t)(passTick % 100);
            currentTimeStamp  += time;
            stoTick = unit;
            lastTick = millsecond;
        }             
    }   
}

void pbc_timeStamp_100ms_run_task(void)
{
    currentTimeStamp++;
}

uint32_t pbc_timeStamp_get_stamp(void)
{
    return currentTimeStamp; 
}
