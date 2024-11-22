#include <stdio.h>
#include <string.h>
#include "daisy_seed.h"
#include "../PTAL/hardwares/Hardware.h"


#define FILE_NAME "test.txt"
#define BUFFER_SIZE 512

using namespace daisy;

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
Hardware hardware;
FIL      file;


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
int TestSdCard () {
  char   writeBuffer[BUFFER_SIZE];
  char   readBuffer[BUFFER_SIZE];
  UINT   bytesRead;
  sprintf(writeBuffer, "Daisy...Testing...\n1...\n2...\n3...\n");
  memset(readBuffer, 0, BUFFER_SIZE);
  size_t len = strlen(writeBuffer);

  // Open and write the test file to the SD Card.
  if (f_open(&file, FILE_NAME, (FA_CREATE_ALWAYS) | (FA_WRITE)) != FR_OK) return 1;
  f_write(&file, writeBuffer, len, &bytesRead);
  f_close(&file);

  // Read back the test file from the SD Card.
  if (f_open(&file, FILE_NAME, FA_READ) != FR_OK) return 2;
  f_read(&file, readBuffer, len, &bytesRead);
  f_close(&file);

  // Check that buffers have the same data.
  for (size_t i = 0; i < len; i++) {
    if (readBuffer[i] != writeBuffer[i]) {
      return 3;
    }
  }

  return 0;
}


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
void DisplayError (int error) {
  if (error == 0) return;
  for (int i = 0; i < error; i++) {
    hardware.led.SetSolid(true);
    System::Delay(40);
    hardware.led.SetSolid(false);
    System::Delay(100);
  }
  System::Delay(700);
}


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
int main (void) {
  // initializations
  hardware.Init();
  hardware.led.SetSolid(true);
  System::Delay(200);

  hardware.InitSdCard();
  hardware.led.SetSolid(false);

  int error = TestSdCard();
  System::Delay(400);

  if (error == 0) {
    hardware.led.SetTriangle(700);
  }

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  while (true) {
    hardware.Update();
    DisplayError(error);
  }
}
