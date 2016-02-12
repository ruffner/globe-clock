from Tkinter import *
from PIL import Image, ImageFont, ImageDraw
import tkMessageBox, tkFileDialog
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import sys
import numpy as np
import vtk
#import img2globe
import os


root=Tk()
root.title("OrbIT- Globe Clock")
root.geometry("500x200")

def callBack():
	input=entry1.get()
	print(input)
	font = ImageFont.truetype("FreeMono.ttf",20)
	img=Image.new("RGBA", (144,72),(255,255,255))
	draw = ImageDraw.Draw(img)
	draw.text((5, 26),input,"red",font=font)
	draw = ImageDraw.Draw(img)
	img.save("textdisplay.png")
	img.show()
	os.system("python img2globe.py textdisplay.png")
	tkMessageBox.showinfo("Globe Clock", "The text is submitted")

def fileBrowser():
	img=Image.open(tkFileDialog.askopenfile(parent=root,mode='rb',title='Choose a file'))
	img.show()



app=Frame(root)
app.grid()
label1 = Label(app, text="Text to Display on Globe Clock")
label1.grid(row=0, column=0)
entry1=Entry(app)
entry1.grid(row=1, column=0)
button1=Button(app, text="Submit", command=callBack)
button1.grid(row=2, column=0)

labelSpace=Label(app, text="              ")
labelSpace.grid(row=0, column=1)

label2 = Label(app, text="Choose Image to Display on Globe Clock", justify=RIGHT)
label2.grid(row=0,column=2)
button2 = Button(app, text="Browse", command=fileBrowser)
button2.grid(row=1, column=2)

root.mainloop()