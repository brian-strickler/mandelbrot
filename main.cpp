/*
Function Title: Mandelbrot

Summary: Generates a Mandelbrot bitmap image

Inputs: none
Outputs: "mandelbrot_x.bmp" file where x is number betweeen 1 and 3,
			corresponding to the color scheme.

Compile instructions: g++ main.cpp or compile in Visual Studio

Pseudocode

Begin
	Create bmp and dib headers using arrays
	Generate random number to determine color scheme
	Create file based on color scheme
	Insert bmp and dib headers into file
	Calculate whether or not values at a point converge/ diverge
		and color the image accordingly by writing each pixel, one by one
	Close image
	Prompt user to exit program
End
*/
#include <iostream>
#include <fstream>
#include <array>
#include <random>
#include <chrono>
#include <string>

using namespace std;

int main()
{
	// bmp header
	std::array<unsigned char, 14> bmp_header = {
		0x42, 0x4D,
		0x76, 0xF3, 0x89, 0x06,
		0x00, 0x00,
		0x00, 0x00,
		0x36, 0x00, 0x00, 0x00
	};

	// dib header
	std::array<unsigned char, 40> dib_header = {
		0x28, 0x00, 0x00, 0x00,
		0x40, 0x1F, 0x00, 0x00,
		0xDB, 0x11, 0x00, 0x00,
		0x01, 0x00,
		0x18, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x40, 0xF3, 0x89, 0x06,
		0x13, 0x0B, 0x00, 0x00,
		0x13, 0x0B, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	};
	
	// generates random number to determine color scheme
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	uniform_int_distribution<int> distribution(1, 3);
	int color_scheme = 3;//distribution(generator);

	// determines file name based on color scheme
	string file_name = "mandelbrot_";
	file_name += to_string(color_scheme);
	file_name += ".bmp";

	cout << "Color Scheme: " << color_scheme << endl;

	ofstream image;
	image.open(file_name, ios::out | ios::binary); // opens file to write to in binary
	if (!image.is_open()) // checks to see if file opened correctly
	{
		cout << "Failed to create and open file"; // displays if it doesn't open correctly
		return 20;
	}

	cout << "File created :)" << endl;

	// inserts the bmp and dib headers into the file
	for (auto a = bmp_header.begin(); a != bmp_header.end(); ++a)
		image << *a;
	for (auto b = dib_header.begin(); b != dib_header.end(); ++b)
		image << *b;
	
	cout << "Generating " << file_name << " now..." << endl;
	double x0;
	double y0;
	double x;
	double y;
	double xtemp;
	int iteration;
	int max_iteration;
	int i;
	int j;

	char loading = 178; // sets the ASCII character for the loading bar

	for (j = 0; j < 4571; j++)
	{
		for (i = 0; i < 8000; i++)
		{
			x0 = (((i * 3.5) / 7999.0) - 2.5);
			y0 = (((j * 2.0) / 4570.0) - 1.0);
			x = 0.0;
			y = 0.0;
			iteration = 0;
			max_iteration = 1000;
			// algorithm to determine whether a particular point is a part of the mandelbrot set
			while ((x*x + y*y < 2.0*2.0) && (iteration < max_iteration))
			{
				xtemp = ((x*x - y*y) + x0);
				y = ((2*x*y) + y0);
				x = xtemp;
				iteration = iteration + 1;
			}

			// places color for pixel in the .bmp file
			if (color_scheme == 1)
			{
				if (iteration == max_iteration)
					image << (unsigned char)0x00 << (unsigned char)0x00 << (unsigned char)0x00;
				else
					image << (unsigned char)( i + j / (j+1)) << (unsigned char)(i + j + (i / 3) * j / (i+1)) << (unsigned char)(0x50);
			}
			if (color_scheme == 2)
			{
				if (iteration == max_iteration)
					image << (unsigned char)0x00 << (unsigned char)0x00 << (unsigned char)0x00;
				else
					image << (unsigned char)(j * iteration * i / 5255 - 5) << (unsigned char)(i * i * iteration / 31250 - 52) << (unsigned char)(i * iteration / 51250);
			}
			if (color_scheme == 3)
			{
				if (iteration == max_iteration)
					image << (unsigned char)0x00 << (unsigned char)0x00 << (unsigned char)0x00;
				else image << (unsigned char)((255 - j * iteration / 17855) * j - 4*i) << (unsigned char)(255 - i * j * iteration / 312500) << (unsigned char)(255 - i * iteration / 31250);
			//	else
			//		image << (unsigned char)(iteration / 4) << (unsigned char)(0x34) << (unsigned char)(iteration / 6);

			}	

			if (j % 135 == 1 && i == 1) // displays loading bar when run in Visual Studio
				cout << (char)loading;
		}
		
	}
	
	image.close(); // closes image file

	cout << endl << endl << "Process Completed, Press Enter to Exit" << endl;
	getchar(); // waits for user the user to press return before exiting

	return 0;
}

