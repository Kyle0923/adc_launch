ip=`ip -4 addr show enp0s3 | grep -oP '(?<=inet\s)\d+(\.\d+){3}'`
export ROS_MASTER_URI=http://$ip:11311
export ROS_IP=$ip
