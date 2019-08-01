
#include "General.h"
union{
	unsigned short crc;
struct{
	uint8_t crcl;
	uint8_t crch;
}segment;
}crc16;
uint8_t masterBuffer[250]={0};
uint8_t gateBuffer[250]={0};
struct{
	uint8_t data[250];
	uint8_t pointer;
	
	
	
}GateBuffer;

struct{
	uint8_t data[250];
	uint8_t pointer;
	
	
	
}MasterBuffer;
uint8_t Usart1Connection=0;
uint8_t Usart2Connection=0;
 uint8_t Usart3Connection=0;

uint32_t ticktime=0;



void Task2_Delay(U32 Ticks)
{
		volatile U32 TimeDivision = (Ticks / (1 Sec));
		volatile U32 TimeRemaining = (Ticks % (1 Sec));
int Time=0;
		for( Time = 0; Time<TimeDivision;Time++)
		{
				os_dly_wait(1 Sec);
		}
		
		if(TimeRemaining)
		{
				os_dly_wait(TimeRemaining);
		}
}





void Task1_Delay(U32 Ticks)
{
		volatile U32 TimeDivision = (Ticks / (1 Sec));
		volatile U32 TimeRemaining = (Ticks % (1 Sec));
int Time=0;
		for( Time = 0; Time<TimeDivision;Time++)
		{
				os_dly_wait(1 Sec);
		}
		
		if(TimeRemaining)
		{
				os_dly_wait(TimeRemaining);
		}
}

void USART2_Config(void){
	
	   /* USART configuration structure for USART1 */
    USART_InitTypeDef usart2_init_struct;
    /* Bit configuration structure for GPIOA PIN9 and PIN10 */
    GPIO_InitTypeDef gpioa_init_struct;     
    /* Enalbe clock for USART1, AFIO and GPIOA */

          RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA 
                            , ENABLE);
	
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   


	/* GPIOA PIN2 alternative function Tx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_2;
  //  gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpioa_init_struct);
    /* GPIOA PIN3 alternative function Rx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_3;
    //gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpioa_init_struct);
 
    /* Enable USART1 */
    USART_Cmd(USART2, ENABLE);  
    /* Baud rate 9600, 8-bit data, One stop bit
     * No parity, Do both Rx and Tx, No HW flow control
     */
    usart2_init_struct.USART_BaudRate = 115200;   
    usart2_init_struct.USART_WordLength = USART_WordLength_8b;  
    usart2_init_struct.USART_StopBits = USART_StopBits_1;   
    usart2_init_struct.USART_Parity = USART_Parity_No ;
    usart2_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart2_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART1 */
    USART_Init(USART2, &usart2_init_struct);
    /* Enable RXNE interrupt */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(USART2_IRQn);
	
}
void USART1_IRQHandler(void) __irq
{
  if (USART_GetITStatus(USART1, USART_IT_RXNE))
 // Received characters and modify string
  {
        USART_ClearFlag(USART1, USART_IT_RXNE);
       
      MasterBuffer.data[MasterBuffer.pointer]=USART_ReceiveData(USART1);
		  ++MasterBuffer.pointer;
        Usart1Connection=1;

  }
}

void USART1_Config(void)
{
    /* USART configuration structure for USART1 */
    USART_InitTypeDef usart1_init_struct;
    /* Bit configuration structure for GPIOA PIN9 and PIN10 */
    GPIO_InitTypeDef gpioa_init_struct;

    /* Enalbe clock for USART1, AFIO and GPIOA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA  
                          , ENABLE);
                            
    /* GPIOA PIN9 alternative function Tx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_9;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpioa_init_struct);
    /* GPIOA PIN9 alternative function Rx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_10;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpioa_init_struct);
 
    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);  
    /* Baud rate 9600, 8-bit data, One stop bit
     * No parity, Do both Rx and Tx, No HW flow control
     */
    usart1_init_struct.USART_BaudRate = 115200;   
    usart1_init_struct.USART_WordLength = USART_WordLength_8b;  
    usart1_init_struct.USART_StopBits = USART_StopBits_1;   
    usart1_init_struct.USART_Parity = USART_Parity_No ;
    usart1_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart1_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART1 */
    USART_Init(USART1, &usart1_init_struct);
    /* Enable RXNE interrupt */
    
    /* Enable USART1 global interrupt */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
		NVIC_EnableIRQ(USART1_IRQn);


}




void USART3_Config(void)
{
     USART_InitTypeDef usart3_init_struct;
    /* Bit configuration structure for GPIOA PIN9 and PIN10 */
    GPIO_InitTypeDef gpioa_init_struct;
     
    /* Enalbe clock for USART1, AFIO and GPIOA */
//    // RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
         RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | 
                           RCC_APB2Periph_GPIOB, ENABLE);                       
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);   


	/* GPIOA PIN2 alternative function Tx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_10;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &gpioa_init_struct);
    /* GPIOA PIN3 alternative function Rx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_11;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &gpioa_init_struct);
 
    /* Enable USART1 */
    USART_Cmd(USART3, ENABLE);  
    /* Baud rate 9600, 8-bit data, One stop bit
     * No parity, Do both Rx and Tx, No HW flow control
     */
    usart3_init_struct.USART_BaudRate = 9600;   
    usart3_init_struct.USART_WordLength = USART_WordLength_8b;  
    usart3_init_struct.USART_StopBits = USART_StopBits_1;   
    usart3_init_struct.USART_Parity = USART_Parity_No ;
    usart3_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart3_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART1 */
    USART_Init(USART3, &usart3_init_struct);
    /* Enable RXNE interrupt */
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(USART3_IRQn);
}

// void SysTick_Handler(void) __irq{
// 	++ticktime;
// 	
// 	
// 	
// }
void delay_ms(uint16_t t){
	ticktime=0;
	
	//SysTick->LOAD=SystemCoreClock/1000;
	SysTick_Config (SystemCoreClock / 1000);
  while(1){
	 if(ticktime==t){
      ticktime=0;
			SysTick_Config(0);
    
		 break;
		}
	}

	
}





void USART2_IRQHandler(void) __irq
{
  if (USART_GetITStatus(USART2, USART_IT_RXNE))
 // Received characters and modify string
  {
//         USART_ClearFlag(USART2, USART_IT_RXNE);
//       MasterBuffer.data[MasterBuffer.pointer]=USART_ReceiveData(USART2);
// 		  ++MasterBuffer.pointer;

        Usart2Connection=1;

  }
}

unsigned short CRC16(unsigned char *puchMsg , unsigned short usDataLen)

{
unsigned char uchCRCHi = 0xFF ; /* high byte of CRC initialized */
unsigned char uchCRCLo = 0xFF ; /* low byte of CRC initialized */
unsigned uIndex ; /* will index into CRC lookup table */
while (usDataLen--) /* pass through message buffer */
{
uIndex = uchCRCHi ^ *puchMsg++ ; /* calculate the CRC */
uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
uchCRCLo = auchCRCLo[uIndex] ;
}
return (uchCRCHi << 8 | uchCRCLo) ;
}


void USART3_IRQHandler(void) __irq
{
   if (USART_GetITStatus(USART3, USART_IT_RXNE))
 // Received characters and modify string
  {
    Usart3Connection=1;
    GateBuffer.data[GateBuffer.pointer]=USART_ReceiveData(USART3);
		++GateBuffer.pointer;
    USART_ClearFlag(USART3, USART_IT_RXNE);
  }
}



void UART_SendDataString(uint8_t usart,uint8_t *str,uint8_t len){
uint8_t pointer=0;
if(usart==1){
	//uint8_t BufferSerial[200]={0};
while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){
	
};
	while(1){
		if(pointer==len) break;
		else{
			
	USART_SendData(USART1, str[pointer]);
      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {
    }
			++pointer	;		
		}		
	}

}else if(usart==2){
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){
	
};
	while(1){
		if(pointer==len) break;
		else{
			
	USART_SendData(USART2, str[pointer]);
      while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    {
    }
			++pointer	;		
		}		
	}
}
else if(usart==3){
	
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET){
};
	while(1){
		if(pointer==len) break;
		else{
	USART_SendData(USART3, str[pointer]);
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
    {
    }
  	++pointer	;		
		}		
	}
}
}


unsigned short CRC16Calculate(unsigned char *puchMsg , unsigned short usDataLen)

{
unsigned char uchCRCHi = 0xFF ; /* high byte of CRC initialized */
unsigned char uchCRCLo = 0xFF ; /* low byte of CRC initialized */
unsigned uIndex ; /* will index into CRC lookup table */
while (usDataLen--) /* pass through message buffer */
{
uIndex = uchCRCHi ^ *puchMsg++ ; /* calculate the CRC */
uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
uchCRCLo = auchCRCLo[uIndex] ;
}
return (uchCRCHi << 8 | uchCRCLo) ;
}


void PortInit(void){
	 GPIO_InitTypeDef gpioa_init_struct;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
    gpioa_init_struct.GPIO_Pin = SOLONOID1 | SOLONOID2 | BUZZER;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(EXTENAL_PORT, &gpioa_init_struct);
	GPIO_ResetBits(	EXTENAL_PORT,SOLONOID1 | SOLONOID2 | BUZZER);	
	
}

void SolonidSet(uint8_t Num){
	
	if(Num==1){
GPIO_SetBits(	EXTENAL_PORT,SOLONOID1 );	
}else if(Num==2){
	GPIO_SetBits(	EXTENAL_PORT,SOLONOID2 );	
}
//Task1_Delay(500);
//GPIO_ResetBits(	EXTENAL_PORT,SOLONOID1 | SOLONOID2);

}

void BuzzerSet(void){
	
GPIO_SetBits(	EXTENAL_PORT,BUZZER );	
//Task1_Delay(2000);
//GPIO_ResetBits(	EXTENAL_PORT,BUZZER);

}

void BuzzerClear(void){
	
//GPIO_SetBits(	EXTENAL_PORT,BUZZER );	
//Task1_Delay(2000);
GPIO_ResetBits(	EXTENAL_PORT,BUZZER);

}
	void SolonidClear(uint8_t Num){
		if(Num==1){
GPIO_ResetBits(	EXTENAL_PORT,SOLONOID1 );	
}else if(Num==2){
	GPIO_ResetBits(	EXTENAL_PORT,SOLONOID2 );	
}	
		
		
	}