wifi.setmode(wifi.STATION)
wifi.sta.config("ukyedu", "")

dofile("apa102.lua")

apa102.init()