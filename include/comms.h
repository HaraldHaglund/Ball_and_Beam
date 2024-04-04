#ifndef COMMS_H
#define COMMS_H

int putMeasurementData(double t, double yRef, double y);
int putControlData(double t, double u);
int startOpcom();

#endif