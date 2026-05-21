#1
import numpy as np
import matplotlib.pyplot as myplot

u = [1500, 2200, 3500, 2700, 3100, 7800, 9300]

# Plotting the data
myplot.plot(u)

myplot.show()

#################################################

#2
import wx

def bnclick(evt):
    print(txt.GetValue())

theApp = wx.App()
f = wx.Frame(parent=None, title="My GUI App")
bn = wx.Button(parent=f, label="Click Me", size=(50, 25))
bn.SetPosition(wx.Point(50, 45))

txt = wx.TextCtrl(parent=f, size=(75, 25))
txt.SetPosition(wx.Point(50, 15))

# Bind the button click event to bnclick function
bn.Bind(wx.EVT_BUTTON, bnclick)

f.Show()
theApp.MainLoop()

#################################################

#3
def reverse_file(input_filename, output_filename):
    with open(input_filename, "rb") as infile open(output_filename, "wb") as outfile:
        infile.seek(0, 2)  # Move to the end of the file

        file_size = infile.tell()
        
        # Read the file byte by byte in reverse order
        for position in range(file_size - 1, -1, -1):
            infile.seek(position)
            byte = infile.read(1)
            outfile.write(byte)

reverse_file("TESTREVERSE.txt", "OUTPUT.txt")
print("done")

# Example usage:
# reverse_file("input_file.bin", "output_file.bin")
