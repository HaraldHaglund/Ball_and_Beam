import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import tkinter as tk
import numpy as np
import comms as c

# Global variables
dataOut = np.array([])  # TODO: Clean these three arrays!
dataRef = np.array([])
dataCon = np.array([])
tracking_time = 10.0  # seconds
h = 10  # ms
xRef = []
index = 0.0
fps = 10  # Does not work for very high numbers (Becomes laggy and out of sync w time)
squarePeriodTime = 3.0
squareAmp = 5.0
manualAmp = 0
iteration = 0
max_ctrl = 0.2
is_manual = True  # Default selection
is_square = False
is_time_optimal = False
new_period = True
OFF = 0
BEAM = 1
BALL = 2
i = 0
referenceSignal = 0
oldreferenceSignal = 0
K_PHI = 4.5
K_V = 10.0
ts = 0

# PID Inner Values
inner_K, inner_Ti, inner_Tr, inner_beta, inner_H, inner_integratorOn = c.getInnerParameters()

# PID Outer Values
outer_K, outer_Ti, outer_Td, outer_Tr, outer_N, outer_beta, outer_H, outer_integratorOn = c.getOuterParameters()

# Set initial mode to OFF.
c.setMode(OFF)

# Initialize root window
root = tk.Tk()
root.geometry("1280x512")
root.option_add('*Font', ("Courier", 15, "bold"))
root.title("Ball & Beam GUI")

# Create 3 main frames
parameterFrame = tk.Frame(root)
plotFrame = tk.Frame(root)
referenceFrame = tk.Frame(root)

# Define frames as three column grids
parameterFrame.grid(row=0, column=0, sticky="nsew")
plotFrame.grid(row=0, column=1, columnspan=2, sticky="nsew")
referenceFrame.grid(row=0, column=3, sticky="nsew")

# Set column/row weights to allow resizing
root.columnconfigure(0, weight=1)
root.columnconfigure(1, weight=2)
root.columnconfigure(3, weight=1)
root.rowconfigure(0, weight=1)

# Create frame for settings (off,beam,ball and stop)
settings = tk.Frame(parameterFrame)
settings.pack(side=tk.BOTTOM)


def kill_all():
    c.shutDown()
    root.destroy()


# Stop-button
stop = tk.Button(settings, text='Stop', font=('Courier', 20), width=20, command=kill_all)
stop.pack(side=tk.BOTTOM)

# Radio-buttons
options = ['Off', 'Beam', 'Ball']
options_cmd = [OFF, BEAM, BALL]
x = tk.IntVar()  # Define x as integer object
for i in range(len(options)):
    button = tk.Radiobutton(settings, text=options[i], variable=x, value=i, padx=2, font=('Courier', 20),
                            width=4, command=lambda cmd=options_cmd[i]: c.setMode(cmd))
    # Put settings as a grid
    button.pack(side=tk.LEFT)

# Create frame for header text
textFrame = tk.Frame(parameterFrame)
textFrame.pack(side=tk.TOP)

# Header text
innerpar_label = tk.Label(textFrame, text='Inner Parameters', padx=8, pady=5, font=('Courier', 10, 'bold'))
innerpar_label.grid(row=1, column=0, sticky='e')
outerpar_label = tk.Label(textFrame, text='Outer Parameters', padx=8, pady=5, font=('Courier', 10, 'bold'))
outerpar_label.grid(row=1, column=1, sticky='e')

# Create frame for parameter buttons
params = tk.Frame(parameterFrame)
params.pack(side=tk.TOP, expand=True)


# Help-function to ensure int/float inputs only to text-box
def on_validate(P):
    if P == "":
        return True
    try:
        float(P)
        return True
    except ValueError:
        root.bell()  # Ring a bell when invalid input is detected
        return False


# Parameter buttons
param1 = ['K', 'Ti', 'Tr', 'Beta', 'h']
start_inner = [str(inner_K), str(inner_Ti), str(inner_Tr), str(inner_beta), str(inner_H)]
button_inner_values = []
for i in range(len(param1)):
    label = tk.Label(params, text=param1[i], font=('Courier', 10), width=5, height=1)
    label.grid(row=i, column=0, sticky="nsew")
    button = tk.Entry(params, validate="key", validatecommand=(params.register(on_validate), "%P"),
                      font=('Courier', 30), width=3)
    button.insert(tk.END, start_inner[i])
    button.grid(row=i, column=1, sticky="nsew")
    button_inner_values.append(button)

param2 = ['K', 'Ti', 'Td', 'Tr', 'N', 'Beta', 'h']
start_outer = [str(outer_K), str(outer_Ti), str(outer_Td), str(outer_Tr), str(outer_N), str(outer_beta), str(outer_H)]
button_outer_values = []
for i in range(len(param2)):
    label = tk.Label(params, text=param2[i], font=('Courier', 10), width=5, height=1)
    label.grid(row=i, column=2, sticky="nsew")
    button = tk.Entry(params, validate="key", validatecommand=(params.register(on_validate), "%P"),
                      font=('Courier', 30), width=3)
    button.insert(tk.END, start_outer[i])
    button.grid(row=i, column=3, sticky="nsew")
    button_outer_values.append(button)


# Apply buttons
def assignInner():
    global inner_K, inner_Ti, inner_Tr, inner_beta, inner_H, inner_integratorOn
    inner_K = float(button_inner_values[0].get())
    inner_Ti = float(button_inner_values[1].get())
    inner_Tr = float(button_inner_values[2].get())
    inner_beta = float(button_inner_values[3].get())
    inner_H = float(button_inner_values[4].get())
    inner_integratorOn = inner_Ti != 0
    c.setInnerParameters(inner_K, inner_Ti, inner_Tr, inner_beta, inner_H, inner_integratorOn)


def assignOuter():
    global outer_K, outer_Ti, outer_Td, outer_Tr, outer_N, outer_beta, outer_H, outer_integratorOn
    outer_K = float(button_outer_values[0].get())
    outer_Ti = float(button_outer_values[1].get())
    outer_Td = float(button_outer_values[2].get())
    outer_Tr = float(button_outer_values[3].get())
    outer_N = float(button_outer_values[4].get())
    outer_beta = float(button_outer_values[5].get())
    outer_H = float(button_outer_values[6].get())

    outer_integratorOn = outer_Ti != 0
    c.setOuterParameters(outer_K, outer_Ti, outer_Td, outer_Tr, outer_N, outer_beta, outer_H, outer_integratorOn)


apply1 = tk.Button(params, text='Apply', font=('Courier', 15), width=5, height=1,
                   command=assignOuter)
apply1.grid(row=7, column=3, sticky="nsew")
apply2 = tk.Button(params, text='Apply', font=('Courier', 15), width=5, height=1,
                   command=assignInner)
apply2.grid(row=5, column=1, sticky="nsew")


def assign_manualAmp(value):
    global manualAmp
    manualAmp = np.double(value)


# Create a vertical slider
sliderFrame = tk.Frame(referenceFrame)
sliderFrame.pack(side=tk.LEFT, expand=True)
sliderVar = tk.DoubleVar(value=0)  # We can now call sliderVar.get() to retrieve this variable
slider = tk.Scale(sliderFrame, from_=10, to=-10, length=500, width=25, orient=tk.VERTICAL, variable=sliderVar,
                  command=assign_manualAmp)
slider.config(tickinterval=5, showvalue=False, resolution=1)
slider.pack(side=tk.LEFT, expand=True)


def assign_values():
    global squareAmp, squarePeriodTime, iteration, max_ctrl
    Amp = entry_widgets[0].get()
    PeriodTime = entry_widgets[1].get()
    MaxCtrl = entry_widgets[2].get()
    # Check if the text is not empty
    iteration = 0
    if Amp:
        try:
            squareAmp = float(Amp)
        except ValueError:
            print('Error: Wrong entry')
            return

    if PeriodTime:
        try:
            squarePeriodTime = float(PeriodTime)
        except ValueError:
            print('Error: Wrong entry')
            return
    if MaxCtrl:
        try:
            max_ctrl = float(MaxCtrl)
        except ValueError:
            print('Error: Wrong entry')
            return


references = tk.Frame(referenceFrame)
references.pack(side=tk.TOP, expand=True)

param3 = ['Amp', 'Period', 'Max ctrl']
entry_widgets = []
start_values = [str(squareAmp), str(squarePeriodTime), str(max_ctrl)]

for i in range(len(param3)):
    label = tk.Label(references, text=param3[i], font=('Courier', 10), width=10)
    label.grid(row=i, column=0, sticky="nsew")
    button = tk.Entry(references, validate="key", validatecommand=(references.register(on_validate), "%P"),
                      font=('Courier', 30), width=3)
    button.grid(row=i, column=1, sticky="nsew")
    button.insert(tk.END, start_values[i])  # Insert start value
    entry_widgets.append(button)

apply3 = tk.Button(references, text='Apply', font=('Courier', 15), width=5, height=2, command=assign_values)
apply3.grid(row=3, column=1, sticky="nsew")

radiobuttons = tk.Frame(referenceFrame)
radiobuttons.pack(side=tk.LEFT)


def set_option():
    global is_manual, is_square, is_time_optimal

    # Set boolean variables based on the selected option
    if y.get() == 0:
        is_manual = True
        is_square = False
        is_time_optimal = False
    elif y.get() == 1:
        is_manual = False
        is_square = True
        is_time_optimal = False
    elif y.get() == 2:
        is_manual = False
        is_square = False
        is_time_optimal = True


# Radio-buttons
options = ['Manual', 'Square', 'Time-Optimal']
y = tk.IntVar()  # Define as integer object
for i in range(len(options)):
    button = tk.Radiobutton(radiobuttons, text=options[i], variable=y, value=i, padx=2, font=('Courier', 15), width=20,
                            height=2, command=set_option)
    # Put settings as a grid
    button.grid(row=i, column=0)  # Adjust column value and alignment

# Create plots
fig, (axRef, axCon) = plt.subplots(2)

axRef.set_title('Reference and Output signal')
# axRef.set_xlabel('Time')
# axRef.set_ylabel('Position')
axRef.set_xlim(0, tracking_time)
axRef.set_ylim(-10, 10)
linesOut, = axRef.plot([], [], color='red', linewidth=2)
linesRef, = axRef.plot([], [], color='black', linewidth=2)

axCon.set_title('Control signal')
# axCon.set_xlabel('Time')
# axCon.set_ylabel('Position')
axCon.set_xlim(0, tracking_time)
axCon.set_ylim(-10, 10)
linesCon, = axCon.plot([], [], color='blue', linewidth=2)

# Create canvas
canvas = FigureCanvasTkAgg(fig, master=plotFrame)
canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)


def update(frame):
    global dataOut, dataCon, dataRef, xRef, index, tracking_time, fps, iteration, squareAmp, is_manual, is_square, is_time_optimal, i, referenceSignal, oldreferenceSignal, max_ctrl, new_period, ts
    xRef.append(index)  # This counts the iterations for the x-axis

    # Append data to dataOut
    measurementData = c.getMeasurementData()[2]
    dataOut = np.append(dataOut, measurementData)
    controlData = c.getControlData()[1]
    dataCon = np.append(dataCon, controlData)
    # Append data to dataRef
    if is_square:
        if iteration > squarePeriodTime * fps:
            squareAmp *= -1
            iteration = 0
        dataRef = np.append(dataRef, squareAmp)

        oldreferenceSignal = referenceSignal
        referenceSignal = squareAmp
    elif is_manual:
        dataRef = np.append(dataRef, manualAmp)
        oldreferenceSignal = referenceSignal
        referenceSignal = manualAmp
    elif is_time_optimal:
        # Make the amp shift just like in squareAmp
        if iteration > squarePeriodTime * fps:
            squareAmp *= -1
            iteration = 0
            new_period = True
        # Calculate time optimal control parameters
        if new_period:
            ts = index  # start time (index = current time)
            new_period = False
        zf = squareAmp  # final setpoint. Our final "goal"
        distance = zf - referenceSignal  # difference between the final setpoint (zf) and the current reference position (z0)
        u0 = np.sign(distance) * max_ctrl  # initial control signal used in the time-optimal control calculation.
        T = np.cbrt(np.abs(distance) / (
                    2.0 * K_PHI * K_V * max_ctrl))  # How long it will take for the system to move from the current position z0 to the final setpoint zf

        # Calculate reference signal based on the time-optimal control
        t = (index - ts)  # Current time - Start time
        if t <= T:
            uff = u0
            phiff = -K_PHI * u0 * t
            ref = referenceSignal + K_PHI * K_V * u0 * t ** 3 / 6
        elif t <= 3.0 * T:
            uff = -u0
            phiff = K_PHI * u0 * (t - 2 * T)
            ref = referenceSignal - K_PHI * K_V * u0 * (t ** 3 / 6 - T * t ** 2 + T ** 2 * t - T ** 3 / 3)
        elif t <= 4.0 * T:
            uff = u0
            phiff = -K_PHI * u0 * (t - 4 * T)
            ref = referenceSignal + K_PHI * K_V * u0 * (t ** 3 / 6 - 2 * T * t ** 2 + 8 * T ** 2 * t - 26 * T ** 3 / 3)
        else:
            uff = 0
            phiff = 0
            ref = zf

        # Here we can check if the value has been updated before calling this function
        c.setUff(uff)
        c.setPhiff(phiff)

        # Append the reference signal to dataRef
        dataRef = np.append(dataRef, ref)

        # Update reference signal if changed
        oldreferenceSignal = referenceSignal
        referenceSignal = ref

    # Update x-axis limits dynamically
    axRef.set_xlim(max(0.0, index - tracking_time), max(tracking_time, index))
    axCon.set_xlim(max(0.0, index - tracking_time), max(tracking_time, index))

    linesOut.set_data(xRef, dataOut)
    linesRef.set_data(xRef, dataRef)
    linesCon.set_data(xRef, dataCon)

    index += 1 / fps
    iteration += 1
    i += 1

    if referenceSignal != oldreferenceSignal:
        c.setRef(referenceSignal)

    return linesOut,  # Return the new changes. This is to just plot the new data using "blit"


animation = FuncAnimation(fig, update, interval=1000 / fps, cache_frame_data=False, blit=False)

root.mainloop()
