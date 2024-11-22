# Simple Sampler

Play samples from the SD Card.

- When powered up, the module scan the SD Card for all `wav` files (top level directory only). Then it loads all valid `wav` file to memory, until it reach the `MAX_SAMPLES_COUNT` number of samples, or the memory gets full.
- Valid `wav` file are MONO, 16 bits PCM.


## Interface

| UI          | function
| ----------- | --------------------
| Trigger     | Play current sample
| Encoder     | Cycle thru samples
| Speed       | Playback speed
| Random      | *not used*
| Part/Entire | *not used*
| LED         | Sample playback
