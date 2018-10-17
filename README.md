## Baremetel for GAP8

A UART example for gapuino. Send the corresponding byte received.

### Build

**First `cd` into gap_sdk and source sourceme.sh**. And then back to the baremetal directory.

    ./build.sh

### Download and run

Use `plpbridge` from `gap_sdk`

    ....../gap_sdk/install/workstation/bin/plpbridge \
    --cable=ftdi@digilent --boot-mode=jtag --chip=gap \
    --binary=test \
    load ioloop reqloop start wait

The uart is usually `/dev/ttyUSB1`, while `/dev/ttyUSB0` is occupied by JTAG.
