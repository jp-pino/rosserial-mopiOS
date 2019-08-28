#ifndef __BOOL_H__
#define __BOOL_H__

#include "OS.h"
#include "ros.h"

#define ROS_BOOL_LEN

char ROS_BOOL_MD5[] = "8b94c1b53db61fb6aed406028ad6332a";
char ROS_BOOL_MSG[] = "std_msgs/Bool";

typedef struct {
	unsigned char data;
} rosbool_t;

unsigned char* ROS_BoolSerialize(rosbool_t *rosbool);
rosbool_t* ROS_BoolDeserialize(rospacket_t *message);

// Deallocate memory
void ROS_BoolFree(rosbool *rosbool);


#endif
