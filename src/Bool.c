#include "Bool.h"

rosbool_t* BoolDeserialize(rospacket_t *message) {
	rosbool_t *parse;

	// Allocate memory for parse
	parse = Heap_Malloc(sizeof(rosbool_t));
	if (parse == 0)
		return 0;

	// Parse message
	if (message->length == 1){
		parse->data = message->data;
		return parse;
	}

	// Error, free memory
	Heap_Free(parse);
	return 0;
}
