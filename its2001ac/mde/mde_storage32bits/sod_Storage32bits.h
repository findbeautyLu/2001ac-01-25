//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _SOD_STORAGE_H
#define _SOD_STORAGE_H
//------------------------------------------------------------------------------
#define max_solid    1

//参数存储块，双备份存储,适用于M3、M0芯片
//-----------------------------------------------------------------------------
static  block_storage_parameter_def block_storage_solid[max_solid];

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void block_storage_solid_cfg(void)
{
//-----------------------------------------------------------------------------
    bsp_flash_memory_cfg();
    block_storage_solid[0].passage_flash_memory_read = bsp_passage_flash_memory_read;
    block_storage_solid[0].passage_flash_memory_write = bsp_passage_flash_memory_write;
    block_storage_solid[0].passage_flash_write_complete = bsp_passage_flash_write_complete;
//-----------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
#endif
//++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++++