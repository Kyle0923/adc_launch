#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

using namespace std;

void stop(geometry_msgs::Twist& cmd_vel, ros::Publisher& vel_pub)
{
    cmd_vel.linear.x = 0;
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0;
    vel_pub.publish(cmd_vel);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "manual_control");

    ros::NodeHandle rosNode;
    ros::Publisher vel_pub = rosNode.advertise<geometry_msgs::Twist>("cmd_vel", 50);

    geometry_msgs::Twist cmd_vel;
    double v = 1.3371; // calc from 170 rpm
    double w = 0;
    std::string input = "";
    double duration = 1.0;
    // ros::Rate loop_rate(500);

    while (ros::ok())
    {
        stop(cmd_vel, vel_pub);
        cout << "Waiting for input W-A-S-D" << endl;
        cin >> input;
        if (input == "a")
        {
            cout << "turnLeft" << endl;
            cmd_vel.linear.x = 0;
            cmd_vel.angular.z = 20.8916;
        }
        else if (input == "d")
        {
            cout << "turnRight" << endl;
            cmd_vel.linear.x = 0;
            cmd_vel.angular.z = -1 * 20.8916;
        }
        else if (input == "s")
        {
            cout << "reverse" << endl;
            cmd_vel.linear.x = -1 * v;
            cmd_vel.angular.z = w;
        }
        else if (input == "w")
        {
            cout << "forward" << endl;
            cmd_vel.linear.x = v;
            cmd_vel.angular.z = w;
        }
        else if (input == "dc")
        {
            cout << "duty cycle mode: e.g. for -10% input -0.1" << endl;
            cmd_vel.linear.x = -1;
            cmd_vel.angular.x = -1;
            cmd_vel.angular.y = -1;
            cmd_vel.angular.z = -1;
            cin >> cmd_vel.linear.y;
            cin >> cmd_vel.linear.z;
        }
        else if (input == "q")
        {
            stop(cmd_vel, vel_pub);
            continue;
        }
        else if (input == "config")
        {
            cin >> input;
            double configValue;
            cin >> configValue;
            if (input == "D")
            {
                if (configValue <= 0) {
                    duration = 1;
                } else {
                    duration = configValue;
                }
            }
            else if (input == "v")
            {
                if (configValue < 0) {
                    v = 1.3371;
                } else {
                    v = configValue;
                }
            }
            else if (input == "w")
            {
                w = configValue;
            }
            continue;
        }
        else if (input == "reset")
        {
            cmd_vel.linear.x = -1;
            cmd_vel.linear.y = -1;
            cmd_vel.linear.z = -1;
            cmd_vel.angular.x = -1;
            cmd_vel.angular.y = -1;
            cmd_vel.angular.z = -1;
        }
        else if (input == "exit")
        {
            stop(cmd_vel, vel_pub);
            break;
        }
        else if (input == "help" || input == "-h" || input == "--help")
        {
            cout << "=================== Usage ===================" << endl;
            cout << "W-A-S-D:\tDirection" << endl;
            cout << "q:\t\tstop (both pwm to 0)" << endl;
            cout << "config [num]:\tchange duration of the pwm signals" << endl;
            cout << "reset:\t\treset base_link frame coordinate" << endl;
            cout << "exit:\t\texit program" << endl;
            continue;
        }

        vel_pub.publish(cmd_vel);
        ros::Duration(duration).sleep();
    }
    return 0;
}