/*
This code simulates the Conway's Game of Life. 
*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <wtypes.h>
#include <iostream>
#include <cstdlib>

using namespace cv;
using namespace std;

class core{
	public:
	Mat *image;
	int **a;
	int **b;
	int ncells;
	int ncellsize;
}core;

void render(class core data);
void update_world(class core data);
void get_no_of_cells(int *pcells, int *pcellsize);
void GetDesktopResolution(int *horizontal, int *vertical);	//borrowed from: https://cppkid.wordpress.com/2009/01/07/how-to-get-the-screen-resolution-in-pixels/
void CallBackFunc(int event, int x, int y, int flags, void* userdata); //borrowed from: http://opencv-srf.blogspot.in/2011/11/mouse-events.html

int main(){

	int ncells, ncellsize, randOrman = -1;
	int **world, **prevworld;
	Mat renderedImage;
	int i, j, k;
	void *userdata;
	class core data;

	get_no_of_cells(&ncells, &ncellsize);

	world = (int **)malloc(sizeof(int *)*ncells);
	prevworld = (int **)malloc(sizeof(int *)*ncells);
	for (i = 0; i < ncells; i++){
		world[i] = (int *)malloc(sizeof(int)*ncells);
		prevworld[i] = (int *)malloc(sizeof(int)*ncells);

	}
	renderedImage = Mat(ncells*ncellsize, ncells*ncellsize, CV_8UC3);
	data.image = &renderedImage;
	data.a = world;
	data.b = prevworld;
	data.ncells = ncells;
	data.ncellsize = ncellsize;
	namedWindow("Conway's Game Of Life", WINDOW_AUTOSIZE);
	imshow("Conway's Game Of Life", renderedImage);

	for (i = 0; i < ncells; i++){
		for (j = 0; j < ncells; j++){
			world[i][j] = 0;
		}
	}
	while (randOrman != 0 && randOrman != 1){
		cout << "Enter 0 for random seeding and 1 for manual seeding: ";
		cin >> randOrman;
	}
	if (randOrman == 1){
		cout << "Left-Click on the cells which you want to make alive. \n";
		cout << "Right-Click on cells which you want to make dead.\n";
		cout <<	"After you are done Press Enter.\n";
		//set the callback function for any mouse event
		userdata = (void *)(&data);
		setMouseCallback("Conway's Game Of Life", CallBackFunc, userdata);
		while (1){
			imshow("Conway's Game Of Life", renderedImage);
			char c = waitKey(100);
			if (c == 13){
				setMouseCallback("Conway's Game Of Life", NULL, NULL);
				cout << "Enjoy :)";
				break;
			}
		}
	}
	else{
		for (i = ncells / 2 - ncells / 5; i < ncells / 2 + ncells / 5; i++){
			for (j = ncells / 2 - ncells / 5; j < ncells / 2 + ncells / 5; j++){
				if ((rand() % 8) - 6 > 0)
					world[i][j] = 1;
			}
		}

		cout << "Initialized randomly. Enjoy :) \n";
	}
	render(data);
	imshow("Conway's Game Of Life", renderedImage);
	while (1){
		imshow("Conway's Game Of Life", renderedImage);
		update_world(data);
		render(data);
		char c = waitKey(100);
		if (c == 27){
			break;
		}
	}
	return 0;
}

void get_no_of_cells(int *pcells, int *pcellsize){
	int nhorizontal, nvertical, nmaxsize, ncellsize;

	GetDesktopResolution(&nhorizontal, &nvertical);
	nmaxsize = int(0.9*min(nhorizontal, nvertical));
	cout << "Screen Size: " << nhorizontal << " " << nvertical << endl;
	cout << "Max. cells possible: " << nmaxsize << "x" << nmaxsize << endl;
	cout << "There will be nXn no. of cells. Enter n: ";
	cin >> *pcells;
	*pcells = min(max(*pcells, 3), nmaxsize);		//fix the range of screen-size
	*pcellsize = nmaxsize / *pcells;
	cout << "Cells per width: " << *pcells << endl;
	return;
}

void GetDesktopResolution(int *phorizontal, int *pvertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	*phorizontal = desktop.right;
	*pvertical = desktop.bottom;
	cout << *phorizontal << " " << *pvertical << endl;

}

void update_world(class core data){
	int i, j;
	int count;
	int ncells = data.ncells;
	for (i = 0; i < ncells; i++){
		for (j = 0; j < ncells; j++){
			data.b[i][j] = data.a[i][j];
		}
	}
	for (i = 0; i < data.ncells; i++){
		for (j = 0; j < data.ncells; j++){
			count = 0;
			count = count + (data.b[(i + ncells - 1) % ncells][(ncells + j - 1) % ncells] == 1);
			count = count + (data.b[i][(ncells + j - 1) % ncells] == 1);
			count = count + (data.b[(i + 1) % ncells][(ncells + j - 1) % ncells] == 1);
			count = count + (data.b[(i + 1) % ncells][j] == 1);
			count = count + (data.b[(i + 1) % ncells][(j + 1) % ncells] == 1);
			count = count + (data.b[i][(j + 1) % ncells] == 1);
			count = count + (data.b[(i + ncells - 1) % ncells][(j + 1) % ncells] == 1);
			count = count + (data.b[(ncells + i - 1) % ncells][j] == 1);
			
/*			Rules:
			1.	Any live cell with fewer than two live neighbours dies, as if caused by under - population.
			2.	Any live cell with two or three live neighbours lives on to the next generation.
			3.	Any live cell with more than three live neighbours dies, as if by over - population.
			4.	Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
*/
			if (data.b[i][j] == 1){
				if (count < 2 || count > 3){
					data.a[i][j] = 0;
				}
			}
			else if(data.b[i][j]== 0){
				if (count == 3){
					data.a[i][j] = 1;
				}
			}
		}
	}
	return;
}

void render(class core data){
	int i, j;
	for (i = 0; i < data.ncells; i++){
		for (j = 0; j < data.ncells; j++){
			Rect rec = { i*data.ncellsize, j*data.ncellsize, data.ncellsize, data.ncellsize };
			if (data.a[i][j] == 1){
				rectangle(*(data.image), rec, Scalar{ 0, 0, 255 }, CV_FILLED);
			}
			else{
				rectangle(*(data.image), rec, Scalar{ 0, 0, 0 }, CV_FILLED);
			}
		}
	}

	return;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata){
	int i, j;
	class core data = *((class core *)userdata);
	if (event == EVENT_LBUTTONDOWN){
		i = x / data.ncellsize;
		j = y / data.ncellsize;
		data.a[i][j] = 1;
		Rect rec = { i*data.ncellsize, j*data.ncellsize, data.ncellsize, data.ncellsize };
		rectangle(*(data.image), rec, Scalar{ 0, 0, 255 }, CV_FILLED);
	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		i = x / data.ncellsize;
		j = y / data.ncellsize;
		data.a[i][j] = 0;
		Rect rec = { i*data.ncellsize, j*data.ncellsize, data.ncellsize, data.ncellsize };
		rectangle(*(data.image), rec, Scalar{ 0, 0, 0 }, CV_FILLED);
	}
	return;
}