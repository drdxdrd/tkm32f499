#ifndef __SYS_H
#define __SYS_H

#ifdef __cplusplus
extern "C" {
#endif

//#include "hal/hal_conf.h"
#include "hal/hal_misc.h"
#include "tk499.h"
//位锟斤拷锟斤拷锟斤拷,实锟斤拷51锟斤拷锟狡碉拷GPIO锟斤拷锟狡癸拷锟斤拷
//锟斤拷锟斤拷实锟斤拷思锟斤拷,锟轿匡拷<<CM3权锟斤拷指锟斤拷>>锟斤拷锟斤拷锟斤拷(87页~92页).M4同M3锟斤拷锟斤拷,只锟角寄达拷锟斤拷锟斤拷址锟斤拷锟斤拷.
// IO锟节诧拷锟斤拷锟疥定锟斤拷
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

void  RemapVtorTable(void);
void  AI_Responder_enable(void);
void  AI_Responder_disable(void);

void Sys_Soft_Reset(void);      							//系统锟斤拷锟斤拷位
void Sys_Standby(void);         							//锟斤拷锟斤拷模式 	

void TK499_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);	
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);				//锟解部锟叫讹拷锟斤拷锟矫猴拷锟斤拷(只锟斤拷GPIOA~I)

//锟斤拷锟斤拷为锟斤拷锟洁函锟斤拷
void WFI_SET(void);		//执锟斤拷WFI指锟斤拷
void INTX_DISABLE(void);//锟截憋拷锟斤拷锟斤拷锟叫讹拷
void INTX_ENABLE(void);	//锟斤拷锟斤拷锟斤拷锟斤拷锟叫讹拷

void TIM3_Config(u16 arr,u16 psc);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

