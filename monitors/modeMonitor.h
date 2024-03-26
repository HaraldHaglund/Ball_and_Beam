#ifndef MODEMONITOR_H
#define MODEMONITOR_H

//use key 'M' when accesing shared memory
struct mode_monitor
{
    //OFF = 0, BEAM = 1, BALL = 2
    volatile int mode;
};

#endif