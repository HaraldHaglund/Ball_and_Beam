#ifndef COMMS_H
#define COMMS_H

int putMeassurementData(double t, double yRef, double y);
int putControlData(double t, double u);
int startOpcom();
int startRegul();

#endif