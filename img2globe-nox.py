#!/usr/bin/python

import sys
import numpy as np
import Image

rmat = [[1.,0,0],[0,0,-1.],[0,1.,0]]

def gen_points(w,h,tex_map):
    points = []
    
    for x in range(w):
        percx = ((x+(1./w)) / w)
        lat = 2 * np.pi * percx - np.pi

        for y in range(h):
            percy = ((y+(1./h)) / h)
            lon = np.pi * percy 

            p = [7*np.cos(lat)*np.sin(lon), 
                 -7*np.cos(lon),
                 -7* np.sin(lat)*np.sin(lon)]

            c = tex_map.getpixel((percx*tex_map.size[0],tex_map.size[1]-percy*tex_map.size[1]))
            for co in c: p.append(co)
            points.append(p)

    return points


if len(sys.argv) == 0:
    print "usage: ./img2globe [input bitmap] [output pointcloud]"

if len(sys.argv) >= 1:
    print "  using",sys.argv[1],"as tex map."
    
    tmap = Image.open(sys.argv[1])
    tmap.transpose(Image.FLIP_LEFT_RIGHT)
    tmap.transpose(Image.FLIP_TOP_BOTTOM)

if len(sys.argv) >= 2:
    print "  opening", sys.argv[2], "as pointcloud output file."
    ascFile = open(sys.argv[2],"w+")
    
    print "    width: ",tmap.size[0]
    print "    height:",tmap.size[1]
    
    pointCloud = gen_points(144,72,tmap)    

    #print pointCloud

    for po in pointCloud:
        for p in po:
            ascFile.write(str(p)) # x
            ascFile.write('\t')
        ascFile.write('\n')

    ascFile.close()
