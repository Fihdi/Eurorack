#ifndef SimpleSampler_FileScanner_h
#define SimpleSampler_FileScanner_h

#include <stdio.h>
#include <string.h>
#include "daisy_seed.h"
#include "./constants.h"
#include "./wavFormat.h"
#include "./SampleMetadata.h"
#include "./MemoryManager.h"
#include "./SamplePlayer.h"
#include "./debug.h"


#define MAX_CWD_LENGTH 1024
#define NULL_CHAR '\0'


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
// NOTA: FIL needs to be declared as static, or defined in global scope
static FIL file;



//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
class FilesScanner {
private:

  char _cwd[MAX_CWD_LENGTH]; // current working directory path
  int  _cwdHead;
  int  _cwdHeadPrev;



  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  bool _isWavExtention (const char* fileName) {
    int i = 0;

    // Go to the end of file name
    while (fileName[i] != NULL_CHAR) ++i;
    if (i <= 4) return false;

    // Go back 4 characters and check these are equal to ".wav"
    i -= 4;
    if (fileName[i] != '.') return false;
    if (fileName[i + 1] != 'w' && fileName[i + 1] != 'W') return false;
    if (fileName[i + 2] != 'a' && fileName[i + 2] != 'A') return false;
    if (fileName[i + 3] != 'v' && fileName[i + 3] != 'V') return false;
    return true;
  }


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  bool _pushFilePathInCwd (const char* fileName) {
    if (_cwdHeadPrev != _cwdHead) return false;
    _cwdHeadPrev = _cwdHead;
    _cwd[_cwdHead++] = '/'; // NOTE: replacing NULL_CHAR with '/'

    int i = 0;

    while (fileName[i] != NULL_CHAR) {
      _cwd[_cwdHead++] = fileName[i++];
      if (_cwdHead > MAX_CWD_LENGTH) {
        _popCwd();
        return false;
      }
    }

    _cwd[_cwdHead++] = NULL_CHAR;

    return true;
  }


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void _popCwd () {
    _cwdHead = _cwdHeadPrev;
    _cwd[_cwdHead] = NULL_CHAR;
  }


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  bool _loadWavFileToMemory (MemoryManager* memoryManager, SamplePlayer* samplePlayer) {
    SampleMetadata* sampleMetadata = samplePlayer->GetNextEmptySlot();

    // Read wav header
    if (!ReadWavMetaData(&file, sampleMetadata)) return false;

    // Check if we still have enough memory to load this sample
    size_t sampleLength = sampleMetadata->sampleLength;
    if (!memoryManager->CanLoadSample(sampleLength)) {
      return false;
    }

    // Load sample data to SDRAM
    int16_t* buffer  = memoryManager->GetBufferHead();
    UINT bytesToRead = sampleLength * BYTE_PER_SAMPLE;
    UINT bytesRead;

    if (f_read(&file, buffer, bytesToRead, &bytesRead) != FR_OK) {
      return false;
    }

    // Check if we read the expected amount of bytes
    if (bytesToRead != bytesRead) return false;

    // Save buffer, update memory head pointer and confirm sample added
    sampleMetadata->buffer = buffer;
    memoryManager->IncrementHeadPointer(sampleLength);
    samplePlayer->AddSample();
    return true;
  }


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  bool _loadFile (MemoryManager* memoryManager, SamplePlayer* samplePlayer) {
    // Open file at current working path
    if (f_open(&file, _cwd, (FA_OPEN_EXISTING) | (FA_READ)) != FR_OK) {
      return false;
    }

    bool success = _loadWavFileToMemory(memoryManager, samplePlayer);

    f_close(&file);

    return success;
  }


public:

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void Init (Hardware* hardware) {
    _cwdHead = 0;
    strcpy(_cwd, hardware->GetSdCardPath()); // NOTE: root path of SD Card is "0:"
    while (_cwd[_cwdHead] != NULL_CHAR) ++_cwdHead;
    _cwdHeadPrev = _cwdHead;
  }


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  bool ScanDirectory (MemoryManager* memoryManager, SamplePlayer* samplePlayer) {
    DIR     directory;
    FILINFO fileInfo;


    if (f_opendir(&directory, _cwd) != FR_OK) {
      return false;
    }

    while (true) {
      if (f_readdir(&directory, &fileInfo) != FR_OK) break; // something went wrong
      if (fileInfo.fname[0] == NULL_CHAR)            break; // end of directory
      if (fileInfo.fattrib & (AM_HID))      continue; // skip hidden files
      if (fileInfo.fattrib & (AM_DIR))      continue; // skip directories
      if (!_isWavExtention(fileInfo.fname)) continue; // skip non-wav files

      // load file
      if (!_pushFilePathInCwd(fileInfo.fname)) continue;
      debug.PrintString(_cwd);
      _loadFile(memoryManager, samplePlayer);
      _popCwd();

      if (samplePlayer->IsSampleCountMaxed()) break;
    }

    f_closedir(&directory);
    return true;
  }

};


#endif // SimpleSampler_FileScanner_h
