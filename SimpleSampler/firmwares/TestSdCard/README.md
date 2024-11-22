# Blink

Test SD Card:

- Try to access the SD Card.
- Try to write a `test.txt` file onto the SD Card.
- Read back the written text file and check that data are correct.
- Panel LED should show the status of the test (c.f. codes below)


## Error

| LED state    | error
| ------------ | --------------------
| Solid ON     | SD initialization failed
| Solid OFF    | SD access failed
| 1 blink      | File write failed
| 2 blinks     | File read failed
| 3 blinks     | Data incorrect
| triangle LFO | Success !

