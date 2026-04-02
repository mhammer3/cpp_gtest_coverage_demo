#include <gtest/gtest.h>
#include <vector>

extern "C" {
#include "myHandler.h"
#include "externalComponents.h"
}

static std::function<void(const char* operation, int result)> g_callback_reportResult;
static std::function<void(const char* errorMessage)> g_callback_reportError    ;
static std::function<Message_t(void)> g_callback_readNextMessage;

Message_t readNextMessage(void) {
    if (g_callback_readNextMessage) {
        return g_callback_readNextMessage();
    }
    return Message_t{0, nullptr, MSG_ERROR};
}

void reportResult(const char* operation, int32_t result) {
    if (g_callback_reportResult) {
        g_callback_reportResult(operation, result);
    }
}

void reportError(const char* errorMessage) {
    // Implementation of error reporting
    if (g_callback_reportError) {
        g_callback_reportError(errorMessage);
    }
}

class TestHandlerAdd : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any necessary state before each test
        g_callback_reportResult = nullptr;
        g_callback_reportError = nullptr;
        g_callback_readNextMessage = nullptr;
    }
    void TearDown() override {
        // Clean up any state after each test
        g_callback_reportResult = nullptr;
        g_callback_reportError = nullptr;
        g_callback_readNextMessage = nullptr;
    }
};

TEST_F(TestHandlerAdd, SingleMsg_Success) {
	// this is the callback that will be called by the handler to report the result of the addition operation,
    g_callback_reportResult = [&](const char* operation, int result) {
        EXPECT_STREQ(operation, "Addition");
        EXPECT_EQ(result, 8);
    };

	/* this is the message sequence for the handler to process, in this case we have a single message containing the numbers 3 and 5, 
    which should result in an addition operation that yields 8. 
    After this message is processed, the callback will return a NO_MSG status to indicate there are no more messages to process. */
    std::vector<Message_t> msgSequence;
    {
        uint8_t data[] = { 3, 5 };
        msgSequence.push_back({ sizeof(data) / sizeof(*data), data, MSG_AVAILABLE });
    }

	// this is the callback that will be called by the handler to read the next message, it will return messages from the msgSequence vector until it is exhausted, after which it will return a NO_MSG status to indicate there are no more messages to process.
    g_callback_readNextMessage = [seq = msgSequence, index = 0]() mutable {
        if (static_cast<size_t>(index) < seq.size()) {
            return seq[index++];
        }
        return Message_t{ 0, nullptr, NO_MSG }; // no more messages after the sequence
    };

	// finally, we call the handler to process the message and report the result, which will trigger the callbacks we set up above to validate the behavior of the handler.
    myHandler();
}

TEST_F(TestHandlerAdd, MultiMsg_Success) {

	// set the input messages for the handler to process and the expected results for the handler to report
    static std::vector<uint8_t> myInput1 = { 3, 5 };
    static std::tuple<const char*, int> myExpectedOutput1 = std::make_tuple("Addition", 8);
    static std::vector<uint8_t> myInput2 = { 1, 2, 3 };
    static std::tuple<const char*, int> myExpectedOutput2 = std::make_tuple("Addition", 6);

	// msg sequence for the handler to process
    std::vector<Message_t> msgSequence;
    msgSequence.push_back({ myInput1.size(), myInput1.data(), MSG_AVAILABLE });    
    msgSequence.push_back({ myInput2.size(), myInput2.data(), MSG_AVAILABLE });
    
	// expected results for the handler to report
    std::vector<std::tuple<const char*, int>> expectedResults;
    expectedResults.push_back(myExpectedOutput1);
    expectedResults.push_back(myExpectedOutput2);
        
	/* this is the callback that will be called by the handler to read the next message, 
    it will return messages from the msgSequence vector until it is exhausted, 
    after which it will return a NO_MSG status to indicate there are no more messages to process. */
    g_callback_readNextMessage = [seq = msgSequence, index = 0]() mutable {
        if (static_cast<size_t>(index) < seq.size()) {
            return seq[index++];
        }
        return Message_t{ 0, nullptr, NO_MSG }; // no more messages after the sequence
    };

    /* this is the callback that will be called by the handler to report the result of an operation,
    it will compare the reported result with the expected result from the expectedResults vector. */
    g_callback_reportResult = [seq = expectedResults, index = 0](const char* operation, int result) mutable {
        if (static_cast<size_t>(index) < seq.size()) {
            const auto& [expectedOperation, expectedResult] = seq[index++];
            EXPECT_STREQ(operation, expectedOperation) << "Operation mismatch at msgSequence[" << index - 1 << "]";
            EXPECT_EQ(result, expectedResult) << "Result mismatch at msgSequence[" << index - 1 << "]";
        }
    };

	// finally, we call the handler to process the messages and report results, which will trigger the callbacks we set up above to validate the behavior of the handler.
    myHandler();
}

TEST_F(TestHandlerAdd, SingleMsg_Failure) {
    // Test case for a single message failure scenario
    g_callback_reportError = [&](const char* errorMessage) {
        EXPECT_STREQ(errorMessage, "Error reading message for addition.");
    };

    g_callback_readNextMessage = []() {
        return Message_t{0, nullptr, MSG_ERROR};
    };

    myHandler();
}

TEST_F(TestHandlerAdd, SingleMsg_InvalidInput) {
    // this is the callback that will be called by the handler to report the result of the addition operation,
    g_callback_reportError = [&](const char* operation) {
        EXPECT_STREQ(operation, "Invalid message.");
        };

    /* this is the message sequence for the handler to process, in this case we have a single message containing the numbers 3 and 5,
    which should result in an addition operation that yields 8.
    After this message is processed, the callback will return a NO_MSG status to indicate there are no more messages to process. */
    std::vector<Message_t> msgSequence;
    {
        uint8_t data[] = { 3, 5 };
        msgSequence.push_back({ 0, data, MSG_AVAILABLE }); // this is an invalid message because the count is 0, which should trigger an error report from the handler.
    }

    // this is the callback that will be called by the handler to read the next message, it will return messages from the msgSequence vector until it is exhausted, after which it will return a NO_MSG status to indicate there are no more messages to process.
    g_callback_readNextMessage = [seq = msgSequence, index = 0]() mutable {
        if (static_cast<size_t>(index) < seq.size()) {
            return seq[index++];
        }
        return Message_t{ 0, nullptr, NO_MSG }; // no more messages after the sequence
        };

    // finally, we call the handler to process the message and report the result, which will trigger the callbacks we set up above to validate the behavior of the handler.
    myHandler();
}
