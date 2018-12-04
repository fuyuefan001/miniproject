/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/




#include "miniproject.h"

void nano_wait(int t) {
    asm("       mov r0,%0\n"
        "repeat:\n"
        "       sub r0,#83\n"
        "       bgt repeat\n"
        : : "r"(t) : "r0", "cc");
}

int16_t acc1[3]={0};
int16_t gyr1[3]={0};
int16_t dmp[3]={0};
float power;//-100 to 100
float p,i,d;
int16_t acczero[3]={0};
int16_t gyrzero[3]={-264,-263,3};
float ANG=0;
float GYO=0;
const float fRad2Deg = 57.295779513f; //弧度换算角度乘的系数
const float dt = 0.005; //时间周期
float angle[3] = {0,0,0};
float R = 0;
int speed;
int main(void)
{
//	init_pwm();
	tim1_init();
	duty_cyc1(700);

	nano_wait(1000000000);
//	for(int i=0;i<100;i++){
//		duty_cyc1(800+5*i);
//		nano_wait(1000000000);
//	}
	char buf1[32]={0};
	char buf2[20]={0};
	char buf3[20]={0};
	init_usart2();
	sendmsg("Finish init usart2",18);
	I2C_config();
	sendmsg("Finish init i2c",15);
	MPU6050_Init();
	sendmsg("Finish init gyroscope",21);
	p=3  ;
	d=0 ;
    while(1){
    	GetAccGyro();
    	int i;
//    	for(i=0;i<3;i++){
//    		gyr1[i]-=gyrzero[i];
//    	}
    	ImuCalculate_Complementary();

    	//kalman_filter(angle[0],gyr1[0],&ANG,&GYO);
    	//TODO:pid control
    	ANG=angle[0];
    	GYO=gyr1[0];
    	sprintf(buf1,"ACC: [%5f %5f %5f] m/s",acc1[0]/835.92,acc1[1]/835.92,acc1[2]/835.92);
    	println(buf1);
    	sprintf(buf1,"GYO: [%5f %5f %5f] deg/s",gyr1[0]*1.0,gyr1[1]*1.0,gyr1[2]*1.0);
    	println(buf1);
    	sprintf(buf1,"ANG: [%5f %5f %5f] deg",angle[0],angle[1],angle[2]);
    	println(buf1);
    	sprintf(buf1,"DMP: [%5f %5f %5f] ",dmp[0],dmp[1],dmp[2]);
    	println(buf1);
    	sprintf(buf1,"ANG,GYO: [%5f %5f] ",ANG,GYO);
    	println(buf1);
       	sprintf(buf1,"power: %.2f",power);
        println(buf1);



    	power=p*(ANG-3)+d*(GYO+250);

    	duty_cyc1(powerscale((int)power));

    	writechar('\n');
//    	nano_wait(800000000);

    }

    return 0;
}
