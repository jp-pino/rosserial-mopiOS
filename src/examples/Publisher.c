void ROSBoolPublisherExample(void) {
	rospacket_t *message;
	rosbool_t *boolmessage;
	while(1) {
		// Setup data to transmit
		boolmessage = Heap_Malloc(sizeof(rosbool_t));
		boolmessage->data = 1;

		// Setup packet to transmit
		message = Heap_Malloc(sizeof(rospacket_t));
		message->lenght = ROS_BOOL_LEN;
		message->topic_id = ROS_FindPublisher()->topic_id;
		message->data = ROS_BoolSerialize(boolmessage);

		// Transmit packet
		ROS_Publish(message);

		// Free data memory
		ROS_BoolFree(boolmessage);

		// Free packet memory
		ROS_PacketFree(message);
	}
}
