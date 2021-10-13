import serial
import re
import pyqtgraph as pg
import datetime as dt
import numpy as np

# Max amount of points displaying on the chart
vals_limit = 500

# Create figure for plotting
xs = np.arange(vals_limit)
ys = np.zeros(vals_limit)
pw = pg.plot()
curve = pw.plot(xs, ys, title='EMG over time')
pw.setLabel('left', 'Power')

# Specify the port name
# You should check this and change (if need) each time you reconnect the Arduino part
port =  'COM8' # '/dev/ttyACM0'

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
def update():

    global ys, xs

    # Read EMG in each animate() call
    emg_signal = read_emg_once()
    if not emg_signal:
        return

    # Add x and y to lists
    #xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
    ys = np.append(ys, int(emg_signal))

    # Limit x and y lists to 20 items
    xs = xs[-vals_limit:]
    ys = ys[-vals_limit:]

    # Draw x and y lists
    curve.setData(xs, ys)

# Set up calling update() function periodically
# Reading emg is performed inside update()
timer = pg.QtCore.QTimer()
timer.timeout.connect(update)
timer.setInterval(1)
timer.start()

# Following lines are required for pyqtgraph 
# (besacuse it is based on Qt and we have to start QApplication to show any visual)
if __name__ == '__main__':
    import sys
    if sys.flags.interactive != 1 or not hasattr(pg.QtCore, 'PYQT_VERSION'):
        pg.QtGui.QApplication.exec_()