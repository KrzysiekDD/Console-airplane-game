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
	int WspX;
	int WspY;
	int Kierunek;
	char Nazwa;
	int ObecnyWzlot;
	char KierunekLotuY;
};

void gotoxy(int x, int y);
void OprawaGraficzna(int n);
void OmowienieDzialania();
/*Glowna Klasa programu*/
class WiezaKontrolna {
public:
	WiezaKontrolna();
	void Main();
	void WypiszWieze();
	void WypiszSamoloty();
	void InitSamoloty();
	void DodajSamoloty();
	void WykonanieTury();
	bool CzyNazwaDostepna(char);
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
WiezaKontrolna::WiezaKontrolna()
{
	SetRozmiary();
	ObliczeniePrzepustowosci();
	InitSamoloty();
	WypiszWieze();
	do {
		Main();
	} while (!CzyKolizja());
	CLS
		cout << "Niestety samoloty " << Zderzenie[0] << " oraz " << Zderzenie[1] << " znalazly sie zbyt blisko siebie!\n"
		<< "Sprobuj ponownie.";
	PAUSE
}
/*Glowna petla programu, kiedy z niej wyjdziemy konczymy aktualne wywolanie obiektu*/
void WiezaKontrolna::Main()
{
	DodajSamoloty();
	WypiszWieze();
	UsunSamoloty();
	WypiszSamoloty();
	Komunikat();
	string komenda, Polecenie;
	char Spacja;
	do {
 		Spacja = _getch();
		if (Spacja == ' ') {
			break;
		}
		else {
			char Nazwa = toupper(Spacja);
			cout << endl << Nazwa;
			getline(cin, komenda);
			Polecenie = Nazwa+komenda;
			PolecenieDlaSamolotu(Polecenie);
			WypiszSamoloty();
			Komunikat();
		}
	} while (Spacja != ' ');
	WykonanieTury();
}
/*Inicjalizuje dwa pierwsze samoloty wiezy kontrolnej, po jednym na bok ekranu*/
void WiezaKontrolna::InitSamoloty()
{
	Samolot StartowyL;
	StartowyL.Kierunek = 1;
	StartowyL.Nazwa = 'A';
	StartowyL.ObecnyWzlot = 0;
	StartowyL.WspX = 1;
	StartowyL.WspY = rand() % (RozmiarWysokosc - 2) + 1;
	StartowyL.KierunekLotuY = '=';
	Samolot StartowyP;
	StartowyP.Kierunek = 2;
	StartowyP.Nazwa = 'B';
	StartowyP.ObecnyWzlot = 0;
	StartowyP.WspX = RozmiarSzerokosc;
	StartowyP.WspY = rand() % (RozmiarWysokosc - 2) + 1;
	StartowyP.KierunekLotuY = '=';
	ObecneSamoloty.push_back(StartowyL);
	ObecneSamoloty.push_back(StartowyP);
}
/*W "losowych" odstepach czasu dodaje samoloty do naszego nieba*/
void WiezaKontrolna::DodajSamoloty()
{
	char Name;
	Samolot _Var;
	int _var = rand() % 6;
	if ((ObecneSamoloty.size() <= (unsigned)PrzepustowoscNieba) && (_var == 0)) {
		do {
			_Var.KierunekLotuY = '=';
			_Var.ObecnyWzlot = 0;
			_Var.Kierunek = rand() % 2 + 1;
			do {
				Name = char(rand() % 26 + 65);
				_Var.Nazwa = Name;
			} while (!CzyNazwaDostepna(Name));
			if (_Var.Kierunek == 1)
				_Var.WspX = 1;
			else if (_Var.Kierunek == 2)
				_Var.WspX = RozmiarSzerokosc;
			_Var.WspY = rand() % (RozmiarWysokosc - 2) + 1;
		} while (CzyKolizja(_Var));
		ObecneSamoloty.push_back(_Var);
	}
}
/*Usuwa samoloty ktore juz dolecialy bezpiecznie do brzegu ekranu */
void WiezaKontrolna::UsunSamoloty()
{
	for (it = ObecneSamoloty.begin(); it != ObecneSamoloty.end(); ) {
		if ((it->Kierunek == 1) && (it->WspX == (RozmiarSzerokosc)) || (it->Kierunek == 2) && (it->WspX == 2)) {
			it = ObecneSamoloty.erase(it);
		}
		else {
			++it;
		}
	}
}
/*Funkcja ta sprawdza Czy w danej turze ktorykolwiek z samolotow lezy w elipsie o polosi x = 5 i polosi y = 3 innego samolotu, jezeli tak to nastepuje kolizja*/
bool WiezaKontrolna::CzyKolizja()
{
	for (unsigned int i = 0; i< ObecneSamoloty.size();++i)
		for (unsigned int j = 0; j < ObecneSamoloty.size(); ++j) {
			if (i == j)
				continue;
		
			int Wspx1 = ObecneSamoloty.at(i).WspX;
			int Wspx2 = ObecneSamoloty.at(j).WspX;
			int Wspy1 = ObecneSamoloty.at(i).WspY;
			int Wspy2 = ObecneSamoloty.at(j).WspY;

			float Odleglosc = (((static_cast<float>(Wspx2) - static_cast<float>(Wspx1))
				* (static_cast<float>(Wspx2) - static_cast<float>(Wspx1))) / 25)
				+ ((static_cast<float>(Wspy2) - static_cast<float>(Wspy1))
					* (static_cast<float>(Wspy2) - static_cast<float>(Wspy1)) / 9);
			if (Odleglosc <= 1 && Odleglosc > 0) {
				Zderzenie[0] = ObecneSamoloty.at(i).Nazwa;
				Zderzenie[1] = ObecneSamoloty.at(j).Nazwa;
				return true; // jest kolizja
			}
		}
	return false;// nie ma kolizji
}
/*Analogiczna funkcja, ale tym razem badajaca czy nowo generowany samolot nie uderzy w zaden obecnie lecacy*/
bool WiezaKontrolna::CzyKolizja(Samolot samolot)
{
	for (unsigned int i = 0; i < ObecneSamoloty.size(); ++i){


	int Wspx1 = samolot.WspX;
	int Wspx2 = ObecneSamoloty.at(i).WspX;
	int Wspy1 = samolot.WspY;
	int Wspy2 = ObecneSamoloty.at(i).WspY;

	float Odleglosc = (((static_cast<float>(Wspx2) - static_cast<float>(Wspx1))
		* (static_cast<float>(Wspx2) - static_cast<float>(Wspx1))) / 25)
		+ ((static_cast<float>(Wspy2) - static_cast<float>(Wspy1))
		* (static_cast<float>(Wspy2) - static_cast<float>(Wspy1))/9);
	if (Odleglosc <= 1 && Odleglosc > 0)
		return true;// jest kolizja
}
	return false;// nie ma kolizji
}
/*Funkcja zapewnia ze kazdy samolot bedzie mial unikalna nazwe*/
bool WiezaKontrolna::CzyNazwaDostepna(char nazwa)
{
	for (it = ObecneSamoloty.begin(); it != ObecneSamoloty.end(); ++it)
	{
		if (it->Nazwa != nazwa)
			continue;
		else 
			return false;
	}
	return true;
}
/*Funkcja przyjmujaca komendy i zmieniajaca je w polecenia dla samolotow*/
void WiezaKontrolna::PolecenieDlaSamolotu(string komenda)
{
	vector<Samolot>::iterator itp;
	for(itp = ObecneSamoloty.begin(); itp->Nazwa != komenda[0]; itp++)
	{ }
	if (komenda[1] == 'c')
	{
		itp->KierunekLotuY = '=';
		itp->ObecnyWzlot = 0;
	}
	else {
		if (komenda[1] == '\\') {
			if (itp->Kierunek == 1)
				itp->KierunekLotuY = '\\';
			else if (itp->Kierunek == 2)
				itp->KierunekLotuY = '\\';
		}
		else if (komenda[1] == '\/') {
			if (itp->Kierunek == 1)
				itp->KierunekLotuY = '\/';
			else if (itp->Kierunek == 2)
				itp->KierunekLotuY = '\/';
		}
		itp->ObecnyWzlot = itp->ObecnyWzlot +  static_cast<int>(komenda[2]-48);
	}
}
/*Wykonanie pojedynczej tury, przesuniecie samolotu w lewo/w prawo(zawsze) oraz ewentualnie w gore lub dol*/
void WiezaKontrolna::WykonanieTury()
{
	for (it = ObecneSamoloty.begin(); it != ObecneSamoloty.end(); ++it) {
		if (it->Kierunek == 1) {
			it->WspX++;
			if (it->KierunekLotuY == '\\') {
				it->WspY++;
				it->ObecnyWzlot--;
			}
			else if (it->KierunekLotuY == '\/'){
				it->WspY--;
			it->ObecnyWzlot--;
		}
		}
		else if (it->Kierunek == 2) {
			it->WspX--;
			if (it->KierunekLotuY == '\\'){
				it->WspY--;
			it->ObecnyWzlot--;
			}
			else if (it->KierunekLotuY == '\/'){
				it->WspY++;
				it->ObecnyWzlot--;
			}
		}
		if (it->ObecnyWzlot == 0)
			it->KierunekLotuY = '=';
	}
}
/*Funkcja wypisuje aktualne polozenie samolotow w konsoli*/
void WiezaKontrolna::WypiszSamoloty()
{
	for (it = ObecneSamoloty.begin(); it != ObecneSamoloty.end(); ++it) {
		Samolot _Var = *it;
		if ((_Var.WspX == 1) || (_Var.WspX) == RozmiarSzerokosc) {
			if (_Var.Kierunek == 1) {
				gotoxy(2, _Var.WspY + 1);
				cout << _Var.Nazwa;
			}
			else if (_Var.Kierunek == 2) {
				gotoxy(RozmiarSzerokosc + 3, _Var.WspY + 1);
				cout << _Var.Nazwa;
			}
		}
		else if (_Var.Kierunek == 1) {
			gotoxy(_Var.WspX - 1, _Var.WspY + 1);
			cout << _Var.KierunekLotuY << "(" << _Var.Nazwa << _Var.ObecnyWzlot << ")";
		}
		else if(_Var.Kierunek == 2 ){
			gotoxy(_Var.WspX, _Var.WspY + 1);
			cout << "(" << _Var.Nazwa << _Var.ObecnyWzlot << ")" << _Var.KierunekLotuY;
		}
	}
}
/*Skromna funkcja w duzym przyblizeniu obliczajaca optymalna maksymalna ilosc samolotow*/
void WiezaKontrolna::ObliczeniePrzepustowosci()
{
	PrzepustowoscNieba = (RozmiarSzerokosc*RozmiarWysokosc)/150 +2;
}
/*Funkcja wypisujaca "Niebo"*/
void WiezaKontrolna::WypiszWieze()
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
void WiezaKontrolna::SetRozmiary()
{
	srand(int(time(nullptr)));
	OprawaGraficzna(1);
	cout << "Prosze podac wysokosc nieba, enter, a nastepnie szerokosc nieba:\n";
	OprawaGraficzna(2);
	OprawaGraficzna(3);
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
void WiezaKontrolna::Komunikat()
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
			OprawaGraficzna(1);
		cout << "Mini Symulacja Wiezy Kontroli lotow w konsoli by Damian Kakol and Krzysztof Dymanowski\n";
		cout << "                 Prosze podac numer opcji i wcisnac enter: \n"
			<< "   1. Prezentacja Symulacji\n"
			<< "   2. Krotkie omowienie dzialania programu\n"
			<< "   3. Wyjscie z programu.\n";
		OprawaGraficzna(2);
		OprawaGraficzna(3);
		cin >> wybor;
		switch (wybor)
		{
		case 1: {CLS
			WiezaKontrolna Boeing;}
			  break;
		case 2: OmowienieDzialania();
			break;
		case 3: exit(0);
			break;
		}
	} while (wybor != 3);
}
/*Krotka instrukcja i omowienie dzialania programu*/
void OmowienieDzialania()
{
	CLS
		OprawaGraficzna(1);
	cout << " Symulator Wiezy Kontroli lotow jest prostym przykladem tego jak programowanie\n"
		<< " obiektowe moze znaczaco ulatwic zycie programiscie. Dzieki dzialaniu jednej klasy\n"
		<< " nie trzeba dzielic naszego podprogramu na poszczegolne funkcje, a jedynie na metody\n"
		<< " ktore operuja na skladowych klasy typu \"private\".\n"
	    << " Spacja - nastepna tura,\n<znak samolotu> / <liczba 1-9> - nakaz wzniesienia sie o podana liczbe pol,\n"
		<< " <znak samolotu> \\ <liczba 1-9> - nakaz opadania o podana liczbe pol,\n"
		<< " <znak samolotu> c - anulowanie rozkazu.\n";
	OprawaGraficzna(2);
	OprawaGraficzna(3);
	PAUSE
}
/*Minimum mimorum oprawy graficznej dla projektu*/
void OprawaGraficzna(int n)
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