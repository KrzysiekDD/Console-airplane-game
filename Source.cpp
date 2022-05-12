#include <iostream>
#include <iomanip>
#include <time.h>
#include <vector>
#include <cstdlib>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <string>

#define PAUSE system("pause");
#define CLS system("cls");
using namespace std;
// Airplane structure used to store parameters of particular airplanes
struct Airplane {
	int x_position;
	int y_position;
	int x_direction;
	char name;
	int current_yaw;
	char y_direction;
};

void gotoxy(int x, int y);
void draw_stars(int n);
void show_instructions();
//Main class of the game
class Control_Tower {
public:
	Control_Tower();
	void Main();
	void draw_environment();
	void draw_airplanes();
	void initial_airplanes();
	void add_airplanes();
	void next_cycle();
	bool is_name_available(char);
	bool collision();
	bool collision(Airplane);
	void del_airplanes();
	void airplane_action(string);
	void set_dimensions();
	void get_airspace_capacity();
	void display_controls();
private:
	int airspace_capacity;
	int dimension_width;
	int dimension_height;
	char impact[2];
	vector<Airplane> current_airplanes;
	vector<Airplane>::iterator it;
};
//Constructor method
Control_Tower::Control_Tower()
{
	set_dimensions();
	get_airspace_capacity();
	initial_airplanes();
	draw_environment();
	do {
		Main();
	} while (!collision());
	CLS
		cout << "Unfortunately " << impact[0] << " and " << impact[1] << " flew too close to each other!\n"
		<< "Try again.";
	PAUSE
}
//Main function, called in a loop inside the main_menu function
void Control_Tower::Main()
{
	add_airplanes();
	draw_environment();
	del_airplanes();
	draw_airplanes();
	display_controls();
	string command, order;
	char next_turn;
	do {
 		next_turn = _getch();
		if (next_turn == ' ') {
			break;
		}
		else {
			char name = toupper(next_turn);
			cout << endl << name;
			getline(cin, command);
			order = name+command;
			airplane_action(order);
			draw_airplanes();
			display_controls();
		}
	} while (next_turn != ' ');
	next_cycle();
}
//Initializes first two airplanes, one coming from the left and the other from the right
void Control_Tower::initial_airplanes()
{
	Airplane StartowyL;
	StartowyL.x_direction = 1;
	StartowyL.name = 'A';
	StartowyL.current_yaw = 0;
	StartowyL.x_position = 1;
	StartowyL.y_position = rand() % (dimension_height - 2) + 1;
	StartowyL.y_direction = '=';
	Airplane StartowyP;
	StartowyP.x_direction = 2;
	StartowyP.name = 'B';
	StartowyP.current_yaw = 0;
	StartowyP.x_position = dimension_width;
	StartowyP.y_position = rand() % (dimension_height - 2) + 1;
	StartowyP.y_direction = '=';
	current_airplanes.push_back(StartowyL);
	current_airplanes.push_back(StartowyP);
}
//Adds airplanes to the airspace in random time intervals
void Control_Tower::add_airplanes()
{
	char Name;
	Airplane _Var;
	int _var = rand() % 6;
	if ((current_airplanes.size() <= (unsigned)airspace_capacity) && (_var == 0)) {
		do {
			_Var.y_direction = '=';
			_Var.current_yaw = 0;
			_Var.x_direction = rand() % 2 + 1;
			do {
				Name = char(rand() % 26 + 65);
				_Var.name = Name;
			} while (!is_name_available(Name));
			if (_Var.x_direction == 1)
				_Var.x_position = 1;
			else if (_Var.x_direction == 2)
				_Var.x_position = dimension_width;
			_Var.y_position = rand() % (dimension_height - 2) + 1;
		} while (collision(_Var));
		current_airplanes.push_back(_Var);
	}
}
//Deletes airplanes that have already crossed the entire airspace
void Control_Tower::del_airplanes()
{
	for (it = current_airplanes.begin(); it != current_airplanes.end(); ) {
		if ((it->x_direction == 1) && (it->x_position == (dimension_width)) 
			|| (it->x_direction == 2) && (it->x_position == 2)) {
			it = current_airplanes.erase(it);
		}
		else {
			++it;
		}
	}
}
/*Checks if at any given time an airplane lies within the radius of another one
the proximity are is an ellipse of width 5 and height 3*/
bool Control_Tower::collision()
{
	for (unsigned int i = 0; i< current_airplanes.size();++i)
		for (unsigned int j = 0; j < current_airplanes.size(); ++j) {
			if (i == j)
				continue;
		
			int x_position1 = current_airplanes.at(i).x_position;
			int x_position2 = current_airplanes.at(j).x_position;
			int y_position1 = current_airplanes.at(i).y_position;
			int y_position2 = current_airplanes.at(j).y_position;

			float Odleglosc = (((static_cast<float>(x_position2) - static_cast<float>(x_position1))
				* (static_cast<float>(x_position2) - static_cast<float>(x_position1))) / 25)
				+ ((static_cast<float>(y_position2) - static_cast<float>(y_position1))
					* (static_cast<float>(y_position2) - static_cast<float>(y_position1)) / 9);
			if (Odleglosc <= 1 && Odleglosc > 0) {
				impact[0] = current_airplanes.at(i).name;
				impact[1] = current_airplanes.at(j).name;
				return true; // collision occured
			}
		}
	return false;// collision did not occur
}
//Similar function, however this one checks if newly generated airplanes
// will not come into collision with existing ones
bool Control_Tower::collision(Airplane new_airplane)
{
	for (unsigned int i = 0; i < current_airplanes.size(); ++i){


	int x_position1 = new_airplane.x_position;
	int x_position2 = current_airplanes.at(i).x_position;
	int y_position1 = new_airplane.y_position;
	int y_position2 = current_airplanes.at(i).y_position;

	float Odleglosc = (((static_cast<float>(x_position2) - static_cast<float>(x_position1))
		* (static_cast<float>(x_position2) - static_cast<float>(x_position1))) / 25)
		+ ((static_cast<float>(y_position2) - static_cast<float>(y_position1))
		* (static_cast<float>(y_position2) - static_cast<float>(y_position1))/9);
	if (Odleglosc <= 1 && Odleglosc > 0)
		return true;// collision occured
}
	return false;// collision did not occur
}
//asserting each airplane will have an unique name
bool Control_Tower::is_name_available(char name)
{
	for (it = current_airplanes.begin(); it != current_airplanes.end(); ++it)
	{
		if (it->name != name)
			continue;
		else 
			return false;
	}
	return true;
}
// input commands, transform them into directions for airplanes
void Control_Tower::airplane_action(string command)
{
	vector<Airplane>::iterator itp;
	for(itp = current_airplanes.begin(); itp->name != command[0]; itp++)
	{ }
	if (command[1] == 'c')
	{
		itp->y_direction = '=';
		itp->current_yaw = 0;
	}
	else {
		if (command[1] == '\\') {
			if (itp->x_direction == 1)
				itp->y_direction = '\\';
			else if (itp->x_direction == 2)
				itp->y_direction = '\\';
		}
		else if (command[1] == '\/') {
			if (itp->x_direction == 1)
				itp->y_direction = '\/';
			else if (itp->x_direction == 2)
				itp->y_direction = '\/';
		}
		itp->current_yaw = itp->current_yaw +  static_cast<int>(command[2]-48);
	}
}
// one "turn" of the game, executes the current move for the airplane
void Control_Tower::next_cycle()
{
	for (it = current_airplanes.begin(); it != current_airplanes.end(); ++it) {
		if (it->x_direction == 1) {
			it->x_position++;
			if (it->y_direction == '\\') {
				it->y_position++;
				it->current_yaw--;
			}
			else if (it->y_direction == '\/'){
				it->y_position--;
			it->current_yaw--;
		}
		}
		else if (it->x_direction == 2) {
			it->x_position--;
			if (it->y_direction == '\\'){
				it->y_position--;
			it->current_yaw--;
			}
			else if (it->y_direction == '\/'){
				it->y_position++;
				it->current_yaw--;
			}
		}
		if (it->current_yaw == 0)
			it->y_direction = '=';
	}
}
// redraws current state of the airspace
void Control_Tower::draw_airplanes()
{
	for (it = current_airplanes.begin(); it != current_airplanes.end(); ++it) {
		Airplane _Var = *it;
		if ((_Var.x_position == 1) || (_Var.x_position) == dimension_width) {
			if (_Var.x_direction == 1) {
				gotoxy(2, _Var.y_position + 1);
				cout << _Var.name;
			}
			else if (_Var.x_direction == 2) {
				gotoxy(dimension_width + 3, _Var.y_position + 1);
				cout << _Var.name;
			}
		}
		else if (_Var.x_direction == 1) {
			gotoxy(_Var.x_position - 1, _Var.y_position + 1);
			cout << _Var.y_direction << "(" << _Var.name << _Var.current_yaw << ")";
		}
		else if(_Var.x_direction == 2 ){
			gotoxy(_Var.x_position, _Var.y_position + 1);
			cout << "(" << _Var.name << _Var.current_yaw << ")" << _Var.y_direction;
		}
	}
}
// vague approximation of how many airplanes will fit in the current airspace
void Control_Tower::get_airspace_capacity()
{
	airspace_capacity = (dimension_width*dimension_height)/150 +2;
}
// draws the environment
void Control_Tower::draw_environment()
{
	CLS
		for (int i = 0; i < dimension_height; i++) {
			for (int j = 0; j < dimension_width; j++) {
				gotoxy(j + 3, i + 1);
				if (i == 0 || i == dimension_height - 1)
					cout << "=";
				else if (j == 0 || j == dimension_width - 1)
					cout << "|";
			}
		}
}
// setter for size of the airspace
void Control_Tower::set_dimensions()
{
	srand(int(time(nullptr)));
	draw_stars(1);
	cout << "Please enter the airspace's height, then width:\n";
	draw_stars(2);
	draw_stars(3);
	int a, b;
	do {
		cin >> a;
		if (a < 7) cout << "Input a number bigger than 6!\n";
	} while (a < 7);
	dimension_height = a;
	do {
		cin >> b;
		if (b < 7) cout << "Input a number bigger than 6!\n";
	} while (b < 7);
	dimension_width = b;
	cin.ignore();
}
//displays controls for the game under the airspace
void Control_Tower::display_controls()
{
	gotoxy(1, dimension_height + 1);
	cout << " Space - next cycle,\n <airplane name> / <number from 1-9> - order the airplane to raise by a given height\n"
		<< " <airplane name> \\ <number from 1-9> - order to fall by a given height\n"
		<< " If the plane is flying from right to left, the \\ and / are inverted\n"
		<< " <airplane name> c - cancel the order.\n"
		<< " Example \"B/3\" - order the airplane B to fall 3 units in next 3 cycles.";
}
// goto function used to draw characters
void gotoxy(int x, int y)
{
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
// contains the whole game
void MainMenu()
{
	int wybor;
	do
	{
		CLS
			draw_stars(1);
		cout << "  Minigame of operating a control tower by Krzysztof Dymanowski\n"
			 << "  Please select the number corresponding to your choice: \n"
			 << "    1. Play the game\n"
			 << "    2. Instructions\n"
		  	 << "    3. Exit.\n";
		draw_stars(2);
		draw_stars(3);
		cin >> wybor;
		switch (wybor)
		{
		case 1: {CLS
			Control_Tower Boeing;}
			  break;
		case 2: show_instructions();
			break;
		case 3: exit(0);
			break;
		}
	} while (wybor != 3);
}
// short instructions describing the controls of the game
void show_instructions()
{
	CLS
		draw_stars(1);
	cout << " Space - next cycle,\n airplane name> / <number from 1-9> - order the airplane to raise by a given height\n"
		 << " <airplane name> \\ <number from 1-9> - order to fall by a given height\n"
		 << " If the plane is flying from right to left, the \\ and / are inverted\n"
		 << " <airplane name> c - cancel the order. \n";
	draw_stars(2);
	draw_stars(3);
	PAUSE
}
// used to draw simple graphic components
void draw_stars(int n)
{
	switch (n)
	{
	case 1: cout << "*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*\n";
		break;
	case 2: cout << " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
		break;
	case 3: cout << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n";
		break;
	}
}

int main()
{
	MainMenu();
	return 0;
}