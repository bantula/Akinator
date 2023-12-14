#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class GuessingSystem
{
public:

	void openTerminal();

	void guess(int brojPokusaja);
	void info(const string& ime);
	void load();
	void deleteSomeone(const string& ime);
	void newPerson();

	void stampajPitanja();
	void ucitajPaProveri(const vector<string>& tokeni);
	void stampajSveOdgovore();
	void stampajImena();
	void mapiraj(const vector<string>& tokeni);
	void stampajMapu();
	void resetuj();
	void nadjiOdgovore(char brojOdgovor, int brojac);

private:
	
	vector<string> pitanja;
	vector<map<string, int>> sviOdgovori;
	vector<string> imena;
	map<string, string> mapaImena;
};

