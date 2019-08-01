#include "Master.h"
extern OS_MUT mutex1;



extern struct{
	uint8_t data[250];
	uint8_t pointer;
	
}MasterBuffer;


union{
	unsigned short crc;
struct{
	uint8_t crcl;
	uint8_t crch;
}segment;
}Crc16;
 uint8_t *UsartConnection_p;
extern uint8_t Usart2Connection;
extern uint8_t Usart1Connection;





void MasterInit(void){
USART1_Config();	
USART2_Config();
UsartConnection_p=&Usart1Connection;
//UsartConnection_p=&Usart2Connection;	
}
	


	void MASTER_ClearBuffer(void){
	memset(&MasterBuffer.data[0],0,250);	
	MasterBuffer.pointer=0;
		
		
		
	}


uint8_t MASTER_GetData(void){
uint8_t temBuffer[50]={0};
uint8_t crcPointer=0;
if(*UsartConnection_p==1){
        Task2_Delay(100);
        *UsartConnection_p=0;
        if(MasterBuffer.data[ADDR_P]==ADDRESS && MasterBuffer.data[LENGHT_P]>0 &&
       	MasterBuffer.data[LENGHT_P]<255){
        crcPointer=LENGHT_P+MasterBuffer.data[LENGHT_P]+1;
        Crc16.crc=CRC16Calculate(&MasterBuffer.data[FUNCTION_P],MasterBuffer.data[LENGHT_P]+LENGHT_P);
        if(Crc16.segment.crcl== MasterBuffer.data[crcPointer] && Crc16.segment.crch== MasterBuffer.data[crcPointer+1] )
        {
//         memset(&MasterData[0],0,100);
// 				memcpy(&MasterData[0],&SERIAL_Buffer.Buffer[0],SERIAL_Buffer.P_Buffer);
//         UART_ClearBuffer();
        return 1;
        }	
        else
        {
        	//UART_ClearBuffer();
         	//return 0;
        return 1;
        }	
        }
        //UART_ClearBuffer();
        return 0;

}	
	
	
	
	// UART_ClearBuffer();
	return 0;
}
	void MUDBUS_SendData(uint8_t *d,uint8_t len){
	uint8_t modbusdata[100]={0};	
	uint8_t pointer=0;
	modbusdata[ADDR_P]=ADDRESS;
	++pointer;
	modbusdata[FUNCTION_P]=GETDATA;
	++pointer;
	modbusdata[LENGHT_P]=len;
pointer=3;
	memcpy(&modbusdata[pointer],d,len);
	pointer +=len;
	Crc16.crc=CRC16Calculate(&modbusdata[1],pointer-1);
	modbusdata[pointer]=Crc16.segment.crcl;
  modbusdata[pointer+1]=Crc16.segment.crch;
  pointer +=2;
	MASTER_SendData(&modbusdata[0],pointer)	;
 	}


void SendError(uint8_t err,uint8_t cmd,uint8_t len){
// 	uint8_t modbusdata[100]={0};	
// 	uint8_t pointer=0;
// 	modbusdata[ADDR_P]=ADDRESS;
// 	++pointer;
// 	modbusdata[FUNCTION_P]=GETDATA;
// 	++pointer;
// 	modbusdata[LENGHT_P]=len;
// pointer=3;
// 	memcpy(&modbusdata[pointer],d,len);
// 	pointer +=len;
// 	Crc16.crc=CRC16Calculate(&modbusdata[1],pointer-1);
// 	modbusdata[pointer]=Crc16.segment.crcl;
//   modbusdata[pointer+1]=Crc16.segment.crch;
//   pointer +=2;
// 	MASTER_SendData(&modbusdata[0],pointer)	;
	
	
	
}

	uint8_t MASTER_SendData(uint8_t *p,uint8_t len){
	//os_mut_wait(&mutex1,0xFFF);
	MASTER_ClearBuffer();
	if(len==0)
		UART_SendDataString(1,&p[0],strlen((char *)&p[0]));
		else
		UART_SendDataString(1,&p[0],len);	
		//os_mut_release(&mutex1);	
    //MasterWithGateBuff
	}