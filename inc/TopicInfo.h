#ifndef __TOPICINFO_H__
#define __TOPICINFO_H__

// Special Topic IDs
#define ID_PUBLISHER 					0
#define ID_SUBSCRIBER 				1
#define ID_SERVICE_SERVER 		2
#define ID_SERVICE_CLIENT 		4
#define ID_PARAMETER_REQUEST 	6
#define ID_LOG 								7
#define ID_TIME 							10
#define ID_TX_STOP 						11

typedef struct {
	unsigned short topic_id;
	char topic_name;
	char *message_type;
	char *md5sum;
	unsigned int buffer_size;
} TopicInfo;

TopicInfo* createTopicInfo(unsigned short topic_id, char *topic_name,
	char *message_type, char *md5sum, unsigned int buffer_size);
unsigned char* serializeTopicInfo(TopicInfo *message);

#endif
