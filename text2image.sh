#!/bin/bash

convert -background black -fill white -font fonts/FreeMono.ttf -size 144x72 -type truecolor -gravity center label:"$1"  out.bmp
