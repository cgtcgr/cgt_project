
#include "ota-bootload-ncp.h"
void emAfInitXmodemState(bool startImmediately);

// Send a chunk of data via xmodem.  Arbitrary lengths of data may be passed,
//   it will be broken up into appropriate sized chunks for transmission. Xmodem
//   itself transfers data in 128 byte chunks
// Note: This function will block for the duration of time required to send
//   the data that is passed in.
// The "finished" flag should be set when called with the final chunk to be
//   transferred to terminate the transfer properly
bool emAfSendXmodemData(const uint8_t *data, int length, bool finished);
