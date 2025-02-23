#ifndef AUDIOCALLBACK_H
#define AUDIOCALLBACK_H

#include <RtAudio.h>
#include "CallbackData.h"


// RTAudio callback fundtion. 
// Buffers are interleaved
int rtaudio_callback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                      double streamTime, RtAudioStreamStatus status, void *data );



// RTAudio error callback function
void errorCallback( RtAudioErrorType /*type*/, const std::string &errorText );



#endif // AUDIOCALLBACK_H
