#include "can/can_model.h"
#include "stm32f1xx_hal.h"
extern CAN_HandleTypeDef hcan;
static bool stm32_can_send(const can_frame_t *f){
 if(!can_frame_valid(f))return false;
 CAN_TxHeaderTypeDef h={.StdId=f->id,.ExtId=0,.IDE=CAN_ID_STD,.RTR=CAN_RTR_DATA,.DLC=f->dlc,.TransmitGlobalTime=DISABLE};uint32_t mb=0;
 return HAL_CAN_AddTxMessage(&hcan,&h,(uint8_t*)f->data,&mb)==HAL_OK;
}
static void stm32_can_config_filter(void){
 CAN_FilterTypeDef flt={0};flt.FilterBank=0;flt.FilterMode=CAN_FILTERMODE_IDMASK;flt.FilterScale=CAN_FILTERSCALE_32BIT;
 flt.FilterIdHigh=(uint16_t)(0x100U<<5);flt.FilterMaskIdHigh=(uint16_t)(0x7F0U<<5);flt.FilterFIFOAssignment=CAN_FILTER_FIFO0;flt.FilterActivation=ENABLE;
 (void)HAL_CAN_ConfigFilter(&hcan,&flt);
}
void stm32_can_start(void){stm32_can_config_filter();(void)HAL_CAN_Start(&hcan);(void)HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hc){
 CAN_RxHeaderTypeDef h;uint8_t d[8];if(HAL_CAN_GetRxMessage(hc,CAN_RX_FIFO0,&h,d)!=HAL_OK||h.IDE!=CAN_ID_STD||h.DLC>8U)return;
 can_frame_t f={.id=(uint16_t)h.StdId,.dlc=(uint8_t)h.DLC};for(uint8_t i=0;i<f.dlc;i++)f.data[i]=d[i];(void)f;
}
/* Board note: bxCAN requires an external CAN transceiver; MCU pins never connect directly to CANH/CANL. */
