//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\snail_data_types.h"
#include "stm32f10x.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//flash map stm32f103rct6
//256k flash,smallest unit 2k
//0x0800 0000 -- 0x0803 FFFF
//boot
//0x0800 0000 -- 0x0800 1FFF     8k      4  page
//user.app 运行区
//0x0800 2000 -- 0x0801 CFFF     108k    54  page  0-57page
//user.upgrade 升级区
//0x0801 D000 -- 0x0803 7FFF     108k    54  page  58,59,60,61,62-127page 由WRP3 bit7写保护
//reserve                    
//0x0803 8000 -- 0x0803 FFFF     32k     16  page 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define user_app_start_addr      0x08002000
#define user_upgrade_start_addr  0x0801D000
#define user_app_inf_addr        0x0801CFE0
#define user_upgrade_inf_addr    0x08037FE0
#define page_unit_size           2048
#define user_upgrade_inf_page    0x08037800
//------------------------------------------------------------------------------
//名称:读取app区的信息内容32bytes
//入口:返回的数据指针

void bsp_read_information_user_app(sdt_int8u* out_pBuff)
{
    sdt_int8u *pAddr;
    sdt_int8u i;
    pAddr = (sdt_int8u*)user_app_inf_addr;
    for(i = 0;i < 32;i++)
    {
        *out_pBuff = *pAddr;
        pAddr ++;
        out_pBuff ++;
    }
}
//------------------------------------------------------------------------------
//名称:写入一个块的数据到upgrade 128bytes
//入口:块序号(从0开始)， 写入的数据指针

void bsp_write_block_user_upgrade(sdt_int16u in_block_num,sdt_int8u* in_pBuff)
{
    sdt_int32u falsh_addr;
    
    falsh_addr = user_upgrade_start_addr + in_block_num*128;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    if(0 == (falsh_addr%page_unit_size))  //2k地址
    {
        FLASHStatus = FLASH_ErasePage(falsh_addr);   //Erase page
        if(FLASHStatus == FLASH_COMPLETE)
        {
        }
        else
        {
            FLASH_Lock();
            return;
        }
    }
    
    union
    {
        sdt_int8u inf_8bits[128];
        sdt_int32u inf_32bits[32];
    }inf_data;

    sdt_int32u words_32bits;
    sdt_int32u i;

    for(i = 0;i < 128;i++)
    {
        inf_data.inf_8bits[i] = in_pBuff[i];
    }
    
    words_32bits = 32;         //128bytes 
    for(i=0;i<words_32bits;i++)
    {
        #ifdef NDEBUG
        IWDG_ReloadCounter(); //
        #endif
        FLASHStatus = FLASH_ProgramWord(falsh_addr,inf_data.inf_32bits[i]);
        falsh_addr += 4;
    }
    FLASH_Lock();
}
//------------------------------------------------------------------------------
//名称:读取upgrade区的信息内容32bytes
//入口:返回的数据指针

void bsp_read_information_user_upgrade(sdt_int8u* out_pBuff)
{
    sdt_int8u *pAddr;
    sdt_int8u i;
    pAddr = (sdt_int8u*)user_upgrade_inf_addr;
    for(i = 0;i < 32;i++)
    {
        *out_pBuff = *pAddr;
        pAddr ++;
        out_pBuff ++;
    }
}
//------------------------------------------------------------------------------
//名称:写入upgrade区的信息内容32bytes
//入口:写入的数据指针

void bsp_write_information_user_upgrade(sdt_int8u* in_pBuff)
{
    sdt_int32u falsh_addr;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    
    falsh_addr = user_upgrade_inf_page;
    FLASHStatus = FLASH_ErasePage(falsh_addr);   //Erase inf
    if(FLASHStatus == FLASH_COMPLETE)
    {
    }
    else
    {
        FLASH_Lock();
        return;
    }
    
    union
    {
        sdt_int8u inf_8bits[32];
        sdt_int32u inf_32bits[8];
    }inf_data;

    sdt_int32u upgrade_falsh_addr;
    sdt_int32u words_32bits;
    sdt_int32u i;

    for(i = 0;i < 32;i++)
    {
        inf_data.inf_8bits[i] = in_pBuff[i];
    }
    
    upgrade_falsh_addr = user_upgrade_inf_addr;
    words_32bits = 8;         //32bytes information
    for(i=0;i<words_32bits;i++)
    {
        #ifdef NDEBUG
        IWDG_ReloadCounter(); //
        #endif
        FLASHStatus = FLASH_ProgramWord(upgrade_falsh_addr,inf_data.inf_32bits[i]);
        upgrade_falsh_addr += 4;
    }
    FLASH_Lock();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++