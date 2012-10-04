Photon-DA AES Implementation
============================

This is the AES implementation used in our [2012 CHES Paper](http://nedos.net/ches2012.pdf) and subsequent works. The AES implementation is based around the [AVR-Crypto-Lib](http://www.das-labor.org/wiki/AVR-Crypto-Lib).

Compiling
---------

There are a couple of compile targets you may want to use at compile time:

    photon

Will only perform the first round of AES.

    verbose

Will output the status of the computation while computing.

    debug

Will output additional debug information about the computation.

Macros
------

Additionally there are a couple of macros:

    DELAY

Specifies the delay in milliseconds between consecutive computations.

    STARTUP

Specifies a startup delay before the initial computation.

    ROUNDS

Specifies the amount of rounds per plaintext before the plaintext byte is incremented. The default is 0, which means the plaintext byte is stored in NVM and incremented on startup, meaning the chip can simply be reset to increment the plaintext byte.
