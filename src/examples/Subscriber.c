void ROSBoolSubscriberExample(void) {
	rospacket_t *message;
	rosbool_t *boolmessage;
	while(1) {
		// Wait for message
		message = ROS_MailBox_Recv(ROS_FindSubscriber()->mailbox);
		if (message) {
			// Deserialize data
			boolmessage = ROS_BoolDeserialize(message);
			if (boolmessage) {
				// Do whatever
				UART_OutChar(boolmessage->data);
				// Deallocate packet and data
				ROS_BoolFree(boolmessage);
				ROS_PacketFree(message);
			}
		}
	}
}
