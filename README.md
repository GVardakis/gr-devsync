## GR-DEVSYNC
gr-devsync is an Out Of Tree module of GNURadio, designed to provide phase synchronization for multiple SDR devices. 

# Description
The goal of this project is to provide a low-cost, extendable solution to phase synchronizing multiple SDR devices for use in MIMO scenarios. Apart from the antenna array to  be synchronized, gr-devysnc also requires an SDR device with transmit capability. A known signal is transmitted, and received by all elements of the antenna array. Using crosscorrelation with the known sequence at the receiver, we detect the arrival of the signal at all channels, and provided that the signal is received at (almost) the same time from each one, we compensate digitally for the phase offset between them. For the purpose of this project, the long training sequence of the IEEE802.11 protocol is being used as the transmitted known signal. Also, it is assumed that the RX devices are synchronized in frequency.

# Usage
The transmitting element is placed at the far field side of the antenna array so that the signal arrives at the array as a plane wave. Also, it must be placed at a direction, such that the distance of the transmitting antenna to each of the receiving elements, is almost equal. For example, if the array is a linear array, the transmitting antenna should be placed at azimuth 90 and elevation 0, so that the signal arrives at the same time at all receiving elements.

After the start of execution, a (user-defined) number of samples is discarded, and after, the noise floor estimation takes place. For this reason, and depending on the values defined by the user in the corresponding blocks, the transmission of the signal should occur a few seconds after the start of execution.

Furthermore, an RMS normalization method is used to bring the receiving signals of the different channels on the same level, in order to compensate for possible amplitude offsets, which degrade the performance of the synchronizer. 

Please refer to the examples folder for a sample flowgraph of how the different blocks are combined.

# Dependencies
Apart from the dependencies of gnuradio, gr-devsync also requires the Armadillo library to build.

# Installation
gr-devsync uses the cmake tool for generating the appropriate Makefiles

First create a build directory


```sh
$ mkdir build
```

Execute the cmake command with the appropriate arguments inside the build directory

```sh
$ cmake ..
```

And finally build the project

```sh
$ make
$ sudo make install
```

# Contact
For any information you will find me on the gnuradio and usrp lists, or email me at vardakis@csd.uoc.gr