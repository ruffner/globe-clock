/* Matt Ruffner
 * March 2 2016 
 * C++ implementation of img2globe 
 * 
 * Thanks to http://www.partow.net/programming/bitmap/index.html
 * for the C++ Bitmap Library
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>

#include "bitmap_image.hpp"

#define PI 3.1415926535897

#define ASC 7
#define GI  8

using namespace std;

void usage(char *sig);

void outputASC(bitmap_image image, ostream *out );
void outputGI(bitmap_image image, ostream *out );

void printAsHex(unsigned char *data, size_t len);

int width = 0, height = 0;
int globeWidth = 144, globeHeight = 70;

int main(int argc, char * argv[])
{
	if(argc < 3) {
		usage(argv[0]);
	}
	
	int task = 0; 
	if( argv[2][0] == 'a' ) {
		task = ASC;
	} else if( argv[2][0] == 'g' ) {
		task = GI;
	} else {
		usage(argv[0]);
	}
	
	bool toFile = false;
	ofstream *fout;
	
	if (argc == 4) {
		toFile = true;
		fout = new ofstream(argv[3]);
		
		if(!fout->is_open()) {
			cout << "[ERROR] - Could not open " << string(argv[3]) << " for writing." << endl;
			exit(1);
		}
	} else if(argc > 4) {
		usage(argv[0]);
	} 
	
	
	string file_name(argv[1]);

	bitmap_image image;

	if( file_name.compare("-") == 0 )
		image = bitmap_image(cin);
	else
		image = bitmap_image(file_name);
		

	if (!image)	{
		cout << "[ERROR] - Failed to open " << file_name << endl;
		exit(1);
	}

	width = image.width();
	height = image.height();



	// TODO: support input image downscaling 
	if (width != globeWidth || height != globeHeight) {
		cout << "Sorry, input image must be " << globeWidth << "x" << globeHeight << "." << endl;
		exit(1);
	}


	switch(task) {
		case ASC:
			if( toFile ) {
				cout << "- Generating point cloud for visualization..." << endl;
				outputASC(image, fout);
				fout->close();
			} else {
				outputASC(image, &cout);
			}
			break;
		case GI: 
			if( toFile) { 
				cout << "- Generating file for GlobeClock" << endl;
				cout << "-- filesize: " << width*height*3 << " bytes" << endl;
				outputGI(image, fout);
				fout->close();
			} else {
				outputGI(image, &cout);
			}
			break;
		default: 
			exit(1);
	}
	

	return 0;
}

void outputASC(bitmap_image image, ostream *out )
{	
	unsigned char r, g, b;
	
	for (long x = 0; x < width; x++) {
		
		double dx = ((x+((double)1/width)) / width);
        double lat = 2 * PI * dx - PI;
		
		for (long y = 0; y < height; y++) {
			
			double dy = ((y+((double)1/height)) / height);
            double lon = PI * dy;
            double xc,yc,zc;
			
			image.get_pixel( (int)(dx*width), height - (int)(dy*height), r, g, b );
						
			xc = 7*cos(lat)*sin(lon);
            yc = -7*cos(lon);
            zc = -7* sin(lat)*sin(lon);
			
			(*out) << xc << "\t" 
				<< yc << "\t" 
				<< zc << "\t" 
				<< (int)r << "\t"
				<< (int)g << "\t"
				<< (int)b << "\n";
			
		}
	}
	
}

void outputGI(bitmap_image image, ostream *out )
{
	unsigned char r, g, b;
	
	for (long x = 0; x < width; x++) {
		
		double dx = ((x+((double)1/width)) / width);
		
		for (long y = 0; y < height; y++) {
			
			double dy = ((y+((double)1/height)) / height);
			
			image.get_pixel( (int)(dx*width), height - (int)(dy*height), r, g, b );
			
			(*out) << r << g << b;
		}
	}
}

void usage(char *exe)
{
	cout << "Globe Convert" << endl;
	cout << "  Utility for converting 24 bit (no alpha) bitmaps for GlobeClocks." << endl << endl;
	cout << "  Usage:" << endl;
	cout << "  " << string(exe) << " <input.bmp> [a|g] [outputfile]" << endl;
	cout << "    out.asc is the structured light point cloud for visualization in the OrbIT Portal and OrbIT Dashboard" << endl;
	cout << "            thanks to the XB-PointStream library." << endl;
	cout << "    out.gi is the globe image of arranged RGB values from the input bitmap." << endl;
	cout << "    - can be specified for the image input in which case it will be read from stdin." << endl << endl;
	exit(1);
}
