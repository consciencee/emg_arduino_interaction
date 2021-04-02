import serial
import re
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import time
import datetime as dt

# Max amount of points displaying on the chart
vals_limit = 50

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
xs.extend(range(0, vals_limit))
ys = [0] * vals_limit

# Specify the port name
# You should check this and change (if need) each time you reconnect the Arduino part
port =  'COM3' # '/dev/ttyACM0'

# HC06 defaults to this value
# You should not change this constant
baudrate = 9600

# Connect to serial port
ser = serial.Serial(port, baudrate)

def read_emg_once() :
    # Read one line
    line = ser.readline()

    # Parse the line to extract the signal value
    signal_search = re.search('([0-9]+)', line.decode('utf-8'))
    if signal_search:
        signal = signal_search.group(1)
        print(signal)
        return signal
    return None


# This function is called periodically from FuncAnimation
def animate(i, xs, ys):

    # Read EMG in each animate() call
    emg_signal = read_emg_once()
    if not emg_signal:
        return

    # Add x and y to lists
    #xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
    ys.append(int(emg_signal))

    # Limit x and y lists to 20 items
    xs = xs[-vals_limit:]
    ys = ys[-vals_limit:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys)
    ax.set_ylim(bottom=0)
    ax.set_ylim(top=500)

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('EMG over time')
    plt.ylabel('Power')

# Set up plot to call animate() function periodically
# Reading emg is performed inside animate()
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=100)
plt.show()     