#include "Manage.hpp"
uint16_t GateDisconnectCounter=0;
uint8_t GateDisconnect=1;
uint8_t StationCloseFlag=0;
uint8_t SetConfigFlag=0;
uint8_t mode=DEFUALT_GATE_MODE;
uint8_t GateRecieveMode=DEFUALT_GATE_MODE;
uint8_t OnfreePassFlag=0;
uint8_t GateDataLenght=0;
extern uint8_t gateEngaged;
extern uint8_t counter_Buff_GateA[4];
extern uint8_t counter_Buff_GateB[4];
union{
	uint8_t MasterWithGateBuff[255];
	struct{
	uint8_t status;
  uint8_t mode;
  uint8_t Config;
	uint8_t GateAlarm[2];	
  uint8_t counterA[4];
  uint8_t counterB[4];		
	}segment;
}GateDataRecieve;
extern struct{
	uint8_t data[250];
	uint8_t pointer;
}MasterBuffer;
extern unsigned char alarm_Buff_Gate[20];





extern OS_MUT mutex1;


void GateInit(void){
uint32_t i=0;
}

	
void GateManage(void){
	bool firstflag=false;
	unsigned char LastValidConfig=0x05;
while(1){
	Task1_Delay(200 MSec);
	if(mbool_Poll() ){
		GateDisconnect=0;
		GateDisconnectCounter=0;
	//	if(!firstflag) mvoid_SetConfig(0x09);	
		firstflag=true;
		if(StationCloseFlag){
			  mvoid_SetConfig(GM_CLOSE);	
				if(StationClose()){	
				os_mut_wait(&mutex1,0xFFF);
				StationCloseFlag=0;	
				os_mut_release(&mutex1);}
				else{
				os_mut_wait(&mutex1,0xFFF);
				StationCloseFlag=1;	
				os_mut_release(&mutex1);	
				}
		}
		else if(SetConfigFlag){
				os_mut_wait(&mutex1,0xFFF);
				SetConfigFlag=0;	
				os_mut_release(&mutex1);	
				mvoid_SetConfig(mode);
		}
		else if(OnfreePassFlag){
		OnfreePassFlag=0;
mvoid_OneFreePassage(GS_A);			
			
			
		}
os_mut_wait(&mutex1,0xFFF);
    mvoid_GetCounter(GS_A);
	os_mut_release(&mutex1);	
os_mut_wait(&mutex1,0xFFF);		
		mvoid_GetCounter(GS_B);
	os_mut_release(&mutex1);	
		os_mut_wait(&mutex1,0xFFF);
						
					mvoid_GetAlarms();	
				os_mut_release(&mutex1);
		
		LastValidConfig=muchar_GetConfig();
		
  	if(LastValidConfig){ GateRecieveMode=LastValidConfig;
		
}
   	}else{
		++GateDisconnectCounter;
		if(GateDisconnectCounter >= MAX_GATE_NOTRESP){
		GateDisconnect=1;
		GateDisconnectCounter=0;
    }

	}
	//os_mut_release(&mutex1);
	os_dly_wait(5);	
//	Task1_Delay(5 MSec);
}	
		
	}
	
void PraperGateReport(void){
uint8_t lenghtB=0;
// union{
// 	uint8_t MasterWithGateBuff[255];
// 	struct{
// 	uint8_t status;
//   uint8_t mode;
//   uint8_t Config;
// 	uint8_t GateAlarm[2];	
//   uint8_t counterA[4];
//   uint8_t counterB[4];		
// 	}segment;
// }GateDataRecieve;
	memset(&GateDataRecieve.MasterWithGateBuff[0],0,100);	
GateDataLenght=0;
	if(GateDisconnect==0 ){
GateDataRecieve.segment.status=1;
GateDataRecieve.segment.mode=ONLINE;	
GateDataRecieve.segment.Config=GateRecieveMode;
GateDataRecieve.segment.GateAlarm[0]=alarm_Buff_Gate[0];	
GateDataRecieve.segment.GateAlarm[1]=alarm_Buff_Gate[1];		
GateDataLenght =5;		
GateDataRecieve.segment.counterA[0]=counter_Buff_GateA[0];
GateDataRecieve.segment.counterA[0]=counter_Buff_GateA[1];
GateDataRecieve.segment.counterA[0]=counter_Buff_GateA[2];
GateDataRecieve.segment.counterA[0]=counter_Buff_GateA[3];
GateDataLenght +=4;	
GateDataRecieve.segment.counterB[0]=counter_Buff_GateB[0];
GateDataRecieve.segment.counterB[0]=counter_Buff_GateB[1];
GateDataRecieve.segment.counterB[0]=counter_Buff_GateB[2];
GateDataRecieve.segment.counterB[0]=counter_Buff_GateB[3];
GateDataLenght +=4;	
}else{
memset(&GateDataRecieve.MasterWithGateBuff[0],0,100);	
GateDataRecieve.segment.status=0x02;	
GateDataLenght =1;		
}



}	
	
	
void MasterManage(void){
	uint8_t ack=0x01;
		while(1){
//BuzzerSet();			
 if(MASTER_GetData()){
	 //Task1_Delay(200 MSec);
 if( MasterBuffer.data[FUNCTION_P] == GETDATA){
	os_mut_wait(&mutex1,0xFFF);
	 PraperGateReport();
	 MUDBUS_SendData(&GateDataRecieve.MasterWithGateBuff[0],GateDataLenght);
	 //MASTER_SendData(&GateDataRecieve.MasterWithGateBuff[0],GateDataLenght);
	 os_mut_release(&mutex1);
 }
 else if	( MasterBuffer.data[FUNCTION_P] == SETCONFIG){
	 mode=MasterBuffer.data[DATA_P];
   SetConfigFlag=1;
   MASTER_SendData(&ack,1);
	 }
	  else if	( MasterBuffer.data[FUNCTION_P] == STATTION_CLOSE){
	  StationCloseFlag=1;
			MASTER_SendData(&ack,1);
	 }
		  else if	( MasterBuffer.data[FUNCTION_P] == ONEFREEPASS ){
  			OnfreePassFlag=1;
				MASTER_SendData(&ack,1);
			}
	else if(MasterBuffer.data[FUNCTION_P] == AUTORISE){
	 RELAY_AutoRise();
	 //Task1_Delay(24 MSec);
	 MASTER_SendData(&ack,1);
 }
 	else if(MasterBuffer.data[FUNCTION_P] ==SOLONOID1CMD_SET){
  MASTER_SendData(&ack,1);
		SolonidSet(1);
	 
 }
 else if(MasterBuffer.data[FUNCTION_P] == SOLONOID2CMD_SET){
 MASTER_SendData(&ack,1);
 SolonidSet(2);

 }
  else if(MasterBuffer.data[FUNCTION_P] == BUZZERCMD_SET){
 MASTER_SendData(&ack,1);
 BuzzerSet();

 }
  	else if(MasterBuffer.data[FUNCTION_P] ==SOLONOID1CMD_CLR){
  MASTER_SendData(&ack,1);
SolonidClear(1);
	 
 }
 else if(MasterBuffer.data[FUNCTION_P] == SOLONOID2CMD_CLR){
 MASTER_SendData(&ack,1);
 SolonidClear(2);

 }
  else if(MasterBuffer.data[FUNCTION_P] == BUZZERCMD_CLR){
 MASTER_SendData(&ack,1);
 BuzzerClear();

 }
 
 }

	MASTER_ClearBuffer();	
	os_dly_wait(5);
	Task2_Delay(20);	
}
//os_mut_release(&mutex1);	
}



	