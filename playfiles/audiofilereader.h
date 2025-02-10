#ifndef AUDIOFILEREADER_H
#define AUDIOFILEREADER_H

#include <string>
#include <sndfile.h>


class AudioFileReader {
public:
    AudioFileReader(std::string filePath);
    ~AudioFileReader();

    std::string filePath() const {
        return m_filePath;
    }

    int nbFrames() const;

    void rewind() ;

    // position the file pointer so that next frame that will be red with readNextFrames() 
    // is frame number newFrame
    void seek(int newFrame) ;

    // frames should point to a buffer of at least arraySize size, already allocated
    // returns the number of frames that could be read
    int readNextFrames(double * frames, int arraySize);

    // frames should point to a buffer of at least nbFrames() size, already allocated
    // returns the number of frames that could be read (should be nbFrames())
    int readAllFrames(double * frames);

private:
    std::string m_filePath;
    SNDFILE * m_sndFile;
    SF_INFO m_sndFileInfo;
};


#endif // AUDIOFILEREADER_H
