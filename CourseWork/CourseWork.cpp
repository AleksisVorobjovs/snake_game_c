#include <iostream>
#include <conio.h>
#include <windows.h>
//coordinates class
class coordinates {
	int x;
	int y;
public:
	void setX(int value) {
		x = value;
	}
	int getX() {
		return x;
	}
	void setY(int value) {
		y = value;
	}
	int getY() {
		return y;
	}
};

//movment class
class movment {
	bool up;
	bool down;
	bool right;
	bool left;
public:
	void reset() {
		up = false;
		down = false;
		right = false;
		left = false;
	}
	void goUp() {
		reset();
		up = true;
	}
	void goDown() {
		reset();
		down = true;
	}
	void goRight() {
		reset();
		right = true;
	}
	void goLeft() {
		reset();
		left = true;
	}
	bool isGoingUp() {
		return up;
	}
	bool isGoingDown() {
		return down;
	}
	bool isGoingRight() {
		return right;
	}
	bool isGoingLeft() {
		return left;
	}
};

using namespace std;
//tacking game status
bool gameInProgress;
//saving size of the playing, must be true -> width/height = 2 or at least width%2=0
const int width = 40;
const int height = 20;
//track score
int score, bestScore;
//track tail size
int tailSize;
//track snakes movment direction
movment headMovment;
//tracks all game object cordinates
coordinates headCoordinates, fruitCoordinates;
//list size max must be (width/2)*height
coordinates tailCoordinates[400];

void setup(){
	gameInProgress = true;
	score = 0;
	headCoordinates.setX(width / 2);
	headCoordinates.setY(height / 2);
	fruitCoordinates.setX(2 * (rand() % width / 2));
	fruitCoordinates.setY(rand() % height);
}
void display(){
	system("cls");//clear console
	for (int i = 0; i < width + 2; i++)
		cout << "-";//draw upper border
	cout << endl;

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (j == 0)
				cout << "|";//draw left border
			if (i == headCoordinates.getY() && j == headCoordinates.getX())
				cout << "O";//draw snake's head
			else if (i == fruitCoordinates.getY() && j == fruitCoordinates.getX())
				cout << "*";//draw fruit
			else{
				bool print = false;
				for (int k = 0; k < tailSize; k++){
					if (tailCoordinates[k].getX() == j && tailCoordinates[k].getY() == i){
						cout << "o";//draw snake's tail
						print = true;
					}
				}
				if (!print) {
					cout << " ";//draw empty space
				}
			}
			if (j == width - 1)
				cout << "|";//draw right border
		}
		cout << endl;
	}

	for (int i = 0; i < width + 2; i++) {
		cout << "-";//draw lower border
	}
	cout << endl;
	cout << "Score:" << score << endl;
}

//read user input
void userInput(){
	if (_kbhit()){
		switch (tolower(_getch())) {
		case 'a':
			if (!headMovment.isGoingRight() || tailSize==0)
				headMovment.goLeft();
			break;
		case 'd':
			if (!headMovment.isGoingLeft() || tailSize == 0)
				headMovment.goRight();
			break;
		case 'w':
			if (!headMovment.isGoingDown() || tailSize == 0)
				headMovment.goUp();
			break;
		case 's':
			if (!headMovment.isGoingUp() || tailSize == 0)
				headMovment.goDown();
			break;
		case ' ':
			system("pause");
			break;
		}
	}
}

void gameplay(){
	coordinates last = tailCoordinates[0];
	coordinates	secondLast;

	tailCoordinates[0] = headCoordinates;

	//update tail location after each gamplay(execution)
	for (int i = 1; i < tailSize; i++){
		secondLast = tailCoordinates[i];
		tailCoordinates[i] = last;
		last = secondLast;
	}

	//moving the snake one board unit
	if(headMovment.isGoingLeft())
		headCoordinates.setX(headCoordinates.getX() - 2);
	else if(headMovment.isGoingRight())
		headCoordinates.setX(headCoordinates.getX() + 2);
	else if(headMovment.isGoingUp())
		headCoordinates.setY(headCoordinates.getY() - 1);
	else if(headMovment.isGoingDown())
		headCoordinates.setY(headCoordinates.getY() + 1);

	//move snake to opposite side if goes past the border
	if (headCoordinates.getX() >= width) 
		headCoordinates.setX(0); 
	else if (headCoordinates.getX() < 0)
		headCoordinates.setX(width);

	if (headCoordinates.getY() >= height)
		headCoordinates.setY(0);
	else if (headCoordinates.getY() < 0)
		headCoordinates.setY(height);

	//check if head has hit the tail
	for (int i = 0; i < tailSize; i++) {
		if (tailCoordinates[i].getX() == headCoordinates.getX() && tailCoordinates[i].getY() == headCoordinates.getY())
			gameInProgress = false;
	}
	//if catch fruit create new fruit, add points, make tail longer
	if (headCoordinates.getX() == fruitCoordinates.getX() && headCoordinates.getY() == fruitCoordinates.getY()){
		score += 1;
		tailSize++;
		fruitCoordinates.setX(2 * (rand() % width / 2));
		fruitCoordinates.setY(rand() % height);
	}
}

int main(){
	srand((unsigned int)time(NULL));//need this or else fruit location isn't true random 
	setup();
	while (gameInProgress){
		display();
		userInput();
		gameplay();
		Sleep(5);
	}
	return 0;
}
