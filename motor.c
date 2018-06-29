#include "ev3api.h"
#include "motor.h"
motor_port_t motorCenter=EV3_PORT_A,motorLeft=EV3_PORT_B,motorRight=EV3_PORT_C;
static const sensor_port_t ultrasonic_sensor = EV3_PORT_2;

void main_task(intptr_t exinf){
	int battery;
	ulong_t time;
	//speed25%_2s and100%_2s
	int speed=15,wait=2000;
	//param
	float wr=5.6;
	float pi =3.14159;
	int cum,th,stop;
	    /* îΩéÀåıíläiî[ópÇÃïœêî */
	static int dist;
	int highpace=50,slowpace=5;
	int ws=0;
	int32_t degreeCenter,degreeLeft,degreeRight;

	//motor_type_settings
	ev3_motor_config(motorCenter,MEDIUM_MOTOR);
	ev3_motor_config(motorLeft,LARGE_MOTOR);
	ev3_motor_config(motorRight,LARGE_MOTOR);
	ev3_sensor_config(ultrasonic_sensor,ULTRASONIC_SENSOR);

	//motor_clear
	ev3_motor_reset_counts(motorCenter);
	ev3_motor_reset_counts(motorLeft);
	ev3_motor_reset_counts(motorRight);
	ev3_lcd_set_font(EV3_FONT_MEDIUM);

	//wait_for_press_down
	ev3_lcd_draw_string("start.wait DOWN",1*CSIZE,5*RSIZE);
	while(ev3_button_is_pressed(DOWN_BUTTON)) tslp_tsk(100);
	while(!ev3_button_is_pressed(DOWN_BUTTON)) tslp_tsk(100);
	ev3_lcd_draw_string("forward	",1*CSIZE,5*RSIZE);

	//forward(dist)	
	
		dist = ev3_ultrasonic_sensor_get_distance(ultrasonic_sensor);
		cum = (int)(dist/wr/pi*360);
		th = (int)(20/wr/pi*360);
		stop = (int)(10/wr/pi*360);
		while(1){
			ev3_motor_set_power(motorLeft,speed);
			ev3_motor_set_power(motorRight,speed);

			degreeLeft=0;
			while(30<dist){
				dist = ev3_ultrasonic_sensor_get_distance(ultrasonic_sensor);
				tslp_tsk(100);
			}
			while(dist!=255){
				dist = ev3_ultrasonic_sensor_get_distance(ultrasonic_sensor);
				tslp_tsk(120);

			}
			
			ev3_motor_stop(motorLeft,false);
			ev3_motor_stop(motorRight,false);
			tslp_tsk(1000);				
			ev3_motor_reset_counts(motorCenter);
			ev3_motor_reset_counts(motorLeft);
			ev3_motor_reset_counts(motorRight);
			ev3_motor_set_power(motorLeft,-speed);
			ev3_motor_set_power(motorRight,-speed);
			cum=(-15/(wr*pi))*360;

			while(cum<degreeLeft){
				degreeLeft=ev3_motor_get_counts(motorLeft);
				tslp_tsk(100);
			}
			ev3_motor_stop(motorLeft,false);
			ev3_motor_stop(motorRight,false);
			ev3_motor_reset_counts(motorCenter);
			ev3_motor_reset_counts(motorLeft);
			ev3_motor_reset_counts(motorRight);
		
		}
/*		while(degreeLeft<cum-th){
			degreeLeft=ev3_motor_get_counts(motorLeft);
			ev3_lcd_draw_string("degreeLeft=",1*CSIZE,5*RSIZE);
			tslp_tsk(10);
		}
		while(degreeLeft<cum-stop){
			degreeLeft=ev3_motor_get_counts(motorLeft);
			ev3_lcd_draw_string("degreeLeft=",1*CSIZE,5*RSIZE);
			tslp_tsk(10);
			ws=((float)((float)(highpace-slowpace)*(cum-degreeLeft)/(cum-(cum-th)))+slowpace);
			speed=((int)(ws+0.5));
			ev3_motor_set_power(motorLeft,speed);
			ev3_motor_set_power(motorRight,speed);
		}*/
		//wait
	//	tslp_tsk(wait);
		dispLcd();
		tslp_tsk(100);
		ev3_motor_set_power(motorLeft,-5);
		ev3_motor_set_power(motorRight,-5);
		if(degreeLeft>degreeLeft-th){
			degreeLeft=ev3_motor_get_counts(motorLeft);
			ev3_lcd_draw_string("degreeLeft=",1*CSIZE,5*RSIZE);
			tslp_tsk(10);
			ws=((float)((float)(highpace-slowpace)*(cum-degreeLeft)/th)+slowpace);
			speed=((int)(ws+0.5)*-1);
			ev3_motor_set_power(motorLeft,speed);
			ev3_motor_set_power(motorRight,speed);
		}
		else if(degreeLeft>th){
			degreeLeft=ev3_motor_get_counts(motorLeft);
			ev3_lcd_draw_string("degreeLeft=",1*CSIZE,5*RSIZE);
			tslp_tsk(10);
		}
		else if(degreeLeft>0){
			degreeLeft=ev3_motor_get_counts(motorLeft);
			ev3_lcd_draw_string("degreeLeft=",1*CSIZE,5*RSIZE);
			tslp_tsk(10);
			ws=((float)((float)(highpace-slowpace)*degreeLeft/th)+slowpace);
			speed=((int)(ws+0.5)*-1);
			ev3_motor_set_power(motorLeft,speed);
			ev3_motor_set_power(motorRight,speed);
		}
		else{
			ev3_motor_stop(motorLeft,true);
			ev3_motor_stop(motorRight,true);
			}
	dispLcd();

}
void dispLcd(){
	char buf[64];
	int battery;
	int32_t cntCenter,cntLeft,cntRight;
	battery=ev3_battery_voltage_mV();
	sprintf(buf,"		%d.%d",battery/1000,((battery+50)%100)/10);
	ev3_lcd_draw_string(buf,0,1*RSIZE);
	cntCenter=ev3_motor_get_counts(motorCenter);
	cntLeft=ev3_motor_get_counts(motorLeft);	
	cntRight=ev3_motor_get_counts(motorRight);
	sprintf(buf,"	A:%ld B:%ld C:%ld	",cntCenter,cntLeft,cntRight);
	ev3_lcd_draw_string(buf,0,2*RSIZE);
}