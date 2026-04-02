
#include "myHandler.h"
#include "externalComponents.h"

/*
	this function is a simple handler that reads messages containing numbers to be added together, processes them, and reports the result. 
	It continues to read messages until it encounters an error or there are no more messages to process.

	readNextMessage() is a function that retrieves the next message to be processed. It is defined elsewhere.
	reportResult() is a function that reports the result of the addition operation. It is also defined elsewhere.
	reportError() is a function that reports any errors encountered during message processing. It is defined elsewhere.
*/

void myHandler(void) {
	// a simulation of a handler that is called periodically...

	bool continueProcessing = true;

	while (continueProcessing) {

		Message_t msg = readNextMessage(); // none-blocking call 

		switch (msg.status) {
		case MSG_ERROR:
		{
			reportError("Error reading message for addition.");
			continueProcessing = false;
			break;
		}
		case NO_MSG:
		{
			// No message available, we can choose to wait, retry, or exit. For this demo, we simply exit.
			continueProcessing = false;
			break;
		}
		case MSG_AVAILABLE:
		{
			if ((0 == msg.count) || (NULL == msg.data)) {
				reportError("Invalid message.");
				continueProcessing = false;
			}
			else {
				// We have a message to process
				// in this demo, we simply add upp all values in the message...
				// we simply that the result will fit in a uint32_t, but in a real implementation, you would want to consider overflow and other edge cases.
				uint32_t i;
				uint32_t result = 0;
				for (i = 0; i < msg.count; i++) {
					result += msg.data[i];
				}
				reportResult("Addition", result);
			}
			break;
		}
		default:
			continueProcessing = false;
			break;
		}
	}
}
