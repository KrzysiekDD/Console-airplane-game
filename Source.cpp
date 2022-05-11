// Damian Kakol 184890 & Krzysztof Dymanowski 184836 IDE Microsoft Visual Studio/JetBrains CLion
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

struct Samolot {
	int x_position;
	int y_position;
	int x_direction;
	char name;
	int current_yaw;
	char y_direction;
};

void gotoxy(int x, int y);
void draw_stars(int n);
void show_instruction();
/*Glowna Klasa programu*/
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
	bool collision(Samolot);
	void del_airplanes();
	void airplane_action(string);
	void set_dimensions();
	void get_airspace_capacity();
	void display_message();
private:
	int airspace_capacity;
	int dimension_width;
	int dimension_height;
	char Zderzenie[2];
	vector<Samolot> current_airplanes;
	vector<Samolot>::iterator it;
	vector<vector<int>> Kolizja;
	vector<vector<Samolot>> Niebo;
};
/*Konstruktor domyslny, wywoluje obiekt i to w nim dzieje sie caly program*/
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
		cout << "Niestety samoloty " << Zderzenie[0] << " oraz " << Zderzenie[1] << " znalazly sie zbyt blisko siebie!\n"
		<< "Sprobuj ponownie.";
	PAUSE
}
/*Glowna petla programu, kiedy z niej wyjdziemy konczymy aktualne wywolanie obiektu*/
void Control_Tower::Main()
{
	add_airplanes();
	draw_environment();
	del_airplanes();
	draw_airplanes();
	display_message();
	string komenda, Polecenie;
	char Spacja;
	do {
 		Spacja = _getch();
		if (Spacja == ' ') {
			break;
		}
		else {
			char name = toupper(Spacja);
			cout << endl << name;
			getline(cin, komenda);
			Polecenie = name+komenda;
			airplane_action(Polecenie);
			draw_airplanes();
			display_message();
		}
	} while (Spacja != ' ');
	next_cycle();
}
/*Inicjalizuje dwa pierwsze samoloty wiezy kontrolnej, po jednym na bok ekranu*/
void Control_Tower::initial_airplanes()
{
	Samolot StartowyL;
	StartowyL.x_direction = 1;
	StartowyL.name = 'A';
	StartowyL.current_yaw = 0;
	StartowyL.x_position = 1;
	StartowyL.y_position = rand() % (dimension_height - 2) + 1;
	StartowyL.y_direction = '=';
	Samolot StartowyP;
	StartowyP.x_direction = 2;
	StartowyP.name = 'B';
	StartowyP.current_yaw = 0;
	StartowyP.x_position = dimension_width;
	StartowyP.y_position = rand() % (dimension_height - 2) + 1;
	StartowyP.y_direction = '=';
	current_airplanes.push_back(StartowyL);
	current_airplanes.push_back(StartowyP);
}
/*W "losowych" odstepach czasu dodaje samoloty do naszego nieba*/
void Control_Tower::add_airplanes()
{
	char Name;
	Samolot _Var;
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
/*Usuwa samoloty ktore juz dolecialy bezpiecznie do brzegu ekranu */
void Control_Tower::del_airplanes()
{
	for (it = current_airplanes.begin(); it != current_airplanes.end(); ) {
		if ((it->x_direction == 1) && (it->x_position == (dimension_width)) || (it->x_direction == 2) && (it->x_position == 2)) {
			it = current_airplanes.erase(it);
		}
		else {
			++it;
		}
	}
}
/*Funkcja ta sprawdza Czy w danej turze ktorykolwiek z samolotow lezy w elipsie o polosi x = 5 i polosi y = 3 innego samolotu, jezeli tak to nastepuje kolizja*/
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
				Zderzenie[0] = current_airplanes.at(i).name;
				Zderzenie[1] = current_airplanes.at(j).name;
				return true; // jest kolizja
			}
		}
	return false;// nie ma kolizji
}
/*Analogiczna funkcja, ale tym razem badajaca czy nowo generowany samolot nie uderzy w zaden obecnie lecacy*/
bool Control_Tower::collision(Samolot samolot)
{
	for (unsigned int i = 0; i < current_airplanes.size(); ++i){


	int x_position1 = samolot.x_position;
	int x_position2 = current_airplanes.at(i).x_position;
	int y_position1 = samolot.y_position;
	int y_position2 = current_airplanes.at(i).y_position;

	float Odleglosc = (((static_cast<float>(x_position2) - static_cast<float>(x_position1))
		* (static_cast<float>(x_position2) - static_cast<float>(x_position1))) / 25)
		+ ((static_cast<float>(y_position2) - static_cast<float>(y_position1))
		* (static_cast<float>(y_position2) - static_cast<float>(y_position1))/9);
	if (Odleglosc <= 1 && Odleglosc > 0)
		return true;// jest kolizja
}
	return false;// nie ma kolizji
}
/*Funkcja zapewnia ze kazdy samolot bedzie mial unikalna nazwe*/
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
/*Funkcja przyjmujaca komendy i zmieniajaca je w polecenia dla samolotow*/
void Control_Tower::airplane_action(string komenda)
{
	vector<Samolot>::iterator itp;
	for(itp = current_airplanes.begin(); itp->name != komenda[0]; itp++)
	{ }
	if (komenda[1] == 'c')
	{
		itp->y_direction = '=';
		itp->current_yaw = 0;
	}
	else {
		if (komenda[1] == '\\') {
			if (itp->x_direction == 1)
				itp->y_direction = '\\';
			else if (itp->x_direction == 2)
				itp->y_direction = '\\';
		}
		else if (komenda[1] == '\/') {
			if (itp->x_direction == 1)
				itp->y_direction = '\/';
			else if (itp->x_direction == 2)
				itp->y_direction = '\/';
		}
		itp->current_yaw = itp->current_yaw +  static_cast<int>(komenda[2]-48);
	}
}
/*Wykonanie pojedynczej tury, przesuniecie samolotu w lewo/w prawo(zawsze) oraz ewentualnie w gore lub dol*/
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
/*Funkcja wypisuje aktualne polozenie samolotow w konsoli*/
void Control_Tower::draw_airplanes()
{
	for (it = current_airplanes.begin(); it != current_airplanes.end(); ++it) {
		Samolot _Var = *it;
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
/*Skromna funkcja w duzym przyblizeniu obliczajaca optymalna maksymalna ilosc samolotow*/
void Control_Tower::get_airspace_capacity()
{
	airspace_capacity = (dimension_width*dimension_height)/150 +2;
}
/*Funkcja wypisujaca "Niebo"*/
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
/*Ustawia rozmiary naszego nieba*/
void Control_Tower::set_dimensions()
{
	srand(int(time(nullptr)));
	draw_stars(1);
	cout << "Prosze podac wysokosc nieba, enter, a nastepnie szerokosc nieba:\n";
	draw_stars(2);
	draw_stars(3);
	int a, b;
	do {
		cin >> a;
		if (a < 7) cout << "Prosze podac liczbe wieksza od 6!\n";
	} while (a < 7);
	dimension_height = a;
	do {
		cin >> b;
		if (b < 7) cout << "Prosze podac liczbe wieksza od 6!\n";
	} while (b < 7);
	dimension_width = b;
	cin.ignore();
}
/*display_message wyswietlany podczas pracy programu*/
void Control_Tower::display_message()
{
	gotoxy(1, dimension_height + 1);
	cout << "Spacja - nastepna tura,\n<znak samolotu> / <liczba 1-9> - nakaz wzniesienia sie o podana liczbe pol,\n"
		<< "<znak samolotu> \\ <liczba 1-9> - nakaz opadania o podana liczbe pol,\n"
		<< "<znak samolotu> c - anulowanie rozkazu.";
}
/*Niestandardowa funkcja ktora ustawia kursor na danych wspolrzednychw  konsoli*/
void gotoxy(int x, int y)
{
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
/*Tutaj zawiera sie caly program*/
void MenuGlowne()
{
	int wybor;
	do
	{
		CLS
			draw_stars(1);
		cout << "Mini Symulacja Wiezy Kontroli lotow w konsoli by Damian Kakol and Krzysztof Dymanowski\n";
		cout << "                 Prosze podac numer opcji i wcisnac enter: \n"
			<< "   1. Prezentacja Symulacji\n"
			<< "   2. Krotkie omowienie dzialania programu\n"
			<< "   3. Wyjscie z programu.\n";
		draw_stars(2);
		draw_stars(3);
		cin >> wybor;
		switch (wybor)
		{
		case 1: {CLS
			Control_Tower Boeing;}
			  break;
		case 2: show_instruction();
			break;
		case 3: exit(0);
			break;
		}
	} while (wybor != 3);
}
/*Krotka instrukcja i omowienie dzialania programu*/
void show_instruction()
{
	CLS
		draw_stars(1);
	cout << " Symulator Wiezy Kontroli lotow jest prostym przykladem tego jak programowanie\n"
		<< " obiektowe moze znaczaco ulatwic zycie programiscie. Dzieki dzialaniu jednej klasy\n"
		<< " nie trzeba dzielic naszego podprogramu na poszczegolne funkcje, a jedynie na metody\n"
		<< " ktore operuja na skladowych klasy typu \"private\".\n"
	    << " Spacja - nastepna tura,\n<znak samolotu> / <liczba 1-9> - nakaz wzniesienia sie o podana liczbe pol,\n"
		<< " <znak samolotu> \\ <liczba 1-9> - nakaz opadania o podana liczbe pol,\n"
		<< " <znak samolotu> c - anulowanie rozkazu.\n";
	draw_stars(2);
	draw_stars(3);
	PAUSE
}
/*Minimum mimorum oprawy graficznej dla projektu*/
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
	MenuGlowne();
	return 0;
}