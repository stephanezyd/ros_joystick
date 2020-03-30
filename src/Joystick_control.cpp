/*
 * @ Intro:
 * @ Author: Stephane ZHANG
 * @ Date: 2020-03-25 15:19
 */

#if 1
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"
#include "ros/ros.h"
#include "iostream"

ros::Subscriber sub;
ros::Publisher  pub;

int num_axe_lin, num_axe_ang;//选择手柄的数据
double factor_axe_lin, factor_axe_ang;//控制系数
int Button_Select;//对应手柄select键
bool flag_button_select;
std::string pub_name;//发布的消息名称
geometry_msgs::Twist speed;

void joy_callBack(const sensor_msgs::Joy::ConstPtr &joy)
{
    double linear_speed = joy->axes[num_axe_lin]*factor_axe_lin;
    double angular_speed = joy->axes[num_axe_ang]*factor_axe_ang;
    Button_Select = joy->buttons[10];

    if(Button_Select==1){
        if(flag_button_select)
            flag_button_select = false;
        else
            flag_button_select = true;
    }

    speed.linear.x = linear_speed;
    speed.angular.z = angular_speed;

    ROS_INFO("joystick linear:%f, angular:%f\n",speed.linear.x, speed.angular.z);
    if(Button_Select == 1)
        ROS_INFO("Button_Select\n");

}

int main(int argc, char **argv){

    ros::init(argc,argv,"Joystick");

    ros::NodeHandle nh;

    //配置默认参数，可从launch文件中配置自定义参数
    std::string default_pub_name = "/turtle1/cmd_vel";
    nh.param("num_axe_lin",num_axe_lin,1);
    nh.param("num_axe_ang",num_axe_ang,2);
    nh.param("factor_axe_lin",factor_axe_lin,1.0);
    nh.param("factor_axe_ang",factor_axe_ang,1.0);
    nh.param("pub_name",pub_name,default_pub_name);

    sub = nh.subscribe<sensor_msgs::Joy>("/joy",10,&joy_callBack);
    pub = nh.advertise<geometry_msgs::Twist >(pub_name,10);

    ros::Rate r(10);//发布频率
    flag_button_select = false;
    while(nh.ok()){
        ros::spinOnce();
        if(flag_button_select)//通过按键控制速度发布与否
            pub.publish(speed);
        r.sleep();
    }

    return 0;

}
#endif

