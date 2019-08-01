#ifndef GATE_H
#define GATE_H

#include "General.h"
#define WAITE_FOR_RESPONSE   800
#define MAX_GATE_NOTRESP  5
#define DEFUALT_GATE_MODE  GM_BLOCK_BLOCK 
#define ONLINE   1
#define OFFLINE   0


enum GATE_SIDE
{
    GS_A=6,
    GS_B,
};
enum GATE_MODE
{
    GM_UNKNOWN = 0,
    GM_CTRL_BLOCK,
    GM_BLOCK_CTRL,
    GM_CTRL_CTRL,
    GM_BLOCK_FREE,
    GM_BLOCK_BLOCK,
    GM_FREE_FREE,
    GM_CTRL_FREE,
    GM_FREE_BLOCK,
    GM_FREE_CTRL,
    GM_CLOSE ,
};
#define RS485_PORT  GPIOB
#define RS485_PIN  GPIO_Pin_12
#define RELAY_PORT  GPIOA
#define RELAY1_PIN  GPIO_Pin_4 
#define RELAY2_PIN  GPIO_Pin_5  
static uint8_t DefaultParameters[97]=//// XDATA
{
   // 0.0.  0.1.  0.2.  0.3.  0.4.  0.5.  0.6.  0.7.  0.8.  0.9.  0.A.  0.B.
   0x01, 0x0f, 0x3c, 0x01, 0x08, 0x01, 0x30, 0x01, 0x40, 0x08, 0x40, 0x80,
   // 0.C.  0.D.  0.E.  0.F.  1.0.  1.1.  1.2.  1.3.  1.4.  1.5.  1.6.  1.7.
   0xb8, 0x38, 0x01, 0x50, 0x08, 0x40, 0x80, 0xb8, 0x42, 0x02, 0x28, 0x00,
   // 1.8.  1.9.  1.A.  1.B.  1.C.  1.D.  1.E.  1.F.  2.0.  2.1.  2.2.  2.3.
   0x0a, 0x05, 0x00, 0x03, 0x05, 0x02, 0x0a, 0x05, 0xe0, 0x01, 0x0a, 0x05,
   // 2.4.  2.5.  2.6.  2.7.  2.8.  2.9.  2.A.  2.B.  2.C.  2.D.  2.E.  2.F.
   0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   // 3.0.  3.1.  3.2.  3.3.  3.4.  3.5.  3.6.  3.7.  3.8.  3.9.  3.A.  3.B.
   0x00, 0x01, 0x00, 0x04, 0x00, 0x02, 0x00, 0x08, 0x00, 0x02, 0x00, 0x08,
   // 3.C.  3.D.  3.E.  3.F.  4.0.  4.1.  4.2.  4.3.  4.4.  4.5.  4.6.  4.7.
   0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x03, 0x02, 0x01, 0x08, 0x0a, 0x06,
   // 4.8.  4.9.  4.A.  4.B.  4.C.  4.D.  4.E.  4.F.  5.0.  5.1.  5.2.  5.3.
   0x03, 0x0a, 0xfe, 0x0a, 0x06, 0x19, 0x02, 0x10, 0x14, 0x00, 0x18, 0x14,
   // 5.4.  5.5.  5.6.  5.7.  5.8.  5.9.  5.A.  5.B.  5.C.  5.D.  5.E.  5.F.
   0x03, 0x03, 0x00, 0x00, 0x0a, 0x10, 0x14, 0x0a, 0x32, 0x01, 0x05, 0x00,
   // 6.0.
   0x03
};
//class Gate
//{
//	public:
	void  mvoid_SetOneRegister( uint8_t address ,uint8_t data);
  void  mvoid_SetRegisters( uint8_t address ,  uint8_t *pdata ,uint8_t n_byte);
	void  mvoid_ComputeCheckSum( unsigned char dataIn1 , unsigned char dataIn2);
  void  mvoid_ComputeCheckSum_String(unsigned char n ,unsigned char *string);
  void  mvoid_EncodeRegPortocl(unsigned char address,unsigned char &r1,unsigned char &r2);
  void  mvoid_EncodeDataPortocl(unsigned char data,unsigned char &d1,unsigned char &d2);
  void  mvoid_DecodeDataPortocl(unsigned char &data,unsigned char var1,unsigned char var2);
	void  mvoid_RegisterRequest(unsigned char address);
  void 	mvoid_GetAlarms(void);	
	unsigned char  muchar_XorFunctionCheck(unsigned char *pstr,
                                                unsigned char size);

	
	unsigned char mbool_Poll(void);
	void mvoid_GetCounter(uint8_t whichGate);
  void 	mvoid_SetConfig(uint8_t Mode);
	bool mvoid_ClearCountAB(void);
	void mvoid_OneFreePassage(uint8_t sideOfTransit);
	uint8_t  muchar_GetConfig(void);
	bool WaitForGateResponse(uint16_t TimeOut);
	uint8_t ResponseGate(uint8_t *p);
	void Gate_Init(void);
	uint8_t GateClearBuffer(void);
	void RS_485Init(void);
void RS_485TXState(uint8_t s);
void RS485_DataSend(uint8_t *data,uint8_t lenght);
bool StationClose(void);
uint8_t muchar_SendParameters(uint8_t address,uint8_t n_byte,uint8_t  *parameter);
void RELAY_Init(void);
void RELAY_AutoRise(void);
//};




#endif

