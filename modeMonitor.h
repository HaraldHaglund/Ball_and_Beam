#ifndef MODEMONITOR_H
#define MODEMONITOR_H

//use key "MODE" when accesing shared memory
struct Mode_Monitor
{
    //OFF = 0, BEAM = 1, BALL = 2
    volatile int mode;
};

#endif