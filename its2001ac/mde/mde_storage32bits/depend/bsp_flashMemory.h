//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_FLASHMEMORY_H
#define _BSP_FLASHMEMORY_H
//-----------------------------------------------------------------------------
#ifndef snail_data_types
    #include ".\snail_data_types.h"
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//Task
//------------------------------------------------------------------------------
//Event
//------------------------------------------------------------------------------
//Status
//------------------------------------------------------------------------------
//Function
void bsp_flash_memory_cfg(void);
sdt_int32u bsp_passage_flash_memory_read(block_def in_block,sdt_int32u in_offset);
void bsp_passage_flash_memory_write(block_def in_block,sdt_int32u in_offset,sdt_int32u in_data);
void bsp_passage_flash_write_complete(void);
#endif
//++++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++


