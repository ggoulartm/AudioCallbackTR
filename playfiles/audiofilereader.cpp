#include <iostream>
#include <string.h>
#include "audiofilereader.h"


AudioFileReader::AudioFileReader(std::string filePath){
    m_filePath = filePath;

    m_sndFileInfo.format = 0;
    m_sndFile = sf_open(filePath.c_str(), SFM_READ, &m_sndFileInfo);

    if( ! m_sndFile) {
        std::cout << "cannot allocate AudioFileReader : cannot open file " << filePath << std::endl;
        throw new std::string("Invalid file");
    }

    // can only read files with 44100 sample rate
    if(m_sndFileInfo.samplerate != 44100) {
        sf_close (m_sndFile) ;
        std::cout << "cannot allocate AudioFileReader : invalid sample rate in file " << filePath << std::endl;
        throw new std::string("Invalid sample rate");
    }

    // can only read monophonic files (1 channel)
    if(m_sndFileInfo.channels != 1) {
        sf_close (m_sndFile) ;
        std::cout << "cannot allocate AudioFileReader : non-monophonic file " << filePath << std::endl;
        throw new std::string("Invalid number of channels");
    }
}

AudioFileReader::~AudioFileReader() {
    sf_close (m_sndFile) ;
}

int AudioFileReader::nbFrames() const {
    return m_sndFileInfo.frames;
}


void AudioFileReader::rewind() {
    sf_seek(m_sndFile, 0, SEEK_SET);
}

void AudioFileReader::seek(int newFrame) {
    sf_seek(m_sndFile, newFrame, SEEK_SET);
}


int AudioFileReader::readNextFrames(double * frames, int arraySize) {
    if(sf_seek(m_sndFile, 0, SEEK_CUR) == nbFrames()) {
        return 0;
    }
    int nb_read = sf_read_double (m_sndFile, frames, arraySize);
    //  std::cout << "AudioFileReader::readNextFrames nbread " << nb_read << " curframe " << sf_seek(m_sndFile, 0, SEEK_CUR) << " nb frames " << m_sndFileInfo.frames << std::endl;
    return nb_read;
}

// frames should point to a buffer of at least nbFrames() size, already allocated
int AudioFileReader::readAllFrames(double * frames) {
    size_t curpos = sf_seek(m_sndFile, 0, SEEK_CUR);
    this->rewind();
    int nb_read = sf_read_double (m_sndFile, frames, nbFrames());
    this->seek(curpos);
    return nb_read;
}
