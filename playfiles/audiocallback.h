#ifndef AUDIOCALLBACK_H
#define AUDIOCALLBACK_H

#include <RtAudio.h>
#include <vector>


class CallbackData {
public:
  unsigned int nb_channels = 2;
  double sample_rate = 44100;

  int frameCounter = 0;

};



// RTAudio callback fundtion. 
// Buffers are interleaved
int rtaudio_callback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                      double streamTime, RtAudioStreamStatus status, void *data );



// RTAudio error callback function
void errorCallback( RtAudioErrorType /*type*/, const std::string &errorText );



#endif // AUDIOCALLBACK_H
