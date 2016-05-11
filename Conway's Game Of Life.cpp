/*
This code simulates the Conway's Game of Life. Rules in priority order:

1.	There will be two kinds of things: prey(red) and predators(blue).
2.	Following rules apply on prey:
2.1.	Any live cell with fewer than two live neighbours dies, as if caused by under - population.
2.2.	Any live cell with two or three live neighbours lives on to the next generation.
2.3.	Any live cell with more than three live neighbours dies, as if by over - population.
2.4.	Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
3.  Predator feeds on prey. Every prey has a 67% chance of being eaten by either of its neighbor predators. Order is decided randomly.
4. Following rules apply on predators:
4.1.	A predator which has not eaten any prey in neighborhood dies.
4.2.	A predator with not a single predator as neighbor dies.
4.3.	An empty cell with 2 or 3 surrounding predators gives birth to a predator
5.	If an empty cell can bear both prey child and predator child, predator child gets priority.

Can be modified by including plant concept, life span concept, weak-strong concept, etc.
*/


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <wtypes.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
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
int *get_random_indices(int nCells);

int main(){

	int ncells, ncellsize, randOrman = -1;
	int **world, **prevworld;
	Mat renderedImage;
	int i, j, k;
	void *userdata;
	class core data;

	get_no_of_cells(&ncells, &ncellsize);
	srand(time(NULL));
	world = (int **)malloc(sizeof(int *)*ncells);
	prevworld = (int **)malloc(sizeof(int *)*ncells);
	for (i = 0; i < ncells; i++){
		world[i] = (int *)malloc(sizeof(int)*ncells);
		prevworld[i] = (int *)malloc(sizeof(int)*ncells);

	}
	//initialize to zero
	for (i = 0; i < ncells; i++){
		for (j = 0; j < ncells; j++){
			world[i][j] = 0;
		}
	}

	renderedImage = Mat(ncells*ncellsize, ncells*ncellsize, CV_8UC3);
	rectangle(renderedImage, Rect{ 0, 0, renderedImage.rows, renderedImage.cols }, Scalar{ 0, 255/2, 0 }, CV_FILLED);
	data.image = &renderedImage;
	data.a = world;
	data.b = prevworld;
	data.ncells = ncells;
	data.ncellsize = ncellsize;
	namedWindow("Conway's Game Of Life", WINDOW_AUTOSIZE);
	render(data);
	imshow("Conway's Game Of Life", renderedImage);
	waitKey(50);		//this is necessary otherwise imshow won't work

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
			char c = waitKey(50);
			if (c == 13){
				setMouseCallback("Conway's Game Of Life", NULL, NULL);
				cout << "Enjoy :)";
				break;
			}
		}
	}
	else{
		for (i = 0; i < ncells ; i++){
			for (j = 0; j < ncells; j++){
				int randomvariable = (rand() % 6);
				if (randomvariable > 2){
					if (randomvariable <= 4)
						world[i][j] = 1;
					else if (randomvariable >= 5)
						world[i][j] = 2;
				}
			}
		}

		cout << "Initialized randomly. Enjoy :) \n";
	}
	render(data);
	imshow("Conway's Game Of Life", renderedImage);
	waitKey(50);
	while (1){
		imshow("Conway's Game Of Life", renderedImage);
		update_world(data);
		render(data);
		char c = waitKey(500);
		if (c == 27){
			break;
		}
	}
	return 0;
}

void get_no_of_cells(int *pcells, int *pcellsize){
	int nhorizontal, nvertical, nmaxsize, ncellsize;

	GetDesktopResolution(&nhorizontal, &nvertical);
	nmaxsize = int(0.95*min(nhorizontal, nvertical));
	cout << "Screen Size: " << nhorizontal << " " << nvertical << endl;
	cout << "Max. cells possible: " << 56 << "x" << 56 << endl;
	cout << "There will be nXn no. of cells. Enter n: ";
	cin >> *pcells;
	*pcells = min(max(*pcells, 3), 56);		//fix the range of screen-size
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

/*	Update Rules in priority order:
1.	There will be two kinds of things: prey(red) and predators(blue).
2.	Following rules apply on prey:
2.1.	Any live cell with fewer than two live neighbours dies, as if caused by under - population.
2.2.	Any live cell with two or three live neighbours lives on to the next generation.
2.3.	Any live cell with more than three live neighbours dies, as if by over - population.
2.4.	Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
3.  Predator feeds on prey. Every prey has a 67% chance of being eaten by either of its neighbor predators. Order is decided randomly.
4. Following rules apply on predators:
4.1.	A predator which has not eaten any prey in neighborhood dies.
4.2.	A predator with not a single predator as neighbor dies, as if by loneliness.
4.3.	An empty cell with 2 or 3 surrounding predators gives birth to a predator.
5.	If an empty cell can bear both prey child and predator child, predator child gets priority.
*/
void update_world(class core data){
	int i, j, k,l;
	int count;
	int ncells = data.ncells;
	for (i = 0; i < ncells; i++){
		for (j = 0; j < ncells; j++){
			data.b[i][j] = data.a[i][j];
		}
	}

	//eating of prey by predator
	srand(time(NULL));
	int r = rand();
	//get random indices
	int *randomindices = get_random_indices(ncells);

	for (l = 0; l < ncells*ncells; l++){
			i = randomindices[l] / ncells;
			j = randomindices[l] % ncells;
			if (data.b[i][j] == 1){
				int predatorneighbor[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

				if (data.b[(i + ncells - 1) % ncells][(ncells + j - 1) % ncells] == 2){
					predatorneighbor[0] = 1;
				};
				if (data.b[i][(ncells + j - 1) % ncells] == 2){
					predatorneighbor[1] = 1;
				};
				if (data.b[(i + 1) % ncells][(ncells + j - 1) % ncells] == 2){
					predatorneighbor[2] = 1;
				};
				if (data.b[(i + 1) % ncells][j] == 2){
					predatorneighbor[3] = 1;
				};
				if (data.b[(i + 1) % ncells][(j + 1) % ncells] == 2){
					predatorneighbor[4] = 1;
				};
				if (data.b[i][(j + 1) % ncells] == 2){
					predatorneighbor[5] = 1;
				};
				if (data.b[(i + ncells - 1) % ncells][(j + 1) % ncells] == 2){
					predatorneighbor[6] = 1;
				};
				if (data.b[(ncells + i - 1) % ncells][j] == 2){
					predatorneighbor[7] = 1;
				};

				count = 0;
				for (k = 0; k < 8; k++){
					count = count + predatorneighbor[k];
				}
				if (count == 0){
					continue;		//i.e. nothing to eat, 
				}
				if(rand() % (3 * count) == 0){
					continue;		//i.e. none of the predators could hunt it, 
				}
				data.b[i][j] = -1;
				count = rand() % count;
				int whichneighbor = -1;
				for (k = 0; k < 8; k++){
					count = count - predatorneighbor[k];
					if (count == 0){
						whichneighbor = k;
						break;
					}
				}
				switch (whichneighbor){
					case 0:
						data.b[(i + ncells - 1) % ncells][(ncells + j - 1) % ncells] = -2;
						break;
					case 1:
						data.b[i][(ncells + j - 1) % ncells] = -2;
						break;
					case 2:
						data.b[(i + 1) % ncells][(ncells + j - 1) % ncells] = -2;
						break;
					case 3:
						data.b[(i + 1) % ncells][j] = -2;
						break;
					case 4:
						data.b[(i + 1) % ncells][(j + 1) % ncells] = -2;
						break;
					case 5:
						data.b[i][(j + 1) % ncells] = -2;
						break;
					case 6:
						data.b[(i + ncells - 1) % ncells][(j + 1) % ncells] = -2;
						break;
					case 7:
						data.b[(ncells + i - 1) % ncells][j] = -2;
						break;
				}
			}
		}

	for (i = 0; i < ncells; i++){
		for (j = 0; j < ncells; j++){
			if (data.b[i][j] == 2){
				data.b[i][j] = 0;
				data.a[i][j] = 0;
			}
		}
	}

	free(randomindices);

//gen rules for prey
	for (i = 0; i < data.ncells; i++){
		for (j = 0; j < data.ncells; j++){
			count = 0;
			count = count + (data.b[(i + ncells - 1) % ncells][(ncells + j - 1) % ncells] == 1 
				|| data.b[(i + ncells - 1) % ncells][(ncells + j - 1) % ncells] == -1);
			count = count + (data.b[i][(ncells + j - 1) % ncells] == 1 
				|| data.b[i][(ncells + j - 1) % ncells] == -1);
			count = count + (data.b[(i + 1) % ncells][(ncells + j - 1) % ncells] == 1 
				|| data.b[(i + 1) % ncells][(ncells + j - 1) % ncells] == -1);
			count = count + (data.b[(i + 1) % ncells][j] == 1 
				|| data.b[(i + 1) % ncells][j] == -1);
			count = count + (data.b[(i + 1) % ncells][(j + 1) % ncells] == 1 
				|| data.b[(i + 1) % ncells][(j + 1) % ncells] == -1);
			count = count + (data.b[i][(j + 1) % ncells] == 1 
				|| data.b[i][(j + 1) % ncells] == -1);
			count = count + (data.b[(i + ncells - 1) % ncells][(j + 1) % ncells] == 1 
				|| data.b[(i + ncells - 1) % ncells][(j + 1) % ncells] == -1);
			count = count + (data.b[(ncells + i - 1) % ncells][j] == 1 
				|| data.b[(ncells + i - 1) % ncells][j] == -1);

			if (data.b[i][j] == -1){
				data.b[i][j] = 0;
				data.a[i][j] = 0;
			}
			if (data.b[i][j] == 1){
				if (count < 2 || count > 3){
					data.a[i][j] = 0;
				}
			}
			else if (data.b[i][j] == 0){
				if (count == 3){
					data.a[i][j] = 1;
				}
			}
		}
	}

	//	gen rules for predator
	for (i = 0; i < data.ncells; i++){
		for (j = 0; j < data.ncells; j++){
			count = 0;
			count = count + (data.b[(i + ncells - 1) % ncells][(ncells + j - 1) % ncells] == -2);
			count = count + (data.b[i][(ncells + j - 1) % ncells] == -2);
			count = count + (data.b[(i + 1) % ncells][(ncells + j - 1) % ncells] == -2);
			count = count + (data.b[(i + 1) % ncells][j] == -2);
			count = count + (data.b[(i + 1) % ncells][(j + 1) % ncells] == -2);
			count = count + (data.b[i][(j + 1) % ncells] == -2);
			count = count + (data.b[(i + ncells - 1) % ncells][(j + 1) % ncells] == -2);
			count = count + (data.b[(ncells + i - 1) % ncells][j] == -2);

			if (data.b[i][j] == -2 && count > 0){
				data.a[i][j] = 2;
			}
			else if (data.b[i][j] == -2 && count == 0){
				data.a[i][j] = 0;
			}
			else if (data.b[i][j] != -2 && data.b[i][j] != 1){
				if (count == 3){
					data.a[i][j] = 2;
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
			Rect rec = { i*data.ncellsize+1, j*data.ncellsize+1, data.ncellsize-1, data.ncellsize-1 };
			if (data.a[i][j] == 1){
				rectangle(*(data.image), rec, Scalar{ 0, 0, 128 }, CV_FILLED);
			}
			else if (data.a[i][j] == 2){
				rectangle(*(data.image), rec, Scalar{ 200, 0, 0 }, CV_FILLED);
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
		Rect rec = { i*data.ncellsize+1, j*data.ncellsize+1, data.ncellsize-1, data.ncellsize-1 };
		if (data.a[i][j] == 1){
			data.a[i][j] = 2;
			rectangle(*(data.image), rec, Scalar{ 200, 0, 0 }, CV_FILLED);
		}
		else if (data.a[i][j] == 2){
			data.a[i][j] = 0;
			rectangle(*(data.image), rec, Scalar{ 0, 0, 0 }, CV_FILLED);
		}
		else if (data.a[i][j] == 0){
			data.a[i][j] = 1;
			rectangle(*(data.image), rec, Scalar{ 0, 0, 128 }, CV_FILLED);
		}

	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		i = x / data.ncellsize;
		j = y / data.ncellsize;
		data.a[i][j] = 0;
		Rect rec = { i*data.ncellsize+1, j*data.ncellsize+1, data.ncellsize-1, data.ncellsize-1 };
		rectangle(*(data.image), rec, Scalar{ 0, 0, 0 }, CV_FILLED);
	}
	return;
}

int *get_random_indices(int ncells){
	srand(time(NULL));
	int i = 0, j = 0;
	int rval;
	int *r = (int *)malloc(sizeof(int)*ncells*ncells);
	int *copy = (int *)malloc(sizeof(int)*ncells*ncells);
	for (i = 0; i < ncells*ncells; i++){
		r[i] = i;
	}
	for (i = 0; i < ncells*ncells; i++){
		rval = rand() % (ncells*ncells - i);
		copy[i] = r[rval];
		r[rval] = r[ncells*ncells - 1 - i];

	}
	return copy;
}
