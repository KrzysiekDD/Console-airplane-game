// Damian Kakol 184890 & Krzysztof Dymanowski 184836 IDE Microsoft Visual Studio/JetBrains CLion
#include <iostream>
#include <iomanip>
#include <time.h>
#include <vector>
#include <algorithm>
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
	int Wspy;
};
/*
Zamienic Samolot na "Wspolrzedne Nieba" AktualneNiebo na "Niebo" PlaneCount na "Obecne Samoloty"
TickNieba % PrzepustowoscNieba == True, to wtedy generuje sie samolot, jakies odstepy czasowe, ale tak zeby samolot
zawsze generowal sie gdy tick = 0, Wyswietlanie nastepuje przez podawanie do funkcji gotoxy(Samolot[i].Wspx, Samolot[i].WspY),
gdzie i to numer samolotu, rowniez musi byc sprawdzane czy nie ma kolizji miedzy samolotami/nie bedzie kolizji miedzy samolotami
Zakladajac ze jeden samolot zajmuje okolo 5x9 macierz kratek do kolizji, Przepustowosc powinna byc czyms w stylu 45-(jakas liczba ktora rosnie w zaleznosci
od tego jak duza jest plansza)

do wpisywania komend
string menu = "B/4";
for (int i = 0; i < 3; i++)
	cout << menu[i];
*/
class WiezaKontrolna {
public:
	WiezaKontrolna(int, int);
	void WypiszWieze(int RozmiarSzerokosc, int RozmiarWysokosc);
	void StworzNiebo();
	void WypiszNiebo();
	int GetWiersz();
	int GetKolumna();
	void RozszerzanieNieba();
	void SetWiersz(int);
	void SetKolumna(int);
	void PlaneCounter();
	void SetTick(unsigned long int);
	void ObliczeniePrzepustowosci();
private:
	static unsigned long int TickNieba;
	int PrzepustowoscNieba;
	int RozmiarSzerokosc;
	int RozmiarWysokosc;
	vector<vector< Samolot > > AktualneNiebo;
	vector<int> PlaneCount;
};

WiezaKontrolna::WiezaKontrolna(int a, int b)
{
	SetKolumna(a);
	SetWiersz(b);
	WypiszWieze(a, b);
	PAUSE
		//	StworzNiebo();
		//	WypiszNiebo();
		CLS
		RozszerzanieNieba();
	PAUSE
}

void WiezaKontrolna::ObliczeniePrzepustowosci()
{
	PrzepustowoscNieba = 2; // Do rozkminienia
}

void WiezaKontrolna::SetTick(unsigned long int tick)
{
	TickNieba = 0;
}

void WiezaKontrolna::RozszerzanieNieba()
{
	int i = 0, j = 0,w = 0, k = 0;
	vector <Samolot>::iterator itKolumn;
	vector<vector<Samolot> >::iterator itWierszy;
	for (i = 0; i < RozmiarWysokosc; i++) {
		vector <Samolot> V1;
		for (j = 0; j < RozmiarSzerokosc; j++) {
			Samolot Probny;
			Probny.WspX = i;
			Probny.Wspy = j;
			V1.push_back(Probny);
		}
		AktualneNiebo.push_back(V1);
	}

	for (itWierszy = AktualneNiebo.begin(), w = 0; itWierszy != AktualneNiebo.end(); itWierszy++, w++) {
		for (itKolumn = itWierszy->begin(), k = 0; itKolumn != itWierszy->end(); itKolumn++, k++)
			cout << AktualneNiebo[w][k].WspX << "   " << AktualneNiebo[w][k].Wspy << "   ";
		cout << "\n";
	}
}

int WiezaKontrolna::GetWiersz()
{
	return RozmiarWysokosc;
}

int WiezaKontrolna::GetKolumna()
{
	return RozmiarSzerokosc;
}

void WiezaKontrolna::SetWiersz(int wysokosc)
{
	RozmiarWysokosc = wysokosc;
}

void WiezaKontrolna::SetKolumna(int szerokosc)
{
	RozmiarSzerokosc = szerokosc;
}

void WiezaKontrolna::PlaneCounter()
{

}

void WiezaKontrolna::WypiszWieze(int RozmiarSzerokosc, int RozmiarWysokosc)
{
	//cout << RozmiarSzerokosc << RozmiarWysokosc << "\n";
	for (int i = 0; i < RozmiarSzerokosc; i++) {
		for (int j = 0; j < RozmiarWysokosc; j++) {
			if (i == 0 || i == RozmiarSzerokosc - 1)
				cout << "=";
			else if (j == 1 || j == RozmiarWysokosc - 2)
				cout << "|";
			else cout << " ";
		}
		cout << "\n";
	}
}

//void WiezaKontrolna::StworzNiebo()
//{
//	int num=0;
//	Samolot Samolocik;
//	cout << endl;
//	for (int i = 0; i < RozmiarSzerokosc; i++) {
//		vector < Samolot > v1;
//		for (int j = 0; j < RozmiarWysokosc; j++) {
//			Samolocik.Var = num;
//			v1.push_back(Samolocik);
//		}
//		AktualneNiebo.push_back(v1);
//	}
//}

void WiezaKontrolna::WypiszNiebo()
{
	for (unsigned int i = 0; i < AktualneNiebo.size(); i++) {
		for (unsigned int j = 0; j < AktualneNiebo[i].size(); j++)
			cout << setw(5) << AktualneNiebo[i][j].WspX;
		cout << "\n";
	}
}

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void MenuGlowne()
{
	srand(unsigned int(time(NULL)));
	cout << "prosze podac wysokosc nieba, enter, a nastepnie szerokosc nieba\n";
	int a, b;
	do {
		cin >> a;
	} while (a < 7);
	do {
		cin >> b;
	} while (b < 7);

	WiezaKontrolna Boeing(a, b);
	PAUSE
		CLS
		int Wiersz = rand() % Boeing.GetKolumna() + 1;
	for (int i = 0; i <= Boeing.GetWiersz(); i++) {
		CLS
			Boeing.WypiszWieze(Boeing.GetKolumna(), Boeing.GetWiersz());
		gotoxy(i, Wiersz);
		if (i == 0) continue;
		cout << "=(B9)";
		gotoxy(Boeing.GetWiersz() + 1, Boeing.GetKolumna() + 1);
		cout << "\n";
		PAUSE
	}
}



int main()
{
	MenuGlowne();
	return 0;

}