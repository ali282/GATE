#include "Gate.h"
extern OS_MUT mutex1;
extern struct{
	uint8_t data[250];
	uint8_t pointer;
	
	
	
}GateBuffer;

extern uint8_t Usart3Connection;
	uint8_t RTX_ErrorFlag=0;
  uint8_t	resultXOR=0;;	
	uint8_t registerChanged=0;
	bool rtxError=false;
	uint8_t gateEngaged=0;
	uint8_t RTX_ErrorFlag_Alarm=false;
	uint8_t counter_Buff_GateA[4]={0};
  uint8_t counter_Buff_GateB[4]={0};
	uint8_t checkSumHigh =0;
  uint8_t checkSumLow=0;
  uint8_t bufGateA[15]={0};
  uint8_t bufGateB[15]={0};
  uint8_t bufGate[200]={0};
  uint8_t buffGate_Out[250]={0};
	unsigned char alarm_Buff_Gate[20]={0};
	
	
	
void Gate_Init(void){

// if(GateClearBuffer()){
// }
GateClearBuffer();
	RS_485Init();
	RS_485TXState(0);
}	
	


uint8_t GateClearBuffer(void){

memset(&GateBuffer.data[0],0,250);
GateBuffer.pointer=0;
}	


uint8_t ResponseGate(uint8_t *p){
memcpy(p,&GateBuffer.data[0],GateBuffer.pointer);	
Usart3Connection=0;
}

unsigned char    muchar_XorFunctionCheck(unsigned char *pstr,
                                                unsigned char size)
{
    unsigned char   i, resultXOR=0;
    for(i=0 ; i<(size);i++)
    {
        if((i !=(size-2)) && (i!=(size-3)))//for chacking that array is bigger as 3 BYTE
        resultXOR=(resultXOR^pstr[i]);
    }
    return resultXOR;


}




void   mvoid_GetAlarms(void)
{
    long int i=0;
    unsigned char recive_byte=0;
    unsigned char d1,d2,data;
    mvoid_RegisterRequest(4);//request reg 4 alarm
if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
	  memset(&bufGate[0],0,40);
	ResponseGate(&bufGate[0]);
}  
recive_byte=GateBuffer.pointer-1;	
GateClearBuffer();
    
//--------------------------------------------------
    d2= d1= muchar_XorFunctionCheck(&bufGate[0],recive_byte);
    d2=(d2&0x0f);///////////////////////FormatingCheckSumNibleFor//////////////
    d1=(d1&0xf0);
    d1=(d1>>4);
    d1=(0xa0|d1);
    d2=(0xb0|d2);//////////////////////////////////////////////////////////////
    //--------------------------------------------------
    if(d1==bufGate[recive_byte-3] && d2==bufGate[recive_byte-2])
    {
        //-----------------------------------------------------
        d1=bufGate[recive_byte-7];
        d2=bufGate[recive_byte-6];
        mvoid_DecodeDataPortocl(data,d1,d2);///////////////////////////////////
        alarm_Buff_Gate[0]=data;
        //-----------------------------------------------------
        d1=bufGate[recive_byte-5];
        d2=bufGate[recive_byte-4];
        mvoid_DecodeDataPortocl(data,d1,d2);///////////////////////////////////
        alarm_Buff_Gate[1]=data;
        //-----------------------------------------------------
        alarm_Buff_Gate[0]&=0x18;//////////////////Why????????????
        alarm_Buff_Gate[1]&=0x0f;
        //-----------------------------------------------------
        RTX_ErrorFlag_Alarm=0;
    }
    else
        RTX_ErrorFlag_Alarm=1;
}
unsigned char   mbool_Poll(void)
{
    //__disable_interrupt ();
	uint8_t datapoll[1]={0};

    long int i=0;
    unsigned char recive_byte=0;
    unsigned char d;
    bufGate[0]=0;
    bufGate[1]=0;
datapoll[0]=0x01;
GateClearBuffer();
	Usart3Connection=0;
RS485_DataSend(&datapoll[0],1);
if(WaitForGateResponse(1000)){
	  ResponseGate(&bufGate[0]);
	 // UART_SendDataString(3,&bufGate[0],strlen((char *)&bufGate[0]));
	GateClearBuffer();
    d=bufGate[1];
    d&=0x01;
    if(d)
        registerChanged=1;
    else
        registerChanged=0;
    d=bufGate[1];
    d&=0x10;
    if(d)
        gateEngaged=1;
    else
        gateEngaged=0;
    d=bufGate[1];
    d&=0x02;
    if(d)
    rtxError=true;
    else
     rtxError=false;
    d=bufGate[0];
    if(d==0x41)
        return  1;
    else
        return  0;
	}else {
		
		return 0;
	}
}


bool WaitForGateResponse(uint16_t TimeOut){
	
	while(!Usart3Connection){
		Task1_Delay(1 MSec);
		--TimeOut;
		if(!TimeOut) return false;
	}
	Task1_Delay(200 MSec);  ////wait for fill buffer
	return true;
}


void  mvoid_ComputeCheckSum( unsigned char dataIn1 , unsigned char dataIn2)
{
    resultXOR =(dataIn1^dataIn2);
    dataIn1=resultXOR;
    dataIn2=resultXOR;
    dataIn1=(dataIn1&0xf0);
    dataIn2=(dataIn2&0x0f);
    dataIn1=(dataIn1>>4);
    checkSumHigh=(dataIn1 | 0xa0);
    checkSumLow=(dataIn1 | 0xb0);
}


void mvoid_ComputeCheckSum_String(unsigned char n ,unsigned char *string)
{
   volatile unsigned char i,d1,d2;
    resultXOR=0;
    for (i=0 ; i<n ; i++)
        resultXOR= ( string[i]^ resultXOR );
    d1=resultXOR;
    d2=resultXOR;
    d1=(d1&0xf0);///////////////////////FormatingCheckSumNible////////////////////
    d2=(d2&0x0f);
    d1=(d1>>4);
    checkSumHigh=(d1 | 0xa0);
    checkSumLow=(d2 | 0xb0);//////////////////////////////////////////////////////
}

void  mvoid_EncodeDataPortocl(unsigned char data,unsigned char &d1,unsigned char &d2)
{
    d1=(data&0xf0);
    d2=(data&0x0f);
    d1=(d1>>4);
    d1=(d1 | 0x80);
    d2=(d2 | 0x80);
}



void  mvoid_DecodeDataPortocl(unsigned char &data,unsigned char var1,unsigned char var2)
{
unsigned char test=0;
        test=var1;
    var1 =(test&0x0f);
    test=0;
    test=var2;
    var2 =(test&0x0f);
    var1=(var1<<4);

    data=(unsigned char)(var1|var2);
}
void  mvoid_EncodeRegPortocl(unsigned char address,unsigned char &r1,unsigned char &r2)
{
    r1=(address&0xf0);/////////////FormatingRegNumberNible//////////////
    r2=(address&0x0f);
    r1=(r1>>4);
    r1=(r1 | 0x90);
    r2=(r2 | 0x90);/////////////////////////////////////////////////////
}

void    mvoid_SetOneRegister( uint8_t address ,uint8_t data)
{
 unsigned char  d1,d2;

	  Usart3Connection=0;
memset(&buffGate_Out[0],0,100);
    buffGate_Out[0]=0x21;
    buffGate_Out[1]=0x60;
    mvoid_EncodeRegPortocl(address,d1,d2);
    //---------------------
    buffGate_Out[2]=d1;
    buffGate_Out[3]=d2;
    //----------------
    mvoid_EncodeDataPortocl(data,d1,d2);//////////////////////////////////////
    //----------------
    buffGate_Out[4]=d1;
    buffGate_Out[5]=d2;
    buffGate_Out[6]=0xc0;
    mvoid_ComputeCheckSum_String( 7 ,  buffGate_Out);
    buffGate_Out[6]=checkSumHigh;
    buffGate_Out[7]=checkSumLow;
    buffGate_Out[8]=0xc0;
   
	RS485_DataSend(&buffGate_Out[0],9);
// 	RS_485TXState(1);
// 	UART_SendDataString(3,&buffGate_Out[0],strlen((char *)&buffGate_Out[0]));
// RS_485TXState(0);
}

void   mvoid_SetRegisters( uint8_t address ,  uint8_t *pdata ,uint8_t n_byte)
{
//   unsigned char d1,d2;
//     int counter;
// 	uint8_t pointer=0;
// 		  	GateClearBuffer();
// 	  Usart3Connection=0;
//   memset(&buffGate_Out[0],0,100);  
// 	buffGate_Out[pointer++]=0x21;
//     buffGate_Out[pointer++]=0x60;
//     //----------------------
//     mvoid_EncodeRegPortocl(address,d1,d2);
//     //---------------------
//     buffGate_Out[pointer++]=d1;
//     buffGate_Out[pointer++]=d2;
//     for(counter=0;counter<n_byte;counter++)
//     {
//         d1=(pdata[counter]&0xf0);///////////////////////FormatingDataNible//////////////
//         d2=(pdata[counter]&0x0f);
//         d1=(d1>>4);
//         d1=(d1 | 0x80);
//         d2=(d2 | 0x80);////////////////////////////////////////////////////
//         
// 			buffGate_Out[pointer+(counter*2)]=d1;
//         buffGate_Out[pointer+1+(counter*2)]=d2;
//     }
//     buffGate_Out[pointer+(n_byte*2)]=0xc0;
// 		
//     mvoid_ComputeCheckSum_String(pointer+(n_byte*2),buffGate_Out);
//     buffGate_Out[pointer+(n_byte*2)]=checkSumHigh;
// 		pointer++;
//     buffGate_Out[pointer+(n_byte*2)]=checkSumLow;
// 		pointer++;
//     buffGate_Out[pointer+(n_byte*2)]=0xc0;
// 		pointer+=(n_byte*2);
// RS485_DataSend(&buffGate_Out[0],pointer);


  unsigned char d1,d2;
    int counter;
    buffGate_Out[0]=0x21;
    buffGate_Out[1]=0x60;
    //----------------------
    mvoid_EncodeRegPortocl(address,d1,d2);
    //---------------------
    buffGate_Out[2]=d1;
    buffGate_Out[3]=d2;
    for(counter=0;counter<n_byte;counter++)
    {
        d1=(pdata[counter]&0xf0);///////////////////////FormatingDataNible//////////////
        d2=(pdata[counter]&0x0f);
        d1=(d1>>4);
        d1=(d1 | 0x80);
        d2=(d2 | 0x80);////////////////////////////////////////////////////
        buffGate_Out[4+(counter*2)]=d1;
        buffGate_Out[5+(counter*2)]=d2;
    }
    buffGate_Out[4+(n_byte*2)]=0xc0;
    mvoid_ComputeCheckSum_String(5+(n_byte*2),buffGate_Out);
    buffGate_Out[4+(n_byte*2)]=checkSumHigh;
    buffGate_Out[5+(n_byte*2)]=checkSumLow;
    buffGate_Out[6+(n_byte*2)]=0xc0;
     RS485_DataSend(&buffGate_Out[0],7+(n_byte*2));
		//Usart -> mvoid_Send(7+(n_byte*2) , buffGate_Out); //send string

}



void  mvoid_RegisterRequest(unsigned char address)
{
 unsigned char d1,d2;
		  	GateClearBuffer();
	  Usart3Connection=0;
	memset(&buffGate_Out[0],0,100);
    buffGate_Out[0]=0x21;
    buffGate_Out[1]=0x70;
    mvoid_EncodeRegPortocl(address,d1,d2);
    buffGate_Out[2]=d1;
    buffGate_Out[3]=d2;
    buffGate_Out[4]=0xc0;
    mvoid_ComputeCheckSum_String( 5 ,buffGate_Out);
    buffGate_Out[4]=checkSumHigh;
    buffGate_Out[5]=checkSumLow;
    buffGate_Out[6]=0xc0;
RS485_DataSend(&buffGate_Out[0],0);
	//   RS_485TXState(1);  
// 	UART_SendDataString(3,&buffGate_Out[0],strlen((char *)&buffGate_Out[0]));
//   RS_485TXState(0); 
	// Usart -> mvoid_Send(7 , buffGate_Out); //send string
   // DELAY_MS(40);
}


void    mvoid_GetCounter(uint8_t whichGate)
{

    memset(&bufGateA[0],0,15);
    memset(&bufGateB[0],0,15);
    
    unsigned char d1,d2,data=0;
    unsigned char recive_byte=0;
  uint32_t test_counterA=0;
  uint32_t test_counterB=0;
 	GateClearBuffer();
	Usart3Connection=0;
  mvoid_RegisterRequest((unsigned char)whichGate);    // REG 6 or 7 counterA or B

    if(whichGate==GS_A )
    {
if(WaitForGateResponse(1000)){
	  ResponseGate(&bufGateA[0]);
	  GateClearBuffer();
   
    d1=(bufGateA[12]&0xf0);
    d2=(bufGateA[13]&0xf0);
    d1 |=(d2>>4);
    //if(d1==bufGateA[recive_byte-3] && d2==bufGateA[recive_byte-2])//this condition consider the VERITY  of Transmision
    if(d1==0xab &&  bufGateA[2]==0x90 && bufGateA[3]==0x96)

    {
        //-----------------------------------------------------

 memset(&counter_Buff_GateA[0],0,4);
        d1=bufGateA[4];//with This Command Max Of Length must be 12Byte
        d2=bufGateA[5];
        mvoid_DecodeDataPortocl(data,d1,d2);///////////////////////////////////
        counter_Buff_GateA[0]=data;
        //-----------------------------------------------------
        d1=bufGateA[6];
        d2=bufGateA[7];
        mvoid_DecodeDataPortocl(data,d1,d2);///////////////////////////////////
        counter_Buff_GateA[1]=data;
        //-----------------------------------------------------
        d1=bufGateA[8];
        d2=bufGateA[9];
        mvoid_DecodeDataPortocl(data,d1,d2);///////////////////////////////////
        counter_Buff_GateA[2]=data;
        //-----------------------------------------------------
        d1=bufGateA[10];
        d2=bufGateA[11];
        mvoid_DecodeDataPortocl(data,d1,d2);///////////////////////////////////
        counter_Buff_GateA[3]=data;
   }
 }
    }//recive_byte[0] starting byteAdressOfSlave & recive_byte[-2,-3] checkSum byte
        if(whichGate==GS_B)
   {
   if(WaitForGateResponse(1000)){
	  ResponseGate(&bufGateB[0]);
	  
		 GateClearBuffer();
    d1=(bufGateB[12]&0xf0);
    d2=(bufGateB[13]&0xf0);
    d1 |=(d2>>4);
    //if(d1==bufGateA[recive_byte-3] && d2==bufGateA[recive_byte-2])//this condition consider the VERITY  of Transmision
    if(d1==0xab &&  bufGateB[2]==0x90 && bufGateB[3]==0x97)

    {
        //-----------------------------------------------------
        memset(&counter_Buff_GateB[0],0,4);

        d1=bufGateB[4];//with This Command Max Of Length must be 12Byte
        d2=bufGateB[5];
        mvoid_DecodeDataPortocl(data,d1,d2);///////////////////////////////////
        counter_Buff_GateB[0]=data;
        //-----------------------------------------------------
        d1=bufGateB[6];
        d2=bufGateB[7];
        mvoid_DecodeDataPortocl(data,d1,d2);///////////////////////////////////
        counter_Buff_GateB[1]=data;
        //-----------------------------------------------------
        d1=bufGateB[8];
        d2=bufGateB[9];
        mvoid_DecodeDataPortocl(data,d1,d2);///////////////////////////////////
        counter_Buff_GateB[2]=data;
        //-----------------------------------------------------
        d1=bufGateB[10];
        d2=bufGateB[11];
        mvoid_DecodeDataPortocl(data,d1,d2);///////////////////////////////////
        counter_Buff_GateB[3]=data;

  }
}
    }

}


void RS485_DataSend(uint8_t *data,uint8_t lenght){
	//	GateClearBuffer();
	Usart3Connection=0;
  RS_485TXState(1);  
	os_mut_wait(&mutex1,0xFFF);
	if(lenght==0)
	UART_SendDataString(3,data,strlen((char *)&data[0]));
  else
	UART_SendDataString(3,data,lenght);	
	RS_485TXState(0); 
os_mut_release(&mutex1);

}

void   mvoid_SetConfig(uint8_t Mode)
{
    long int i=0;
    uint8_t  d1,d2,d3;
    uint8_t recive_byte=0;
    bufGate[0]=0;
    bufGate[1]=0;
  	GateClearBuffer();
	  Usart3Connection=0;
    switch ( (uint8_t)Mode )
    {//begin switch
    case  GM_CTRL_BLOCK  ://ControlBlock=========================================
        {

            mvoid_SetOneRegister( 0 , 3); //controll entrance
if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
	  ResponseGate(&bufGate[0]);
}
	  GateClearBuffer();
    d2=bufGate[1];
            mvoid_SetOneRegister( 1 , 1); //block exit
if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
	  ResponseGate(&bufGate[0]);
}
	  GateClearBuffer();
            d3=bufGate[1];
            mvoid_SetOneRegister( 3 , 0x09); //normaly close
if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
	  ResponseGate(&bufGateA[0]);
	 
}
 GateClearBuffer();   
d1=bufGate[1];
            d1&=0x02;
            d2&=0x02;
            d3&=0x02;
            if(d1 || d2 || d3 )
                RTX_ErrorFlag=1;
            else
                RTX_ErrorFlag=0;
            break;
        }

    case  GM_BLOCK_CTRL  ://=========================================================================
        {
           // virtualactive=true;
            mvoid_SetOneRegister( 0 , 1); //block entrance
if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
	  ResponseGate(&bufGate[0]);
}
	  GateClearBuffer();
             d2=bufGate[1];
             mvoid_SetOneRegister( 1 , 3);//controll exit
				if(WaitForGateResponse(1500)){
						ResponseGate(&bufGate[0]);
				}
						GateClearBuffer();
										d3=bufGate[1];
										mvoid_SetOneRegister( 3 , 0x09); //normaly close
				if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
						ResponseGate(&bufGate[0]);
				}
						GateClearBuffer();
            d1=bufGate[1];
            d1&=0x02;
            d2&=0x02;
            d3&=0x02;
            if(d1 || d2 || d3 )
                RTX_ErrorFlag=1;
            else
                RTX_ErrorFlag=0;
            break;
        }

    case  GM_CTRL_CTRL  ://=========================================================================
        {

            mvoid_SetOneRegister( 0 , 3); //controll entrance
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
												d2=bufGate[1];
													 mvoid_SetOneRegister( 1 , 3); //controll entrance
												// mvoid_SetOneRegister( 1 , 2);//controol exit
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
            d3=bufGate[1];
            mvoid_SetOneRegister( 3 , 0x09); //normaly close
if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
	  ResponseGate(&bufGate[0]);
}
	  GateClearBuffer();

            d1=bufGate[1];
            d1&=0x02;
            d2&=0x02;
            d3&=0x02;
            if(d1 || d2 || d3 )
                RTX_ErrorFlag=1;
            else
                RTX_ErrorFlag=0;
            break;
        }


    case  GM_BLOCK_FREE  ://=========================================================================
        {
//            virtualactive=false;
            mvoid_SetOneRegister( 0 , 1); //block entrance
if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
	  ResponseGate(&bufGate[0]);
}
	  GateClearBuffer();

            d2=bufGate[1];
            mvoid_SetOneRegister( 1 , 2);//free exit
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();

												d3=bufGate[1];
												mvoid_SetOneRegister( 3 , 0x09); //normaly close
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
            d1=bufGate[1];
            d1&=0x02;
            d2&=0x02;
            d3&=0x02;
            if(d1 || d2 || d3 )
                RTX_ErrorFlag=1;
            else
                RTX_ErrorFlag=0;
            break;
        }

    case  GM_BLOCK_BLOCK  ://BlockBlock=========================================================================
        {

            mvoid_SetOneRegister( 0 , 1); //block entrance
					if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
							ResponseGate(&bufGate[0]);
					}
							GateClearBuffer();

            d2=bufGate[1];
            mvoid_SetOneRegister( 1 , 1);//block exit
           
					if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
							ResponseGate(&bufGate[0]);
					}
							GateClearBuffer();
            d3=bufGate[1];
            mvoid_SetOneRegister( 3 , 9); //normaly close
					if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
							ResponseGate(&bufGate[0]);
					}
							GateClearBuffer();

            d1=bufGate[1];
            d1&=0x02;
            d2&=0x02;
            d3&=0x02;
            if(d1 || d2 || d3 )
                RTX_ErrorFlag=1;
            else
                RTX_ErrorFlag=0;
            break;
        }

    case  GM_FREE_FREE  ://FreeFree=========================================================================
        {

            mvoid_SetOneRegister( 1 , 2); //free exit
						if(WaitForGateResponse(1500)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
							 
												d3=bufGate[1];
												mvoid_SetOneRegister( 3 , 0x08); //normaly open
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
												d1=bufGate[1];
												mvoid_SetOneRegister( 0 , 2); //free entrance
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
            d2=bufGate[1];
            d1&=0x02;
            d2&=0x02;
            d3&=0x02;
            if(d1 || d2 || d3 )
                RTX_ErrorFlag=1;
            else
                RTX_ErrorFlag=0;
            break;
        }


    case  GM_CTRL_FREE  ://=========================================================================
        {

            mvoid_SetOneRegister( 0 , 3); //controll entrance
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();

												d2=bufGate[1];
												mvoid_SetOneRegister( 1 , 2);//free exit
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();

												d3=bufGate[1];
												mvoid_SetOneRegister( 3 , 0x09); //normaly close
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
            d1=bufGate[1];
            d1&=0x02;
            d2&=0x02;
            d3&=0x02;
            if(d1 || d2 || d3 )
                RTX_ErrorFlag=1;
            else
                RTX_ErrorFlag=0;
            break;
        }


    case  GM_FREE_BLOCK  ://=========================================================================
        {

            mvoid_SetOneRegister( 0 , 2); //free entrance
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
											 d2=bufGate[1];
												mvoid_SetOneRegister( 1 , 1);//block exit
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
												d3=bufGate[1];
												mvoid_SetOneRegister( 3 , 0x09); //normaly close
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
            d1=bufGate[1];
            d1&=0x02;
            d2&=0x02;
            d3&=0x02;
            if(d1 || d2 || d3 )
                RTX_ErrorFlag=1;
            else
                RTX_ErrorFlag=0;
            break;
        }

    case  GM_FREE_CTRL  : //=========================================================================
        {
            mvoid_SetOneRegister( 0 , 2); //free entrance
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
												d2=bufGate[1];
												mvoid_SetOneRegister( 1 , 3);//controll exite
												 
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
												d3=bufGate[1];
												mvoid_SetOneRegister( 3 , 0x09); //normaly close//09
											 
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
	          GateClearBuffer();
            d1=bufGate[1];
            d1&=0x02;
            d2&=0x02;
            d3&=0x02;
            if(d1 || d2 || d3 )
                RTX_ErrorFlag=1;
            else
                RTX_ErrorFlag=0;
            break;
        }
    case  GM_CLOSE  ://=========================================================================
        {

            mvoid_SetOneRegister( 0 , 1); //block entrance

						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
            d2=bufGate[1];
            mvoid_SetOneRegister( 1 , 1);//block exit
  					if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
            d3=bufGate[1];
            mvoid_SetOneRegister( 3 , 0x08); //normaly open
						if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
								ResponseGate(&bufGate[0]);
						}
								GateClearBuffer();
            d1=bufGate[1];
                d1&=0x02;
                d2&=0x02;
                d3&=0x02;
                if(d1 || d2 || d3 )
                    RTX_ErrorFlag=1;
                else
                    RTX_ErrorFlag=0;
                break;
        }
    case GM_UNKNOWN:
        break;

    default:
        break;
    }//end switch
}
void RS_485Init(void){
	         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    GPIO_InitTypeDef gpioa_init_struct;
	    gpioa_init_struct.GPIO_Pin = RS485_PIN;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RS485_PORT, &gpioa_init_struct);
	
}
void RELAY_Init(void){
	         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    GPIO_InitTypeDef gpioa_init_struct;
	    gpioa_init_struct.GPIO_Pin = RELAY1_PIN | RELAY2_PIN;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RELAY_PORT, &gpioa_init_struct);
	GPIO_ResetBits(	RELAY_PORT,RELAY1_PIN | RELAY2_PIN);
	
}
void RELAY_AutoRise(void){
	GPIO_SetBits(	RELAY_PORT,RELAY1_PIN | RELAY2_PIN);
	
	//GPIO_SetBits(	RELAY_PORT,  );
	Task1_Delay(500 MSec);
	GPIO_ResetBits(	RELAY_PORT,RELAY1_PIN | RELAY2_PIN);
	//Task1_Delay(2000 MSec);
// 	Task1_Delay(500 MSec);
}

void RS_485TXState(uint8_t s){
	if(s==1)
	GPIO_SetBits(RS485_PORT,RS485_PIN);
	else
	GPIO_ResetBits(	RS485_PORT,RS485_PIN);
	
}

uint8_t   muchar_GetConfig(void)
{
    long int i=0;
    uint8_t recieve_byte=0;
    uint8_t data;
    uint8_t rega=0;
    uint8_t regb=0;
    uint8_t regc=0;
  	GateClearBuffer();
	  Usart3Connection=0;
Task1_Delay(20 MSec);   
	mvoid_RegisterRequest(0); // request REG A if j=0,request REG B if j=1
	 if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
			ResponseGate(&bufGate[0]);
		}
			 GateClearBuffer();
			data=bufGate[5]&0x0f;
       rega=data;
Task1_Delay(20 MSec);      
		mvoid_RegisterRequest(1); // request REG A if j=0,request REG B if j=1
			 if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
					ResponseGate(&bufGate[0]);
				}
					GateClearBuffer();
        data=bufGate[5]&0x0f;
        regb=data;
        //881210        DELAY_MS(150);
       // mbool_Poll();
Task1_Delay(20 MSec);
        mvoid_RegisterRequest(3);   //reg relate NO | NC
	 if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
			ResponseGate(&bufGate[0]);
		}
			GateClearBuffer();
        data=bufGate[5]&0x0f;
        regc=data;

        data=0;
    if( rega==0x03 && regb==0x01 && regc==9)//control entrance block exit
    {
        data=GM_CTRL_BLOCK;
        return (GM_CTRL_BLOCK);
    }
    if( rega==0x01 && regb==0x03 && regc==9)//block entrance control exit
    {
        data=GM_BLOCK_CTRL;
        return (GM_BLOCK_CTRL);
    }
    if( rega==0x01 && regb==0x01 && regc==9)//block entrance block exit
    {
        data=GM_BLOCK_BLOCK;
        return (GM_BLOCK_BLOCK);
    }
    if( rega==0x01 && regb==0x01 && regc==8)
    {
        data=GM_CLOSE;
        return (GM_CLOSE);
    }
    if( rega==0x03 && regb==0x03 && regc==9)//control entrance control exit
    {
        data=GM_CTRL_CTRL;
        return (GM_CTRL_CTRL);
    }
    if( rega==0x02 && regb==0x02&& regc==8)//free entrance free exit
    {
        data=GM_FREE_FREE;
        return (GM_FREE_FREE);
    }
    if( rega==0x02 && regb==0x01&& regc==9)//free entrance block exit
    {
     data=GM_FREE_BLOCK;
        return (GM_FREE_BLOCK);
    }
    if( rega==0x01 && regb==0x02&& regc==9)//block entrance free exit
    {
       // if(virtualactive){
         data=GM_BLOCK_FREE;
        // virtualactive=false;
        // return(GM_BLOCK_FREE);


       // }

             data=GM_BLOCK_FREE;
        return (GM_BLOCK_FREE);
    }
    if( rega==0x03 && regb==0x02&& regc==9)//control entrance free exit
    {
        data=GM_CTRL_FREE;
        return (GM_CTRL_FREE);
    }
    if( rega==0x02 && regb==0x03&& regc==9)//free entrance control exit
    {
        data=GM_FREE_CTRL;
        return (GM_FREE_CTRL);
    }
    if(data==0)
        return (0x00); // GATE IS OFF
}
//*****************mvoid_OneFreePassageA*********************************************
void  mvoid_OneFreePassage(uint8_t sideOfTransit)
{
    long int i=0;
    unsigned char recive_byte=0;
  //Task1_Delay(20 MSec);    
	  	GateClearBuffer();
	  Usart3Connection=0;
	if(sideOfTransit==GS_A)
        mvoid_SetOneRegister( 0 , 0x13); //one free passage to direction A
    if(sideOfTransit==GS_B)
        mvoid_SetOneRegister( 1 , 0x13); //one free passage to direction B

 
	mvoid_RegisterRequest(0); // request REG A if j=0,request REG B if j=1
	 if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
			ResponseGate(&bufGate[0]);
		}
		GateClearBuffer();

 }


bool   mvoid_ClearCountAB(void)
{
    unsigned char data[4]={0,0,0,0};
    long int i=0;
    unsigned char recive_byte=0;
    unsigned char d1,d2;
bool clearCounter=false;
    bufGate[0]=0;
    bufGate[1]=0;
	  	GateClearBuffer();
	  Usart3Connection=0;    
for(int i=0 ; i<4 ; i++ )
    {
        counter_Buff_GateA[i]=0;
        counter_Buff_GateB[i]=0;
    }
    for(int j=0;j<2;j++)
    {
        if(j==0)
            mvoid_SetRegisters((unsigned char)GS_A, data , 4); //set counterA to zero
        if(j==1)
            mvoid_SetRegisters((unsigned char)GS_B, data , 4);//set counterB to zero

	 if(WaitForGateResponse(WAITE_FOR_RESPONSE+1000)){
			ResponseGate(&bufGate[0]);
		 clearCounter=true;
		 
		}

		//         

        if(j==0)
            d1=bufGate[1];
        if(j==1)
            d2=bufGate[1];
    }
				GateClearBuffer();
if(clearCounter) return 1;
		else return 0;
    d1&=0x02;
    d2&=0x02;
//     if(d1 || d2 )
//         RTX_ErrorFlag_ZeroCounter=1;
//     else
//         RTX_ErrorFlag_ZeroCounter=0;

}
bool StationClose(void){
if(mvoid_ClearCountAB()) return 1;
else return 0;	

}

uint8_t muchar_SendParameters(uint8_t address,uint8_t n_byte,uint8_t  *parameter)
{
 //switch(Get_State_SendParameter()){
//case SENDPARAMETR_STATE_START:
 // PRINT_TXT(muchar_SetParameters);
    long int j=0;
    unsigned char recive_byte=0;
    unsigned char d1,d2,i=0;
memset(&bufGate[0],0,50);
    buffGate_Out[0]=0x21;
    buffGate_Out[1]=0x6f;

    mvoid_EncodeRegPortocl(address,d1,d2);//////////////////////////////////////
    buffGate_Out[2]=d1;
    buffGate_Out[3]=d2;

    mvoid_EncodeRegPortocl(n_byte,d1,d2);
    buffGate_Out[4]=d1;
    buffGate_Out[5]=d2;

    for (i=0;i<n_byte;i++)
    {
        mvoid_EncodeDataPortocl(parameter[i],d1,d2);
        buffGate_Out[(2*i)+6]=d1;
        buffGate_Out[(2*i)+7]=d2;
        //  buffGate_Out[i+6]= parameter[i];
    }

    buffGate_Out[6+(2*i)]=0xc0;
    mvoid_ComputeCheckSum_String( 7+(2*n_byte) ,  buffGate_Out);
    buffGate_Out[6+(2*i)]=checkSumHigh;
    buffGate_Out[7+(2*i)]=checkSumLow;
    buffGate_Out[8+(2*i)]=0xc0;
    GateClearBuffer();
  //  Usart -> mvoid_Send(9+(2*n_byte)  , buffGate_Out); //send data
   RS485_DataSend(&buffGate_Out[0],9+(2*n_byte) );
	 if(WaitForGateResponse(WAITE_FOR_RESPONSE)){
			ResponseGate(&bufGate[0]);
		}
		GateClearBuffer();

		//Set_State_SendParameter(SENDPARAMETR_STATE_RESPONSE);
   // break;
 //case SENDPARAMETR_STATE_RESPONSE:
//     while( (recive_byte=(unsigned char) Usart -> muint_InRecive())<2   && j<MAX_WAIT_RECIEVE_DATA_1 ){
//         //     PRINT_BYTE(j);
//         j++;
//     }
//     if(recive_byte<2)
//     {
//         // Mcpu.mvoid_ResetWatchdog();
//         DELAY_MS(500);
//         Usart -> mint_GetString(2 , bufGate);
//     }
//     else
//     {
//         Usart -> mint_GetString(recive_byte , bufGate);
//         DELAY_MS(10);
//     }
    //Usart -> mviod_ClearInBuffer();
    d1=bufGate[0];
    d2=bufGate[1];
    d2&=0x02;
    if(d2)
        RTX_ErrorFlag=1;
    else
        RTX_ErrorFlag=0;
    if(d1==0x41)
        return 1;
    else
        return 0;
 //break;
 //}
 }
 
 
 uint8_t muchar_SetParameters(void)

{

        //unsigned char CGate::muchar_SetParameters(BYTE address,BYTE n_byte,BYTE __eeprom *parameter)
        for(int i=0;i<9;i++)
        {
         muchar_SendParameters((i*0xa),10,&DefaultParameters[(i*10)]);
          
        }
        muchar_SendParameters(0x5a,7,&DefaultParameters[90]);
       
        return 1;

}