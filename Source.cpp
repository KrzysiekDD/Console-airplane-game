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
	void InitSamoloty();
	void DodajSamoloty();
	void WykonanieTury();
	bool CzynameDostepna(char);
	bool CzyKolizja();
	bool CzyKolizja(Samolot);
	void UsunSamoloty();
	void PolecenieDlaSamolotu(string);
	void SetRozmiary();
	void ObliczeniePrzepustowosci();
	void Komunikat();
private:
	int PrzepustowoscNieba;
	int RozmiarSzerokosc;
	int RozmiarWysokosc;
	char Zderzenie[2];
	vector<Samolot> ObecneSamoloty;
	vector<Samolot>::iterator it;
	vector<vector<int>> Kolizja;
	vector<vector<Samolot>> Niebo;
};
/*Konstruktor domyslny, wywoluje obiekt i to w nim dzieje sie caly program*/
Control_Tower::Control_Tower()
{
	SetRozmiary();
	ObliczeniePrzepustowosci();
	InitSamoloty();
	draw_environment();
	do {
		Main();
	} while (!CzyKolizja());
	CLS
		cout << "Niestety samoloty " << Zderzenie[0] << " oraz " << Zderzenie[1] << " znalazly sie zbyt blisko siebie!\n"
		<< "Sprobuj ponownie.";
	PAUSE
}
/*Glowna petla programu, kiedy z niej wyjdziemy konczymy aktualne wywolanie obiektu*/
void Control_Tower::Main()
{
	DodajSamoloty();
	draw_environment();
	UsunSamoloty();
	draw_airplanes();
	Komunikat();
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
			PolecenieDlaSamolotu(Polecenie);
			draw_airplanes();
			Komunikat();
		}
	} while (Spacja != ' ');
	WykonanieTury();
}
/*Inicjalizuje dwa pierwsze samoloty wiezy kontrolnej, po jednym na bok ekranu*/
void Control_Tower::InitSamoloty()
{
	Samolot StartowyL;
	StartowyL.x_direction = 1;
	StartowyL.name = 'A';
	StartowyL.current_yaw = 0;
	StartowyL.x_position = 1;
	StartowyL.y_position = rand() % (RozmiarWysokosc - 2) + 1;
	StartowyL.y_direction = '=';
	Samolot StartowyP;
	StartowyP.x_direction = 2;
	StartowyP.name = 'B';
	StartowyP.current_yaw = 0;
	StartowyP.x_position = RozmiarSzerokosc;
	StartowyP.y_position = rand() % (RozmiarWysokosc - 2) + 1;
	StartowyP.y_direction = '=';
	ObecneSamoloty.push_back(StartowyL);
	ObecneSamoloty.push_back(StartowyP);
}
/*W "losowych" odstepach czasu dodaje samoloty do naszego nieba*/
void Control_Tower::DodajSamoloty()
{
	char Name;
	Samolot _Var;
	int _var = rand() % 6;
	if ((ObecneSamoloty.size() <= (unsigned)PrzepustowoscNieba) && (_var == 0)) {
		do {
			_Var.y_direction = '=';
			_Var.current_yaw = 0;
			_Var.x_direction = rand() % 2 + 1;
			do {
				Name = char(rand() % 26 + 65);
				_Var.name = Name;
			} while (!CzynameDostepna(Name));
			if (_Var.x_direction == 1)
				_Var.x_position = 1;
			else if (_Var.x_direction == 2)
				_Var.x_position = RozmiarSzerokosc;
			_Var.y_position = rand() % (RozmiarWysokosc - 2) + 1;
		} while (CzyKolizja(_Var));
		ObecneSamoloty.push_back(_Var);
	}
}
/*Usuwa samoloty ktore juz dolecialy bezpiecznie do brzegu ekranu */
void Control_Tower::UsunSamoloty()
{
	for (it = ObecneSamoloty.begin(); it != ObecneSamoloty.end(); ) {
		if ((it->x_direction == 1) && (it->x_position == (RozmiarSzerokosc)) || (it->x_direction == 2) && (it->x_position == 2)) {
			it = ObecneSamoloty.erase(it);
		}
		else {
			++it;
		}
	}
}
/*Funkcja ta sprawdza Czy w danej turze ktorykolwiek z samolotow lezy w elipsie o polosi x = 5 i polosi y = 3 innego samolotu, jezeli tak to nastepuje kolizja*/
bool Control_Tower::CzyKolizja()
{
	for (unsigned int i = 0; i< ObecneSamoloty.size();++i)
		for (unsigned int j = 0; j < ObecneSamoloty.size(); ++j) {
			if (i == j)
				continue;
		
			int x_position1 = ObecneSamoloty.at(i).x_position;
			int x_position2 = ObecneSamoloty.at(j).x_position;
			int y_position1 = ObecneSamoloty.at(i).y_position;
			int y_position2 = ObecneSamoloty.at(j).y_position;

			float Odleglosc = (((static_cast<float>(x_position2) - static_cast<float>(x_position1))
				* (static_cast<float>(x_position2) - static_cast<float>(x_position1))) / 25)
				+ ((static_cast<float>(y_position2) - static_cast<float>(y_position1))
					* (static_cast<float>(y_position2) - static_cast<float>(y_position1)) / 9);
			if (Odleglosc <= 1 && Odleglosc > 0) {
				Zderzenie[0] = ObecneSamoloty.at(i).name;
				Zderzenie[1] = ObecneSamoloty.at(j).name;
				return true; // jest kolizja
			}
		}
	return false;// nie ma kolizji
}
/*Analogiczna funkcja, ale tym razem badajaca czy nowo generowany samolot nie uderzy w zaden obecnie lecacy*/
bool Control_Tower::CzyKolizja(Samolot samolot)
{
	for (unsigned int i = 0; i < ObecneSamoloty.size(); ++i){


	int x_position1 = samolot.x_position;
	int x_position2 = ObecneSamoloty.at(i).x_position;
	int y_position1 = samolot.y_position;
	int y_position2 = ObecneSamoloty.at(i).y_position;

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
bool Control_Tower::CzynameDostepna(char name)
{
	for (it = ObecneSamoloty.begin(); it != ObecneSamoloty.end(); ++it)
	{
		if (it->name != name)
			continue;
		else 
			return false;
	}
	return true;
}
/*Funkcja przyjmujaca komendy i zmieniajaca je w polecenia dla samolotow*/
void Control_Tower::PolecenieDlaSamolotu(string komenda)
{
	vector<Samolot>::iterator itp;
	for(itp = ObecneSamoloty.begin(); itp->name != komenda[0]; itp++)
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
void Control_Tower::WykonanieTury()
{
	for (it = ObecneSamoloty.begin(); it != ObecneSamoloty.end(); ++it) {
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
	for (it = ObecneSamoloty.begin(); it != ObecneSamoloty.end(); ++it) {
		Samolot _Var = *it;
		if ((_Var.x_position == 1) || (_Var.x_position) == RozmiarSzerokosc) {
			if (_Var.x_direction == 1) {
				gotoxy(2, _Var.y_position + 1);
				cout << _Var.name;
			}
			else if (_Var.x_direction == 2) {
				gotoxy(RozmiarSzerokosc + 3, _Var.y_position + 1);
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
void Control_Tower::ObliczeniePrzepustowosci()
{
	PrzepustowoscNieba = (RozmiarSzerokosc*RozmiarWysokosc)/150 +2;
}
/*Funkcja wypisujaca "Niebo"*/
void Control_Tower::draw_environment()
{
	CLS
		for (int i = 0; i < RozmiarWysokosc; i++) {
			for (int j = 0; j < RozmiarSzerokosc; j++) {
				gotoxy(j + 3, i + 1);
				if (i == 0 || i == RozmiarWysokosc - 1)
					cout << "=";
				else if (j == 0 || j == RozmiarSzerokosc - 1)
					cout << "|";
			}
		}
}
/*Ustawia rozmiary naszego nieba*/
void Control_Tower::SetRozmiary()
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
	RozmiarWysokosc = a;
	do {
		cin >> b;
		if (b < 7) cout << "Prosze podac liczbe wieksza od 6!\n";
	} while (b < 7);
	RozmiarSzerokosc = b;
	cin.ignore();
}
/*Komunikat wyswietlany podczas pracy programu*/
void Control_Tower::Komunikat()
{
	gotoxy(1, RozmiarWysokosc + 1);
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