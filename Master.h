#ifndef MANAGE_H
#define MANAGE_H
#define ADDRESS 0x05
#define FUNCTION_P  0x01
#define LENGHT_P    0x02
#define ADDR_P      0x00
#define DATA_P      0x03
#include "General.h"

#define MASTER_PORT   2


enum FUNCTION_CODE{
	GETDATA=0x14,
	ONEFREEPASS=0x15,
	STATTION_CLOSE=0x16,
	CLEAR_COUNTER,
	SETCONFIG,
	SETMODE,
	AUTORISE,
	SOLONOID1CMD_SET,
	SOLONOID2CMD_SET,
	BUZZERCMD_SET,
	BUZZERCMD_CLR,	
	SOLONOID1CMD_CLR,
	SOLONOID2CMD_CLR

	
};
	


#ifdef __cplusplus
extern "C" {
#endif

void MasterInit(void);
void MASTER_ClearBuffer(void);
uint8_t MASTER_GetData(void);
uint8_t MASTER_SendData(uint8_t *p,uint8_t len);
		void MUDBUS_SendData(uint8_t *d,uint8_t len);
	void SendError(uint8_t err,uint8_t cmd,uint8_t len);
#ifdef __cplusplus
}
#endif
#endif