# arduino-issues
Repo for sketches that (attempt) to reproduce issues our team report with arduinos


## SAMD21

### Issue00 - DigitalRead

One of the team reports that digitalRead is not working correctly on a pin on the analogue port, when the ADC is enabled. They've since identified it only occurs if there is a mix of digitalRead and analogRead being performed on the same pin.

**Thoughts:** would be a cleaner design if it stayed in only the digital or analogue domain - could convert an analogue value to truthiness when required, or derive an analogue value from tracking signal transitions and their timing, inferring the duty cycle and hence analogue value.  Of course, there may also be a way to unpick which registers are set/unset to change mode from digitalRead to analogRead, and issue these register settings directly. This has the side effect of the making the code unportable to other micros.

**As a possible fix staying in analogue domain:** read only analogue values, and if you need to know whether the signal is "truthy", do a conversion to bool by comparing to a threshold. 

```
int thresh; // global

thresh = 511; // in setup

// in main loop
ci = analogRead(15);
bi = ci >= thresh;
```  

Read on for details of the attempts to reproduce and fix the problem.

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


#### Do both digital and analog read on same pin

This is the issue! See [sketch](./issue00/switch-read-write/switch-read-write.ino)

Essentially, read a pin both ways (as to why we want to do this - is a separate issue!)

```
bi = digitalRead(15);
ci = analogRead(15);
```

```
Errors 3096.00/6192.00 attempts [01->00] (ignore: 1023)
Errors 3096.00/6193.00 attempts [10->10] (ignore: 2)
Errors 3097.00/6194.00 attempts [01->00] (ignore: 1023)
Errors 3097.00/6195.00 attempts [10->10] (ignore: 0)
```

### Mitigation

a/ if high speed digital line, is analogue read needed? Perhaps using hardware to count pulses or measure duty cycle would be more appropriate.

b/ if must do analogue read but also want a truthy value, consider just doing analogRead and converting to `bool`

e.g. [sketch](./issues00/mitigate/mitigate.ino)

```
int thresh; // global

thresh = 511; // in setup

// in main loop
ci = analogRead(15);
bi = ci >= thresh;
```  

Now we can do both:

```
Errors 0.00/26390.00 attempts [01->01] (ignore: 1023)
Errors 0.00/26391.00 attempts [10->10] (ignore: 1)
Errors 0.00/26392.00 attempts [01->01] (ignore: 1023)
Errors 0.00/26393.00 attempts [10->10] (ignore: 3)
Errors 0.00/26394.00 attempts [01->01] (ignore: 1023)
Errors 0.00/26395.00 attempts [10->10] (ignore: 0)
Errors 0.00/26396.00 attempts [01->01] (ignore: 1023)
Errors 0.00/26397.00 attempts [10->10] (ignore: 1)
Errors 0.00/26398.00 attempts [01->01] (ignore: 1023)
Errors 0.00/26399.00 attempts [10->10] (ignore: 0)
Errors 0.00/26400.00 attempts [01->01] (ignore: 1021)
Errors 0.00/26401.00 attempts [10->10] (ignore: 0)
```

