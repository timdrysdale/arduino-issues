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


#### Many ADC inputs

Let's try increasing the number of analogue inputs. We will ensure the compiler does not optimise them away, by ensuring that the output of each write is being put to "use" (contributing to a sum that is printed over the serial link).

[sketch](./issue00/mostly-analogue-read-write/mostly-analogue-read-write.ino)

With 2 digital inputs, and 6 analogue inputs, the issues is still not presetn.


```
Errors 0.00/17840.00 attempts [01->01] (ignore: 3747)
Errors 0.00/17841.00 attempts [10->10] (ignore: 3661)
Errors 0.00/17842.00 attempts [01->01] (ignore: 3708)
Errors 0.00/17843.00 attempts [10->10] (ignore: 3758)
```

#### No pinMode statements

Perhaps there is a setup issue?

We could try not specifying that the inputs are inputs.

[sketch](./issue00/no-pinmode/no-pinmode.ino)

```
Errors 0.00/12859.00 attempts [10->10] (ignore: 3602)
Errors 0.00/12860.00 attempts [01->01] (ignore: 3719)
Errors 0.00/12861.00 attempts [10->10] (ignore: 3788)
```

Also no problem showing up quickly.
