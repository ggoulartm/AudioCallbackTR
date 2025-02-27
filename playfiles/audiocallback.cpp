#include "audiocallback.h"
#include <unistd.h>



// Audio callback fundtion. Buffers are interleaved
int rtaudio_callback( void *outputBuffer,
                      void *inputBuffer, unsigned int nBufferFrames,
                      double streamTime, RtAudioStreamStatus status,
                      void *data ) {
  (void)(inputBuffer); // unused
  (void)(streamTime);  // unused

  unsigned int i, j;

  double *buffer = (double *) outputBuffer;
  CallbackData * callbackData = (CallbackData *) data;

  if ( status ) {
    std::cout << "Stream underflow detected!" << std::endl;
    return 0;
  }

  for ( i=0; i<nBufferFrames; i++ ) {
      for ( j=0; j<callbackData->nb_channels; j++ ) {
        *buffer = 0;
          // @ TODO write the appopriate samples into the audio stream buffer
          // to mix the callbackData->samples.size() audio files contents.
          for(size_t numberOfFiles = 0; numberOfFiles < callbackData->samples_size.size(); numberOfFiles++){
            if(callbackData->frameCounter >= callbackData->samples_size[numberOfFiles]) continue;
            double sample = callbackData->dequeue(numberOfFiles);
            if(sample == NULL) sample = 0;
            *buffer += sample/callbackData->samples.size();
          } 
          buffer ++;
      }
      callbackData->frameCounter ++;
    }


  return 0;
}



void errorCallback( RtAudioErrorType /*type*/, const std::string &errorText )
{
  // This example error handling function simply outputs the error message to stderr.
  std::cerr << "\nerrorCallback: " << errorText << "\n\n";
}
