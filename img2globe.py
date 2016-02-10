#!/usr/bin/python

import numpy as np
import Image

nx = 20
ny = 20

rmat = [[1.,0,0],[0,0,-1.],[0,1.,0]]

for x in range(nx):
    lat = 2 * np.pi * ((x+(1./nx)) / nx) - np.pi

    for y in range(ny):
        lon = np.pi * ((y+(1./ny)) / ny) 

        p = [np.cos(lat)*np.sin(lon), 
             -1*np.cos(lon),
             -1* np.sin(lat)*np.sin(lon)]

        new_p = []
        for row in rmat:
            new_p.append(np.dot(row, p))

        print new_p[0]," ",new_p[1]," ",new_p[2]

        #print p[0],"\t",p[1],"\t",p[2]
