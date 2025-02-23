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
#include <assert.h>
#include <signal.h>

#include <vector>
#include <RtAudio.h>

#include "audiofilereader.h"
#include "audiocallback.h"

// Platform-dependent sleep routines.
#if defined(WIN32)
#include <windows.h>
#define SLEEP_MS(milliseconds) Sleep((DWORD)milliseconds)
#else // Unix variants
#include <unistd.h>
#define SLEEP_MS(milliseconds) usleep((unsigned long)(milliseconds * 1000.0))
#endif
#include <thread>

// Interrupt handler function
bool done;
static void finish(int /*ignore*/)
{
  done = true;
}

void usage(void)
{
  // Error function in case of incorrect command-line
  // argument specifications
  std::cout << "\nuseage: playsine audioFile#1 <audioFile#2> ... \n";
  std::cout << "    where :\n";
  std::cout << "    audioFile#x  = one of the audio file to mix, wav or aiff format, One channel, 44100 Hz\n";
  exit(1);
}

int main(int argc, char *argv[])
{
  CallbackData callbackData;
  unsigned int bufferFrames;

  std::vector<std::string> audiofilepaths;
  std::vector<AudioFileReader *> audiofilereaders;

  // => Déplacé en attribut de la classe CallbackData 
  // pour pouvoir être accessible depuis la fonction callback audio !
  // // a vector of int, that correspond to the number 
  // // of elements of each of the array of double below
  // std::vector<int> samples_size;
  // // a vector of array of double
  // std::vector<double *> samples;

  // minimal command-line checking
  if (argc <= 1)
  {
    usage();
  }

  audiofilepaths.reserve(argc - 1);
  audiofilereaders.reserve(argc - 1);

  for (int i = 1; i < argc; i++)
  {
    audiofilepaths.push_back(argv[i]);
  }

  for (std::string file : audiofilepaths)
  {
    std::cout << "file to play : " << file << "\n";
  }

  for (unsigned long fileid = 0; fileid < audiofilepaths.size(); fileid++)
  {
    audiofilereaders.push_back(new AudioFileReader(audiofilepaths[fileid]));
  }

  callbackData.samples_size.reserve(argc - 1);
  callbackData.samples.reserve(argc - 1);

  // Load in memory all the samples of all the audio files
  for (unsigned long fileid = 0; fileid < audiofilepaths.size(); fileid++)
  {
    AudioFileReader *filereader = audiofilereaders[fileid];
    callbackData.samples_size.push_back(filereader->nbFrames());

    auto ReadData =  [](AudioFileReader *filereader, CallbackData *callbackData, unsigned long fileid) {
      double *buffer = new double[filereader->nbFrames()];
      // load the file content ten seconds per ten seconds
      int nbReadTotal = filereader->readAllFrames(buffer);
      assert(nbReadTotal == callbackData->samples_size[fileid]);
      
      // we store the adress of the buffer into the vector of pointers to double
      for(int el = 0; el < filereader->nbFrames(); el++){
        callbackData->queue(buffer+el, fileid);
      }  
    };
    
    std::thread t(ReadData, filereader, &callbackData, fileid);
    t.detach();
  }


  std::cout << "Number of samples found in each file :" << std::endl;
  for (unsigned long fileid = 0; fileid < audiofilepaths.size(); fileid++)
  {
    std::cout << "    * " << audiofilepaths[fileid] << " : " << callbackData.samples_size[fileid] << " samples " << std::endl;
  }

  sleep(1);
  std::cout << std::endl;


  // Audio stream management

  // Specify our own error callback function.
  RtAudio dac(RtAudio::UNSPECIFIED, &errorCallback);

  if (dac.getDeviceCount() < 1)
  {
    std::cout << "\nNo audio devices found!" << std::endl;
    // Oups, ce exit() était oublié...
    for (unsigned long i = 0; i < audiofilepaths.size(); i++) {
      delete audiofilereaders[i];
    }
    exit(1);
  }

  // Let RtAudio print messages to stderr.
  dac.showWarnings(true);

  // Set our stream parameters for output only.
  bufferFrames = 1024;
  RtAudio::StreamParameters oParams;
  oParams.deviceId = dac.getDefaultOutputDevice();
  oParams.nChannels = callbackData.nb_channels;
  oParams.firstChannel = 0; // offset

  RtAudio::StreamOptions options;
  options.flags = RTAUDIO_HOG_DEVICE;
  options.flags |= RTAUDIO_SCHEDULE_REALTIME;

  // An error in the openStream() function can be detected either by
  // checking for a non-zero return value OR by a subsequent call to
  // isStreamOpen().
  if (dac.openStream(&oParams, NULL, RTAUDIO_FLOAT64, callbackData.sample_rate, &bufferFrames, &rtaudio_callback, (void *)&callbackData, &options))
  {
    std::cout << dac.getErrorText() << std::endl;
    for (unsigned long i = 0; i < audiofilepaths.size(); i++)
    {
      delete audiofilereaders[i];
    }
    dac.closeStream();
    exit(1);
  }

  if (dac.isStreamOpen() == false)
  {
    std::cout << "stream could not be opened" << std::endl;
    for (unsigned long i = 0; i < audiofilepaths.size(); i++)
    {
      delete audiofilereaders[i];
    }
    dac.closeStream();
    exit(1);
  }

  // Stream is open ... now start it.
  if (dac.startStream())
  {
    std::cout << dac.getErrorText() << std::endl;
    std::cout << "stream could not be started" << std::endl;
    for (unsigned long i = 0; i < audiofilepaths.size(); i++)
    {
      delete audiofilereaders[i];
    }
    dac.closeStream();
    exit(1);
  }

  //  std::cout << "Stream latency = " << dac.getStreamLatency() << std::endl;
  std::cout << "\nPlaying ... press <enter> to quit (buffer size = " << bufferFrames << ")." << std::endl;

  done = false;
  (void)signal(SIGINT, finish);

  while (!done && dac.isStreamRunning())
  {
    SLEEP_MS(500);
  }

  if (dac.isStreamRunning())
  {
    dac.stopStream(); // or could call dac.abortStream();
  }

  if (dac.isStreamOpen())
  {
    dac.closeStream();
  }

  for (unsigned long i = 0; i < audiofilepaths.size(); i++)
  {
      delete audiofilereaders[i];
  }
  
  std::cout << "bye bye" <<std::endl;

  return 0;
}
