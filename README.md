# arduino-issues
Repo for sketches that (attempt) to reproduce issues our team report with arduinos


## SAMD21

### Issue00 - DigitalRead

One of the team reports that digitalRead is not working correctly on a pin on the analogue port, when the ADC is enabled. 

#### Basic read write
First let's try a basic digital read and write from digital port to analogue port, with no code relating to analogue functions. This requires two wires


 - D2 -> A0
 - D3 -> A1

[sketch](./issue00/basic-read-write/basic-read-write.ino)

Example output after running for a few minutes:

```
Errors 0.00/277290.00 attempts [01->01]
Errors 0.00/277291.00 attempts [10->10]
Errors 0.00/277292.00 attempts [01->01]
```

No problems here.

#### Read write with an analogue input specified

Now cause the ADC to be enabled by doing an analogRead.

[sketch](./issue00/adc-enabled-read-write/adc-enabled-read-write.ino)

Example output after running for a minute or so 

```
Errors 0.00/31789.00 attempts [10->10]
Errors 0.00/31790.00 attempts [01->01]
Errors 0.00/31791.00 attempts [10->10]
```
Does not seem to be an issue.

Interim view -> I don't seem to be able to reproduce the problem so far.
