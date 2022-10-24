#include <iostream>
#include <conio.h>
#include <windows.h>
//coordinates class
class coordinates {
private:
	int x;
	int y;
public:
	void setX(int value) {
		x = value;
	}
	void setY(int value) {
		y = value;
	}
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
};

//movment class
class movment {
private:
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
string username;
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
coordinates headCoordinate, fruitCoordinate;
//list size max must be (width/2)*height
coordinates tailCoordinates[400];
//store wall locations
coordinates wallCoordinates[30];
//difficulty
int difficulty=0;

void setup() {
	//interaction with user asks name and difficulty
	cout << "Enter your name: ";
	cin >> username;
	cout << "Choose difficulty:" << endl;
	cout << "     1 - EASY" << endl;
	cout << "     2 - NORMAL" << endl;
	cout << "     3 - HARD" << endl;
	cout << "Enter difficulty number: ";
	cin >> difficulty;
	cin.clear();//to clear input if user enter string or char value
	cin.ignore(100, '\n');
	cout << endl;//if user enter string or value that is smaller than 1 or bigger than 3
	if (difficulty < 1 || difficulty > 3) {
		while (difficulty < 1 || difficulty > 3) {//enter in while loop until correct input
			cout << "Please enter valid difficulty number (1-3): ";//if user enter 1.9 or 1.4 it will go to closest lowest value in this case 1
			cin >> difficulty;
			cin.clear();//to clear input if user enter string or char value
			cin.ignore(100, '\n');
			cout << endl;
		}
	}


	gameInProgress = true;
	score = 0;
	headCoordinate.setX(width / 2);
	headCoordinate.setY(height / 2);
	//create wall cordinates
	coordinates wallCoordinate;
	//check if wall isn't inside of other walls or the snake's head
	for (int i = 0; i < difficulty * 10; i++) {
		wallCoordinate.setX(2 * (rand() % width / 2));
		wallCoordinate.setY(rand() % height);
		for (int j = 0; j < difficulty * 10; j++) {
			if (wallCoordinates[j].getX() == wallCoordinate.getX() && wallCoordinates[j].getY() == wallCoordinate.getY()) {
				wallCoordinate.setX(2 * (rand() % width / 2));
				wallCoordinate.setY(rand() % height);
				j = 0;
			}
			else if (headCoordinate.getX() == wallCoordinate.getX() && headCoordinate.getY() == wallCoordinate.getY()) {
				wallCoordinate.setX(2 * (rand() % width / 2));
				wallCoordinate.setY(rand() % height);
				j = 0;
			}

		}
		wallCoordinates[i] = wallCoordinate;
	}
	//create first fruit spawn coordinates
	fruitCoordinate.setX(2 * (rand() % width / 2));
	fruitCoordinate.setY(rand() % height);
	//check if first fruit spawns in wall or in the snake's head
	bool isWall = true;
	bool isHead = true;
	int i;
	while(!isWall && !isHead ){
		for (i = 0; i < difficulty * 10; i++) {
			if (wallCoordinates[i].getX() == fruitCoordinate.getX() && wallCoordinates[i].getY() == fruitCoordinate.getY()) {
				fruitCoordinate.setX(2 * (rand() % width / 2));
				fruitCoordinate.setY(rand() % height);
				isWall = true;
				isHead = true;
				i = 0;
			}else if(i == difficulty * 10) isWall = false;
		}
		if (headCoordinate.getX() == fruitCoordinate.getX() && headCoordinate.getY() == fruitCoordinate.getY()) {
			fruitCoordinate.setX(2 * (rand() % width / 2));
			fruitCoordinate.setY(rand() % height);
			isHead = true;
			i = 0;
		}else isHead = false;
	}
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
			if (i == headCoordinate.getY() && j == headCoordinate.getX())
				cout << "O";//draw snake's head
			else if (i == fruitCoordinate.getY() && j == fruitCoordinate.getX())
				cout << "*";//draw fruit
			else{
				bool print = false;
				for (int l = 0; l < tailSize; l++){
					if (tailCoordinates[l].getX() == j && tailCoordinates[l].getY() == i){
						cout << "o";//draw snake's tail
						print = true;
					}
				}
				for (int k = 0; k < difficulty * 10; k++) {
					if (wallCoordinates[k].getX() == j && wallCoordinates[k].getY() == i) {
						cout << "#";//draw walls
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
	cout << username <<"'s Score:" << score << endl;
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

	tailCoordinates[0] = headCoordinate;

	//update tail location after each gamplay(execution)
	for (int i = 1; i < tailSize; i++){
		secondLast = tailCoordinates[i];
		tailCoordinates[i] = last;
		last = secondLast;
	}

	//moving the snake one board unit
	if(headMovment.isGoingLeft())
		headCoordinate.setX(headCoordinate.getX() - 2);
	else if(headMovment.isGoingRight())
		headCoordinate.setX(headCoordinate.getX() + 2);
	else if(headMovment.isGoingUp())
		headCoordinate.setY(headCoordinate.getY() - 1);
	else if(headMovment.isGoingDown())
		headCoordinate.setY(headCoordinate.getY() + 1);

	//move snake to opposite side if goes past the border
	if (headCoordinate.getX() >= width) 
		headCoordinate.setX(0); 
	else if (headCoordinate.getX() < 0)
		headCoordinate.setX(width);

	if (headCoordinate.getY() >= height)
		headCoordinate.setY(0);
	else if (headCoordinate.getY() < 0)
		headCoordinate.setY(height);

	//check if snake's head has hit the tail
	for (int i = 0; i < tailSize; i++) {
		if (tailCoordinates[i].getX() == headCoordinate.getX() && tailCoordinates[i].getY() == headCoordinate.getY())
			gameInProgress = false;
	}
	//check if snake's head has hit the wall
	for (int i = 0; i < difficulty * 10; i++) {
		if (wallCoordinates[i].getX() == headCoordinate.getX() && wallCoordinates[i].getY() == headCoordinate.getY())
			gameInProgress = false;
	}

	//if catch fruit create new fruit, add points, make tail longer
	if (headCoordinate.getX() == fruitCoordinate.getX() && headCoordinate.getY() == fruitCoordinate.getY()){
		score += difficulty;
		tailSize++;
		fruitCoordinate.setX(2 * (rand() % width / 2));
		fruitCoordinate.setY(rand() % height);
		//check if fruit not inside of tail and wall
		bool isWall = true;
		bool isTail = true;
		while(!isTail&&!isWall){}
			for (int i = 0; i < tailSize; i++) {
				if (tailCoordinates[i].getX() == fruitCoordinate.getX() && tailCoordinates[i].getY() == fruitCoordinate.getY()) {
					fruitCoordinate.setX(2 * (rand() % width / 2));
					fruitCoordinate.setY(rand() % height);
					isTail = true;
					i = 0;
				}else if(i==tailSize) isTail = false;
			}

			for (int i = 0; i < difficulty * 10; i++) {
				if (wallCoordinates[i].getX() == fruitCoordinate.getX() && wallCoordinates[i].getY() == fruitCoordinate.getY()) {
					fruitCoordinate.setX(2 * (rand() % width / 2));
					fruitCoordinate.setY(rand() % height);
					isWall = true;
					i = 0;
				}else if (i == difficulty*10) isTail = false;
			}
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