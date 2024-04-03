#include "../../include/regulator.h"
#define MAX (10)
#define MIN (-10)

void initialize_regulator(Regulator_t *regulator, PI_t *pi, PID_t *pid, ModeMonitor_t *modeMon)
{
    regulator->pi = pi;
    regulator->pid = pid;
    regulator->modeMon = modeMon;
    regulator->should_run = true;
    pthread_mutex_init(&(regulator->mutex_pi), NULL);
    pthread_mutex_init(&(regulator->mutex_pid), NULL); // Needs to be destroyed!

    /* AnalogIn and AnalogOut is needed here for access to the hardware!
     * Function should maybe return 1 or 0 for error checking in main.
     */
}

// void setOpCom(OpCom opCom);

void set_reference_generator(Regulator_t *regul, ReferenceGenerator_t *refgen)
{
    regul->refgen = refgen;
}

/* void sendDataToOpCom(double yRef, double y, double u);
 * void setInnerParameters(PIParameters p);
 * PIParameters getInnerParameters();
 * void setOuterParameters(PIDParameters p);
 * PIDParameters getOuterParameters();
 */

void shutDown(Regulator_t *regulator)
{
    regulator->should_run = false;
}

double limit(double v)
{
    if (v < MIN)
        v = MIN;
    else if (v > MAX)
        v = MAX;
    return v;
}

void run_regulator(Regulator_t *regulator)
{
    long duration;
    clock_t begin = clock();

    int previous = getMode(regulator->modeMon);
    int current;

    double yRef = 0;
    double y_position = 0;
    double y_angle = 0;
    double u_1 = 0;
    double u_2 = 0;

    while (regulator->should_run) // Is something similar to interrupted() neccessary here?
    {
        switch (current = getMode(regulator->modeMon))
        {
            if (previous != current)
            {
                resetPI(regulator->pi);
                resetPID(regulator->pid);
            }

        case OFF:

            yRef = 0;
            y_position = 0;
            u_2 = 0;

            //writeOutput(u_2);

            break;

        case BEAM:

            // y_angle = readInput(analogInAngle); Needs AnalogIn
            // yRef = getRef(refgen); Needs ReferenceGenerator

            pthread_mutex_lock(&(regulator->mutex_pi));
            u_2 = limit(calculateOutputPI(regulator->pi, y_angle, yRef));
            // writeOutput(u_2); Needing working writeOutput()
            updateStatePI(regulator->pi, u_2);
            pthread_mutex_unlock(&(regulator->mutex_pi));

            break;

        case BALL:

            // y_position = readInput(analogInPosition); Needs AnalogIn
            // yRef = getRef(refgen); Needs ReferenceGenerator

            pthread_mutex_lock(&(regulator->mutex_pid));

            u_1 = calculateOutputPID(regulator->pid, y_position, yRef);
            
            pthread_mutex_lock(&(regulator->mutex_pi));

            // y_angle = readInput(analogInAngle); Needs AnalogIn
            u_2 = limit(calculateOutputPI(regulator->pi, y_angle, u_1));
            // writeOutput(u_2); Needing working writeOutput()
            updateStatePI(regulator->pi, u_2);

            pthread_mutex_lock(&(regulator->mutex_pi));

            updateStatePID(regulator->pid, u_1);

            pthread_mutex_unlock(&(regulator->mutex_pid));
            break;

        default:
            printf("Error: Illegal mode.");
            break;
        }

        previous = getMode(regulator->modeMon);
        // sendDataToOpCom(yRef, y_position, u_2); Needing function implementation

        clock_t end = clock();
        duration = (end - begin) / (long)CLOCKS_PER_SEC;
        if (duration > 0)
        {
            sleep(duration);
        }
        else
        {
            printf("Lagging behind...");
        }
    }

    // writeOutput(0.0); Needing working writeOutput()
    pthread_mutex_destroy(&(regulator->mutex_pi));
    pthread_mutex_destroy(&(regulator->mutex_pid));
}


// Writes the control signal u to the output channel: analogOut
// @throws: IOChannelException

/* private void writeOutput(double u)
* {
*    try
*    {
*        analogOut.set(u);
*    }
*    catch (IOChannelException e)
*    {
*        e.printStackTrace();
*    }
* } 
*/

// Reads the measurement value from the input channel: in
// @throws: IOChannelException

/* private double readInput(AnalogIn in)
* {
*    try
*    {
*        return in.get()
*    }
*    catch (IOChannelException e)
*    {
*        e.printStackTrace();
*        return 0.0;
*    }
* }
*/