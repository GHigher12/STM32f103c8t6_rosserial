#include <mainpp.h>
#include <ros.h>
#include <std_msgs/String.h>
#include "main.h"
void command_callback( const std_msgs::String& rxbuff);

ros::NodeHandle nh;

std_msgs::String stm32_to_pc_word;

ros::Subscriber<std_msgs::String> cmd_sub("pc_to_stm32", command_callback);
ros::Publisher stm32_to_pc("stm32_to_pc", &stm32_to_pc_word);

void command_callback(const std_msgs::String &rxbuff)
{
    char oled_rxbuff[128];
    stm32_to_pc_word = rxbuff;
    snprintf(oled_rxbuff, sizeof(oled_rxbuff), "%s", rxbuff.data);
//  stm32_to_pc.publish(&stm32_to_pc_word);
    OLED_ShowString(0,24, (u8*)oled_rxbuff,16);
    OLED_Refresh();
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->flush();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->reset_rbuf();
}
char hello[13] = "hello ros!";
u8 cnt = 0;
void setup(void)
{
    nh.initNode();
	nh.advertise(stm32_to_pc);
//	nh.subscribe(cmd_sub);
}

void loop(void)
{
    cnt+=1;
    sprintf(oledBuf,"stm32_2_ros->%d",cnt);
    OLED_ShowString(0,24,(u8*)oledBuf,16);
    OLED_Refresh();
    stm32_to_pc_word.data = oledBuf;
    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	HAL_Delay(1000);
	stm32_to_pc.publish(&stm32_to_pc_word);
	nh.spinOnce();
}

