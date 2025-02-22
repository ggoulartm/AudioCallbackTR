#ifndef AUDIOCALLBACK_H
#define AUDIOCALLBACK_H

#include <RtAudio.h>
#include <vector>


class CallbackData {
public:
  unsigned int nb_channels = 2;
  double sample_rate = 44100;
  // a vector of int, that correspond to the number 
  // of elements of each of the array of double below
  std::vector<int> samples_size;
  // a vector of array of double
  std::vector<double *> samples;
  int frameCounter = 0;

  void print(){
    std::cout << "nb_channels : " << nb_channels << std::endl;
    std::cout << "sample_rate : " << sample_rate << std::endl;
    std::cout << "frameCounter : " << frameCounter << std::endl;
    for(int i = 0; i < samples_size.size(); i++){
      std::cout << "samples_size[" << i << "] : " << samples_size[i] << std::endl;
      for(int j = 0; j < samples.size(); j++){
        std::cout << "samples["<< i <<"]"<< "[" << j << "] : " << samples[i][j] << std::endl;
      }
    }

    std::cout << "---------------------------------" << std::endl;
  }

};



// RTAudio callback fundtion. 
// Buffers are interleaved
int rtaudio_callback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                      double streamTime, RtAudioStreamStatus status, void *data );



// RTAudio error callback function
void errorCallback( RtAudioErrorType /*type*/, const std::string &errorText );



#endif // AUDIOCALLBACK_H
