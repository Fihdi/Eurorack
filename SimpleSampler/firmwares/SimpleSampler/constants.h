#ifndef SimpleSampler_constants_h
#define SimpleSampler_constants_h


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
#define MAX_MEMORY       (64 * (1 << 20)) // 64 MB of SDRAM
#define SAMPLE_SIZE      2 // 2 bytes per sample (int16_t)
#define BUFFER_SIZE      (MAX_MEMORY / SAMPLE_SIZE)
#define SAMPLE_RATE      48000 // allowed values: 8k, 16k, 32k, 48k, 96k
#define AUDIO_BLOCK_SIZE 16    // Block size can be any number up to 256


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
#define MAX_SAMPLES_COUNT 32
#define ALLOW_DEBUG false



#endif // SimpleSampler_constants_h