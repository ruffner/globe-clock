wifi.setmode(wifi.STATION)
wifi.sta.config("ukyedu", "")

dofile("apa102.lua")

dofile("ledserver.lua")

apa102.init()
