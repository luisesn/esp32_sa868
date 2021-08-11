#ifndef __wav_file_reader_h__
#define __wav_file_reader_h__

#include <SPIFFS.h>
#include <FS.h>

class WAVFileReader {
  private:
    int m_num_channels;
    int m_sample_rate;
    File m_file;

  public:
    WAVFileReader();
    ~WAVFileReader();
    void init(const char *file_name);
    int sampleRate() {
      return m_sample_rate;
    }
    void getFrames(uint16_t *frames, int number_frames);
};

#endif
