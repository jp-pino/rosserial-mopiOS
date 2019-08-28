#ifndef __ROS_H__
#define __ROS_H__

#define SYNC_F 0xFF
#define PROT_F 0xFE

#define ROS_TOPIC_LEN 20

// Serialized messages
typedef struct {
  unsigned short length;
  unsigned short topic_id;
  unsigned char *data;
} rospacket_t;

// ROS Subscriber Mailbox
typedef struct rosmailbox_t {
	sema_t free;
	sema_t valid;
	rospacket_t *message;
} rosmailbox_t;

// Publisher
typedef struct rospublisher_t {
	unsigned short topic_id;
	char name[ROS_TOPIC_LEN + 1];
	struct rospublisher_t *next;
	tcb_t *thread;
} rospublisher_t;

// Subscriber
typedef struct rossubscriber_t {
	unsigned short topic_id;
	char name[ROS_TOPIC_LEN + 1];
	struct rossubscriber_t *next;
	rosmailbox_t mailbox;
	tcb_t *thread;
} rossubscriber_t;

// Mailbox functions
void ROS_MailBox_Init(rosmailbox_t *mailbox);
void ROS_MailBox_Send(rosmailbox_t *mailbox, rospacket_t *message);
rospacket_t* ROS_MailBox_Recv(rosmailbox_t *mailbox);
void ROS_PacketFree(rospacket_t *message);

// Setup publishers and subscribers
rospublisher_t* ROS_AddPublisher(char *topic_name, void(*task)(void), unsigned int stack, unsigned int priority);
rossubscriber_t* ROS_AddSubscriber(char *topic_name, void(*task)(void), unsigned int stack, unsigned int priority);

// Look for subscribers
rossubscriber_t* ROS_FindSubscriberByTopic(unsigned short topic_id);
rossubscriber_t* ROS_FindSubscriber(void); 
rossubscriber_t* ROS_FindPublisher(void);

// Send serialized message
void ROS_Publish(rospacket_t *message);

// Setup ROS Environment
void ROS_Init(void);


#endif
