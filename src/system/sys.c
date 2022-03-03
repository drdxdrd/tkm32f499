#include "sys.h"

void RemapVtorTable(void) {
  int i;
  RCC->AHB1ENR |=
      1 << 13;  // bkp clk,enable sram
                // Kunjin copy ROM Kunjin copy Kun called corrupt copy
  for (i = 0; i < 90; i++) {
    NVIC_DisableIRQ((IRQn_Type)i);
  }
  SCB->VTOR = 0;
  SCB->VTOR |= 0x1 << 29;
  for (i = 0; i < 512; i += 4) {
    *(u32*)(T_SRAM_BASE + i) = *(u32*)(T_SDRAM_BASE + i);
  }
}

void  AI_Responder_enable(void)
{
  AI_Responder->ADDR1= 0x70807040;
//	AI_Responder->ADDR0 = 0x70027080;
  AI_Responder->ADDR1 = 0;
  AI_Responder->CCR &= ~(0x3<<3);
  AI_Responder->CCR |= 1;
  while((AI_Responder->SR & 0x3) != 2);
}

void  AI_Responder_disable(void)
{
    AI_Responder->CCR &= ~1;
}


//锟解部锟叫讹拷锟斤拷锟矫猴拷锟斤拷
//只锟斤拷锟斤拷GPIOA~I;锟斤拷锟斤拷锟斤拷PVD,RTC,USB_OTG,USB_HS,锟斤拷太锟斤拷锟斤拷锟窖碉拷
//锟斤拷锟斤拷:
//GPIOx:0~8,锟斤拷锟斤拷GPIOA~I
//BITx:锟斤拷要使锟杰碉拷位;
//TRIM:锟斤拷锟斤拷模式,1,锟斤拷锟斤拷锟斤拷;2,锟较斤拷锟斤拷;3锟斤拷锟斤拷锟斤拷锟斤拷平锟斤拷锟斤拷
//锟矫猴拷锟斤拷一锟斤拷只锟斤拷锟斤拷锟斤拷1锟斤拷IO锟斤拷,锟斤拷锟斤拷IO锟斤拷,锟斤拷锟斤拷锟轿碉拷锟斤拷
//锟矫猴拷锟斤拷锟斤拷锟皆讹拷锟斤拷锟斤拷锟斤拷应锟叫讹拷,锟皆硷拷锟斤拷锟斤拷锟斤拷   	    
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{ 
	u8 EXTOFFSET=(BITx%4)*4;  
	RCC->APB2ENR|=1<<14;  						//使锟斤拷SYSCFG时锟斤拷  
	SYSCFG->EXTICR[BITx/4]&=~(0x000F<<EXTOFFSET);//锟斤拷锟斤拷原锟斤拷锟斤拷锟矫ｏ拷锟斤拷锟斤拷
	SYSCFG->EXTICR[BITx/4]|=GPIOx<<EXTOFFSET;	//EXTI.BITx映锟戒到GPIOx.BITx 
	//锟皆讹拷锟斤拷锟斤拷
	EXTI->IMR|=1<<BITx;					//锟斤拷锟斤拷line BITx锟较碉拷锟叫讹拷(锟斤拷锟斤拷要锟斤拷止锟叫断ｏ拷锟津反诧拷锟斤拷锟斤拷锟斤拷)
	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;	//line BITx锟斤拷锟铰硷拷锟铰斤拷锟截达拷锟斤拷
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;	//line BITx锟斤拷锟铰硷拷锟斤拷锟斤拷锟斤拷锟截达拷锟斤拷
} 	


//THUMB指锟筋不支锟街伙拷锟斤拷锟斤拷锟斤拷
//锟斤拷锟斤拷锟斤拷锟铰凤拷锟斤拷实锟斤拷执锟叫伙拷锟斤拷指锟斤拷WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//锟截憋拷锟斤拷锟斤拷锟叫讹拷(锟斤拷锟角诧拷锟斤拷锟斤拷fault锟斤拷NMI锟叫讹拷)
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}
//锟斤拷锟斤拷锟斤拷锟斤拷锟叫讹拷
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//锟斤拷锟斤拷栈锟斤拷锟斤拷址  __set_MSP(0x70002000);

//锟斤拷锟斤拷锟斤拷锟斤拷模式	  
void Sys_Standby(void)
{ 
	SCB->SCR|=1<<2;		//使锟斤拷SLEEPDEEP位 (SYS->CTRL)	   
	RCC->APB1ENR|=1<<28;//使锟杰碉拷源时锟斤拷 
	PWR->CSR|=1<<8;     //锟斤拷锟斤拷WKUP锟斤拷锟节伙拷锟斤拷
	PWR->CR|=1<<2;      //锟斤拷锟斤拷Wake-up 锟斤拷志
	PWR->CR|=1<<1;      //PDDS锟斤拷位   	
	WFI_SET();			//执锟斤拷WFI指锟斤拷,锟斤拷锟斤拷锟斤拷锟斤拷模式		 
}	     
//系统锟斤拷锟斤拷位   
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 		 
   
//		TK499_NVIC_Init(2,2,TK80_IRQn,2);
//锟斤拷锟斤拷NVIC 
//NVIC_PreemptionPriority:锟斤拷占锟斤拷锟饺硷拷
//NVIC_SubPriority       :锟斤拷应锟斤拷锟饺硷拷
//NVIC_Channel           :锟叫断憋拷锟斤拷
//NVIC_Group             :锟叫断凤拷锟斤拷 0~4
//注锟斤拷锟斤拷锟饺硷拷锟斤拷锟杰筹拷锟斤拷锟借定锟斤拷锟斤拷锟侥凤拷围!锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟诫不锟斤拷锟侥达拷锟斤拷
//锟介划锟斤拷:
//锟斤拷0:0位锟斤拷占锟斤拷锟饺硷拷,4位锟斤拷应锟斤拷锟饺硷拷
//锟斤拷1:1位锟斤拷占锟斤拷锟饺硷拷,3位锟斤拷应锟斤拷锟饺硷拷
//锟斤拷2:2位锟斤拷占锟斤拷锟饺硷拷,2位锟斤拷应锟斤拷锟饺硷拷
//锟斤拷3:3位锟斤拷占锟斤拷锟饺硷拷,1位锟斤拷应锟斤拷锟饺硷拷
//锟斤拷4:4位锟斤拷占锟斤拷锟饺硷拷,0位锟斤拷应锟斤拷锟饺硷拷
//NVIC_SubPriority锟斤拷NVIC_PreemptionPriority锟斤拷原锟斤拷锟斤拷,锟斤拷值越小,越锟斤拷锟斤拷	   
void TK499_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{
	u32 temp;	  
	NVIC_SetPriorityGrouping(NVIC_Group);//锟斤拷锟矫凤拷锟斤拷
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//取锟斤拷锟斤拷位
	NVIC->ISER[NVIC_Channel/32]|=1<<NVIC_Channel%32;//使锟斤拷锟叫讹拷位(要锟斤拷锟斤拷锟侥伙拷,锟斤拷锟斤拷ICER锟斤拷应位为1锟斤拷锟斤拷)
	NVIC->IP[NVIC_Channel]|=temp<<4;				//锟斤拷锟斤拷锟斤拷应锟斤拷锟饺硷拷锟斤拷锟斤拷锟斤拷锟斤拷锟饺硷拷   	    	  				   
}

void TK80_IRQHandler(void)
{
    if(TK80->SR & 0x1)
    {
    
    }
    if(TK80->SR & 0x2)
    {
    
    }
    if(TK80->SR & 0x4)
    {
    
    }
    if(TK80->SR & 0x8)
    {
    
    }
    TK80->SR |= 0;
}

//锟斤拷锟矫猴拷锟斤拷
//#define T_SRAM_FUN1 0x20000400
//copyAtoB((u32)LCD_PutPixel&0xFFFFFFFE,T_SRAM_FUN1,800);//锟斤拷锟截猴拷锟斤拷锟斤拷SRAM
//void copyAtoB(u32 srcAdd,u32 dstAdd,u16 len)
//{
//    len = (len + 3)/4;
//    while(len--)
//    {
//        *(u32*)dstAdd = *(u32*)srcAdd;
//        dstAdd += 4 ;
//        srcAdd +=4 ;
//    }
//}














