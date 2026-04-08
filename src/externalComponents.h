#ifndef EXTERNAL_COMPONENTS_H
#define EXTERNAL_COMPONENTS_H

#include <stdint.h>

typedef enum {
	MSG_ERROR,
	NO_MSG,
	MSG_AVAILABLE
} MessageStatus_t;

typedef struct {
	size_t count;
	int8_t* data;
	MessageStatus_t status;
} Message_t;

void reportResult(const char* operation, int32_t result);
void reportError(const char* errorMessage);
Message_t readNextMessage(void);

#endif // EXTERNAL_COMPONENTS_H