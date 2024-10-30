#ifndef SimpleSampler_wavFormat_h
#define SimpleSampler_wavFormat_h


#include <fatfs.h>
#include "./SampleMetadata.h"


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
/*
Header format for wave files.

For more infos about the wave file format:
http://soundfile.sapp.org/doc/WaveFormat/

byteRate         == sampleRate * nChannels * bitsPerSample / 8
blockAlign       == nChannels * BitsPerSample / 8
                               The number of bytes for one sample including
                               all channels.
BitsPerSample    8 bits = 8, 16 bits = 16, etc.

*/


#define FMT_SIZE 16 // NOTE: For our purpose, it's alway 16. More means extended
#define BYTE_PER_SAMPLE 2 // we only support 16 bits audio


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
// NOTA: IDs are big-endian encoded (i.e. string is reversed)
#define CHUNK_ID_RIFF 0x46464952 // RIFF
#define CHUNK_ID_WAVE 0x45564157 // WAVE
#define CHUNK_ID_fmt  0x20746D66 // fmt█
#define CHUNK_ID_data 0x61746164 // data
#define PCM_AUDIO     0x0001


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
typedef struct {
  uint32_t RIFF;          // "R I F F"
  uint32_t fileSize;      // file size, minus 8 bytes (the first 2 fields)
  uint32_t WAVE;          // "W A V E"
  uint32_t FMT;           // "f t m █"
  uint32_t fmtChunkSize;  // fmt chunk size
  uint16_t audioFormat;   // needs to be PCM
  uint16_t nChannels;     // number of channels
  uint32_t sampleRate;    // sampleRate, in Hz
  uint32_t byteRate;      // byteRate = sampleRate * nChannels * bitsPerSample/8
  uint16_t blockAlign;    // blockAlign = nChannels * bitsPerSample/8
  uint16_t bitsPerSample; // bit per sample, usually 16
  uint32_t DATA;          // "d a t a"
  uint32_t dataChunkSize; // data chunk size, in byte
} RiffChunk;



//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
extern bool ReadWavMetaData (FIL* file, SampleMetadata* sampleMetadata) {
  RiffChunk riffChunk;
  UINT      bytesRead;


  // Read RIFF chunk
  if (f_read(file, (void *)&riffChunk, sizeof(RiffChunk), &bytesRead) != FR_OK) {
    return false;
  }

  // Ensure RIFF format is correct and supported (PCM, 16 bits, mono)
  if (riffChunk.RIFF != CHUNK_ID_RIFF)    return false;
  if (riffChunk.WAVE != CHUNK_ID_WAVE)    return false;
  if (riffChunk.FMT  != CHUNK_ID_fmt)     return false;
  if (riffChunk.fmtChunkSize != FMT_SIZE) return false;
  if (riffChunk.audioFormat != PCM_AUDIO) return false;
  if (riffChunk.nChannels != 1)           return false;
  if (riffChunk.bitsPerSample != 16)      return false;
  if (riffChunk.DATA != CHUNK_ID_data)    return false;

  // Store DATA chunk position and sample length
  sampleMetadata->dataStart    = file->fptr;
  sampleMetadata->sampleLength = riffChunk.dataChunkSize / BYTE_PER_SAMPLE;

  return true;
}


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
#endif // SimpleSampler_wavFormat_h
