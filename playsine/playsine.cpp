/******************************************/
/*
  playsaw.cpp
  by Gary P. Scavone, 2006

  This program will output sawtooth waveforms
  of different frequencies on each channel.
*/
/******************************************/

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include "RtAudio.h"

// Platform-dependent sleep routines.
#if defined( WIN32 )
  #include <windows.h>
  #define SLEEP_MS( milliseconds ) Sleep( (DWORD) milliseconds ) 
#else // Unix variants
  #include <unistd.h>
  #define SLEEP_MS( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

// Interrupt handler function
bool done;
static void finish( int /*ignore*/ ){ 
  done = true; 
}

void usage(void)
{
  // Error function in case of incorrect command-line
  // argument specifications
  std::cout << "\nuseage: playsine <freq_sine_left=440.0> <freq_sine_right=880.0> <amp_left=0.8> <amp_right=0.8>\n";
  std::cout << "    where :\n";
  std::cout << "    freq_sine_left  = frequency of the sine to play on left channel  (< 22050)\n";
  std::cout << "    freq_sine_right = frequency of the sine to play on right channel (< 22050)\n";
  std::cout << "    amp_left        = amplitude of left channel (between 0.0 and 1.0)\n";
  std::cout << "    amp_right       = amplitude of right channel (between 0.0 and 1.0)\n";
  exit(1);
}

void errorCallback(RtAudioErrorType /*type*/, const std::string &errorText)
{
  // This example error handling function simply outputs the error message to stderr.
  std::cerr << "\nerrorCallback: " << errorText << "\n\n";
}


class CallbackData
{
public:
  unsigned int nb_channels = 2;
  double sample_rate = 44100;

  double sine_freq[2] = {440, 880};
  double amplitude[2] = {0.8, 0.8};
};



// Audio callback function. Buffers are interleaved
int rtaudio_callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                     double streamTime, RtAudioStreamStatus status, void *data)
{
  (void)(inputBuffer); // unused
  (void)(streamTime);  // unused

  unsigned int i, j;
  double *buffer = (double *)outputBuffer;

  // répération des données du callback par cast de pointeur
  CallbackData *callbackData = (CallbackData *)data;

  if (status) {
    std::cout << "Stream underflow detected!" << std::endl;
    return 0;
  }

  for (i = 0; i < nBufferFrames; i++)
  {
    for (j = 0; j < callbackData->nb_channels; j++)
    {
      *buffer = 0. ; // @TODO générer l'échantillon voulu du signal sinusoidal, voie droite ou gauche
      buffer++;
    }
    }

  return 0;
}

int main(int argc, char *argv[])
{
  CallbackData callbackData;
  unsigned int bufferFrames;

  // minimal command-line checking
  if (argc > 5)
  {
    usage();
  }
  if (argc >= 2)
  {
    callbackData.sine_freq[0] = atof(argv[1]);
  }
  if (argc >= 3)
  {
    callbackData.sine_freq[1] = atof(argv[2]);
  }
  if (argc >= 4)
  {
    callbackData.amplitude[0] = atof(argv[3]);
  }
  if (argc >= 5)
  {
    callbackData.amplitude[1] = atof(argv[4]);
  }

  if (callbackData.sine_freq[0] > 22050 || callbackData.sine_freq[1] > 22050 || callbackData.amplitude[0] < 0. || callbackData.amplitude[0] > 1. || callbackData.amplitude[1] < 0. || callbackData.amplitude[1] > 1.)
  {
    usage();
  }

  // Specify our own error callback function.
  RtAudio dac(RtAudio::UNSPECIFIED, &errorCallback);

  if (dac.getDeviceCount() < 1)
  {
    std::cout << "\nNo audio devices found!\n";
    exit(1);
  }
  // Let RtAudio print messages to stderr.
  dac.showWarnings(true);

  // Set our stream parameters for output only.
  bufferFrames = 512;
  RtAudio::StreamParameters oParams;
  oParams.deviceId = dac.getDefaultOutputDevice();
  oParams.nChannels = callbackData.nb_channels; // 2 channels
  oParams.firstChannel = 0; // offset

  RtAudio::StreamOptions options;
  options.flags = RTAUDIO_HOG_DEVICE;
  options.flags |= RTAUDIO_SCHEDULE_REALTIME;

  if (dac.openStream(&oParams, NULL, RTAUDIO_FLOAT64, callbackData.sample_rate, &bufferFrames, &rtaudio_callback, (void *) & callbackData, &options))
  {
    std::cout << dac.getErrorText() << std::endl;
    if (dac.isStreamOpen())
    {
      dac.closeStream();
      return 1;
    }
  }
  if (dac.isStreamOpen() == false)
  {
      return 1;
  }

  // Stream is open ... now start it.
  if ( dac.startStream() ) 
  {
      std::cout << dac.getErrorText() << std::endl;
      dac.closeStream();
      return 1;
  }

  //std::cout << "Stream latency = " << dac.getStreamLatency() << "\n" << std::endl;
  std::cout << "\nPlaying ... quit with Ctrl-C (buffer size = " << bufferFrames << ").\n";

  // Install an interrupt handler function.
  done = false;
  (void) signal(SIGINT, finish);

  while ( !done && dac.isStreamRunning() ) SLEEP_MS( 100 );

  // Block released ... stop the stream
  if ( dac.isStreamRunning() ) 
  {
    dac.stopStream();  // or could call dac.abortStream();
  }

  if ( dac.isStreamOpen() ) dac.closeStream();

  return 0;
}
