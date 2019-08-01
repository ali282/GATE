

// #include "Gate.h"
// #include "Master.h"
#include "Manage.hpp"

 static U64 StartTasks_STAK[512/8];      
__task void Task1_data(void);
__task void Task2_data(void);
__task void StartTasks(void);
uint8_t taskstack1[4024];
uint8_t taskstack2[4024];
//Manage m;
OS_MUT mutex1;
 
int main(void){
	
	USART3_Config();	
 	RS_485Init();
	MasterInit();
	RELAY_Init();
 	PortInit();


				/// Start Main Task to init and start other tasks
			os_sys_init_user(StartTasks, 255, &StartTasks_STAK, sizeof(StartTasks_STAK));

			while(1);
	//return 0;
}
//*************************************************************** tasks *******************************************************************//
__task void StartTasks(void)
{

//os_tsk_create_user (
//	Task_Manager[Task_Manager_Added].Task,
//	Task_Manager[Task_Manager_Added].Priority,
//	Task_Manager[Task_Manager_Added].Stack,
//	Task_Manager[Task_Manager_Added].Stack_Size);

os_mut_init (&mutex1);
	os_tsk_create_user (Task1_data,3,&taskstack1,sizeof(taskstack1));		
os_tsk_create_user (Task2_data,4,&taskstack2,sizeof(taskstack2));	
os_tsk_delete_self();  // task must delete itself before exiting
			}

// }

__task void Task1_data(void){

GateManage();
// while(1){
// 	RELAY_AutoRise();
// }
}

__task void Task2_data(void){

MasterManage();


}