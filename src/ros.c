#include "ros.h"
#include "Heap.h"
#include "UART2.h"

// Limitations:
// 		- One subscriber per topic
// 		- One publisher per topic

// Mailbox functions
// rosmain:
// 		1. Allocate packet
// 		2. Send packet
// 		3. (Don't deallocate)

// Subscriber:
// 		1. Receive packet
// 			1. Allocate new packet
// 			2. Copy values
// 			3. Deallocate old packet (but not data)
// 		2. Do what you will with packet
// 		3. Deallocate packet and data

// Mailbox ---------------------------------------------------------------
// Initialize
void ROS_MailBox_Init(rosmailbox_t *mailbox) {
  OS_InitSemaphore("rosmail_f", &(mailbox->free), 1);
  OS_InitSemaphore("rosmail_v", &(mailbox->valid), 0);
}

// Send
void ROS_MailBox_Send(rosmailbox_t *mailbox, rospacket_t *message) {
  OS_bWait(&(mailbox->free));
  mailbox->message = message;
  OS_bSignal(&(mailbox->valid));
}

// Receive
rospacket_t* ROS_MailBox_Recv(rosmailbox_t *mailbox) {
	rospacket_t *message = Heap_Malloc(sizeof(rospacket_t));
  OS_bWait(&(mailbox->valid));
	message->length = mailbox->message->length;
	message->topic_id = mailbox->message->topic_id;
	message->data = mailbox->message->data;
	Heap_Free(mailbox->message);
  OS_bSignal(&(mailbox->free));
  return message;
}

// Deallocate memory
void ROS_PacketFree(rospacket_t *message) {
	Heap_Free(message->data);
	Heap_Free(message);
}

// Publishers and Subscribers --------------------------------------------
unsigned short topic_id = 101;
rospublisher_t *ROSPublishers = 0;
rossubscriber_t *ROSSubscribers = 0;

rospublisher_t* ROS_AddPublisher(char *topic_name, void(*task)(void), unsigned int stack, unsigned int priority) {

}
rossubscriber_t* ROS_AddSubscriber(char *topic_name, void(*task)(void), unsigned int stack, unsigned int priority) {

}

// Look for subscribers
rossubscriber_t* ROS_FindSubscriberByTopic(unsigned short topic_id) {
	// Find by name on lists
}
rossubscriber_t* ROS_FindSubscriber(void) {
	// Use OS_Id() on lists
}

// Checksums -------------------------------------------------------------

// Message Length Checksum = 255 - ((Message Length High Byte +
//                                   Message Length Low Byte) % 256 )
unsigned char checksum_length(unsigned short length) {
  return (unsigned char)(255 - ((((length >> 8) & 0xFF) + (length & 0xFF)) % 256) );
}

// Message Data Checksum = 255 - ((Topic ID Low Byte +
//                                 Topic ID High Byte +
//                                 Data byte values) % 256)
unsigned char checksum_data(unsigned short topic, unsigned short length, unsigned char *data) {
  unsigned char checksum = (topic & 0xFF) + ((topic >> 8) & 0xFF);
  for (int i = 0; i < length; i++) {
    checksum += data[i];
  }
  checksum = checksum % 256;
  checksum = 255 - checksum;
  return checksum;
}



// Protocol --------------------------------------------------------------

// 1st Byte - Sync Flag (Value: 0xff)
// 2nd Byte - Sync Flag / Protocol version
// 3rd Byte - Message Length (N) - Low Byte
// 4th Byte - Message Length (N) - High Byte
// 5th Byte - Checksum over message length
// 6th Byte - Topic ID - Low Byte
// 7th Byte - Topic ID - High Byte
// x Bytes  - Serialized Message Data
// Byte x+1 - Checksum over Topic ID and Message Data

// Send packet to PC

sema_t ROS_PUBLISH_FREE;

void ROS_Publish(rospacket_t *message) {
	OS_bWait(&ROS_PUBLUSH_FREE);
  UART2_OutChar(SYNC_F);
  UART2_OutChar(PROT_F);

  UART2_OutChar((unsigned char)((message->length) & 0xFF));
  UART2_OutChar((unsigned char)((message->length >> 8) & 0xFF));
  UART2_OutChar((unsigned char)(checksum_length(message->length)));

  UART2_OutChar((unsigned char)((message->topic_id) & 0xFF));
  UART2_OutChar((unsigned char)((message->topic_id >> 8) & 0xFF));

  for (int i = 0; i < message->length; i++) {
    UART2_OutChar((unsigned char)(message->data[i]));
  }
  UART2_OutChar((unsigned char)(checksum_data(message->topic_id, message->length, message->data)));
	OS_bSignal(&ROS_PUBLUSH_FREE);
}

// Receive packet from PC
rospacket_t* message_in(void) {
  rospacket_t *message;
  unsigned char length_h, length_l, csum_length, csum_data, topic_l, topic_h, sync_f, prot_f;
  unsigned short length, topic;

	// Allocate memory for message
  message = Heap_Malloc(sizeof(rospacket_t));
  if (message == 0)
    return 0;

	// Sync message
	do {
		sync_f = UART2_InChar();
		prot_f = UART2_InChar();
	} while (sync_f != SYNC_F || prot_f != PROT_F);

	// Read message length
  length_h = UART2_InChar();
  length_l = UART2_InChar();
  length = (length_h << 8) & length_l;

	// Read checksum and verify
  csum_length = UART2_InChar();
  if (csum_length != checksum_length(length))
    return 0;

	// Allocate memory for message data
  message->data = Heap_Malloc(sizeof(unsigned char) * length);
  if (message->data == 0) {
    Heap_Free(message);
    return 0;
  }

  topic_l = UART2_InChar();
  topic_h = UART2_InChar();
  topic = (topic_h << 8) & topic_l;

  for (int i = 0; i < length; i++) {
    message->data[i] = UART2_InChar();
  }

  csum_data = UART2_InChar();
  if (csum_data != checksum_data(topic, length, message->data)) {
    Heap_Free(message->data);
    Heap_Free(message);
    return 0;
  }
  return message;
}

// Topic negotiation


// Main thread
void rosmain(void) {
	rospacket_t *message;
	rossubscriber_t *subscriber;

	// Topic negotiation
	message = message_in();



	// Listen for topics
	while(1) {
		message = message_in();
		if (message == 0) continue;
		switch(message->topic_id) {
			case ROS_ID_PUBLISHER:
				break;
			case ROS_ID_SUBSCRIBER:
				break;
			case ROS_ID_SERVICE_SERVER:
				break;
			case ROS_ID_SERVICE_CLIENT:
				break;
			case ROS_ID_PARAMETER_REQUEST:
				break;
			case ROS_ID_LOG:
				break;
			case ROS_ID_TIME:
				break;
			case ROS_ID_TX_STOP:
				break;
			default:
				subscriber = ROS_FindSubscriberByTopic(message->topic_id);
				if (subscriber)
					ROS_MailBox_Send(subscriber->mailbox, message);
		}


		// Free memory for new message
		Heap_Free(message->data);
		Heap_Free(message);
	}
}

// Initialize ------------------------------------------------------------
// Called after declaring Publishers and Subscribers
void ROS_Init(void) {
	// SEMAPHORE INITIALIZATION
	OS_InitSemaphore("rosout", &ROS_PUBLISH_FREE, 1);

	// Launch main thread
	OS_AddThread("ros", &rosmain, 512, 5); // Same priority as interpreter
}
