#!/usr/bin/python

import sys
import numpy as np
import Image

nx = 20
ny = 20

rmat = [[1.,0,0],[0,0,-1.],[0,1.,0]]

tmap = 0

if len(sys.argv) > 1:
    print "  using",sys.argv[1]," as tex map"
    
    tmap = Image.open(sys.argv[1])
    print "    width: ",tmap.size[0]
    print "    height:",tmap.size[1]

for x in range(nx):
    percx = ((x+(1./nx)) / nx)
    lat = 2 * np.pi * percx - np.pi

    for y in range(ny):
        percy = ((y+(1./ny)) / ny)
        lon = np.pi * percy 

        p = [np.cos(lat)*np.sin(lon), 
             -1*np.cos(lon),
             -1* np.sin(lat)*np.sin(lon)]

        new_p = []
        for row in rmat:
            new_p.append(np.dot(row, p))

        print new_p[0], new_p[1], new_p[2], tmap.getpixel((percx*tmap.size[0],percy*tmap.size[1]))

        #print p[0],"\t",p[1],"\t",p[2]
