#include "GuessingSystem.h"


void GuessingSystem::openTerminal()
{
	cout << "DOBRODOSLI U AKINATORAAA" << endl;
	cout << "------------------------" << endl;
	cout << "Opciju: " << endl;
	cout << "Ucitaj fajl: LOAD" << endl;
	cout << "Pogodi: GUESS" << endl;
	cout << "Pogodi sa pokusajima: TRY" << endl;
	cout << "Informacije o pojmu: INFO" << endl;
	cout << "Dodaj novi pojam: NEW" << endl;
	cout << "Izbrisi nekog: DELETE" << endl;
	cout << "Izadji iz programa: EXIT" << endl;
	cout << "------------------------" << endl;


	string opcija;
	cin >> opcija;

	while (opcija != "EXIT") {
		if (opcija == "LOAD") {
			load();
		}
		else if (opcija == "GUESS") {
			if (!sviOdgovori.empty()) {
				guess(-1);
			}
			else {
				cout << "Niste ucitali fajl" << endl;	
			}
			
		}
		else if (opcija == "NEW") {
			if (!sviOdgovori.empty()) {
				newPerson();
			}
			else {
				cout << "Niste ucitali fajl" << endl;
			}

		}
		else if (opcija == "TRY") {
			if (!sviOdgovori.empty()) {
				cout << "Unesite broj pokusaja: " << endl;
				int brojPokusaja;
				cin >> brojPokusaja;
				guess(brojPokusaja);
			}
			else {
				cout << "Niste ucitali fajl" << endl;
			}
		}
		else if (opcija == "INFO") {
			if (!sviOdgovori.empty()) {
				cout << "Unesite ime: " << endl;
				stampajImena();
				string ime;
				cin >> ime;
				info(ime);
			}
			else {
				cout << "Niste ucitali fajl" << endl;
			}
		}
		else if (opcija == "DELETE") {
			if (!sviOdgovori.empty()) {
				cout << "Unesite koga zelite da izbrisete: " << endl;
				stampajImena();
				string ime;
				cin >> ime;
				deleteSomeone(ime);
				stampajSveOdgovore();
			}
			else {
				cout << "Niste ucitali fajl" << endl;
			}

		}
		else {
			cout << "Nevalidna opcija, probajte opet" << endl;
		}
		cout << "Sta zelite da uradim?" << endl;
		cin >> opcija;
	}
	cout << "Hvala sto ste koristili Akinatora" << endl;
}

void GuessingSystem::load()
{
	resetuj();

	cout << "Unesi putanju fajla: " << endl;

	string putanja;
	cin >> putanja;
	ifstream fajl(putanja);

	//Ovde dodaj izuzetkeee

	if (!fajl.is_open()) {
		cerr << "Error opening file: " << putanja << endl;
		return; // Return an error code, mozda izuzetak
	}

	bool daLiJePrvaLinija = true;

	string linija;
	while (getline(fajl, linija)) {

		// Delimo liniju u tokene i pravimo vektor tokeni gde ih smestamo

		stringstream ss(linija);
		vector<string> tokeni;
		string token;

		while (getline(ss, token, ',')) {
			tokeni.push_back(token);
		}

		// Ovime dodajemo tokene na vektor
		if (daLiJePrvaLinija == false) {
			ucitajPaProveri(tokeni);
			mapiraj(tokeni);
		}
		else {
			for (const auto& t : tokeni) { // t je iterator koji ide kroz vektor 
				pitanja.push_back(t);
			}
			daLiJePrvaLinija = false;
		}
	}
}

void GuessingSystem::deleteSomeone(const string& ime)
{
	for (int i = 0; i < imena.size(); i++) {
		if (imena[i] == ime) {
			imena.erase(imena.begin() + i);
			break;
		}
	}

	for (int i = 0; i < pitanja.size(); i++) { // Ovom petljom brisem odgovor koji je imao samo obrisan primerak
		int brojPonavljanjaOdgovora = 0;
		string odgovor;
		for (const auto& pair1 : mapaImena) {
			if (pair1.second[i] == mapaImena[ime][i]) {

				brojPonavljanjaOdgovora++;
				for (const auto& pair2 : sviOdgovori[i]) {
					if (pair2.second == (mapaImena[ime][i] - '0')) {
						odgovor = pair2.first;
					}
				}
			}
		}
		if (brojPonavljanjaOdgovora == 1) { //Poklopice se jednom sa samim sobom
			sviOdgovori[i].erase(odgovor);
		}
	}

	auto iterator = mapaImena.find(ime);
	if (iterator != mapaImena.end()) {
		mapaImena.erase(iterator);
	}
}

void GuessingSystem::newPerson()
{
	bool proveraImena = false;
	string ime;
	while (proveraImena == false) {
		cout << "Unesite ime:" << endl;

		cin >> ime;
		proveraImena = true;

		for (const auto& i : imena) {
			if (i == ime) {
				cout << "Ime vec postoji, probajte ponovo" << endl;
				proveraImena = false;
				break;
			}
		}
	}

	imena.push_back(ime);

	for (int i = 0; i < pitanja.size(); i++) {
		cout << pitanja[i] << endl;
		cout << "Unesite odgovor: " << endl;
		string odgovor;
		cin >> odgovor;

		if (sviOdgovori[i].count(odgovor) == 0) { // Ako ne postoji odgovor, ubacujemo ga 
			int maxVrednost = -1;

			for (const auto& pair : sviOdgovori[i]) { // Ovako najdemo koji je sledeci broj koji vezujemo za odgovor
				if (pair.second > maxVrednost) {
					maxVrednost = pair.second;
				}
			}
			sviOdgovori[i][odgovor] = maxVrednost + 1;

		}
		mapaImena[ime] += to_string(sviOdgovori[i][odgovor]);

	}
	int brojac = 0;
	for (const auto& pair : mapaImena) {
		if (pair.second == mapaImena[ime]) {
			brojac++;
		}
	}

	if (brojac >= 2) {
		cout << "Vec postoji pojam sa istim odgovorima, probajte ponovo" << endl;
		deleteSomeone(ime);
		return;
	}
}

void GuessingSystem::guess(int brojPokusaja)
{
	cout << "Vreme je za Akinatora" << endl;

	cout << "Zamisli jedan od ovih pojmova:" << endl;

	stampajImena();

	string trenutniOdgovor; // Ovo ce da nam cuva trenutni odgovor koji se formira nakon svakog pitanja
	int brojac = 0; // Ovime cuvam 'i' van petlje, a treba mi za slucaj try

	for (int i = 0; i < pitanja.size(); i++) {
		brojac = i;
		cout << pitanja[i];
		cout << " (";
		for (const auto& pair : sviOdgovori[i]) {
			cout << pair.first << " ";
		}
		cout << ")" << endl;

		string odgovor;
		cin >> odgovor;

		bool validanOdgovor = false; // Proveravamo da li je uneti odgovor validan

		for (const auto& pair : sviOdgovori[i]) {
			if (pair.first == odgovor) {
				trenutniOdgovor += to_string(pair.second);
				validanOdgovor = true;
			}
		}

		if (validanOdgovor == false) {
			cout << "Nevalidan odgovor" << endl;
			i--;
			continue;
		}

		int slicniOdgovori = 0; // Ovo ce da nam broji koliko odgovora je slicno sa nasim

		string daNijeMozda; // Odgovor da li smo pogodili

		string tacanOdgovor;

		for (const auto& pair : mapaImena) {
			if (pair.second.substr(0, i+1) == trenutniOdgovor) {

				tacanOdgovor = pair.first;
				slicniOdgovori++;
				if (slicniOdgovori >= 2) {
					break;
				}

			}
		}

		if (brojPokusaja == i + 1) {
			cout << "Da li je ovo tacan odgovor: " << tacanOdgovor << "(Da, Ne)" << endl;
			cin >> daNijeMozda;
			if (daNijeMozda == "Da") {
				cout << "Tacan odgovor je: " << tacanOdgovor << endl;
				return;
			}
			else {
				break;
			}

		}

		if (slicniOdgovori >= 2) {
			continue;
		}
		if (slicniOdgovori == 0) {
			cout << "Ne postoji zadati pojam" << endl;
			return;
		}

		cout << "Tacan odgovor je: " << tacanOdgovor << endl;
		return;
	}

	cout << "Moguci odgovori: " << endl;

	for (const auto& pair : mapaImena) {
		const string& kljuc = pair.first;

		if (pair.second.substr(0, brojac + 1) == trenutniOdgovor) {
			cout << kljuc << endl;
		}
	}

}

void GuessingSystem::info(const string& ime)
{
	cout << "Informacije o " << ime << endl;

	for (const auto& pair : mapaImena) {
		
		if (pair.first == ime) {
			for (int i = 0; i < pitanja.size(); i++)
			{
				cout << pitanja[i] << endl;
				nadjiOdgovore(pair.second[i], i);
			}
			break;
		}

	}
}

void GuessingSystem::stampajPitanja()
{
	for (const auto& t : pitanja) {
		cout << t << endl;
	}
}

void GuessingSystem::ucitajPaProveri(const vector<string>& tokeni) // Dodajemo sve odgovore
{
	imena.push_back(tokeni.back());

	if (sviOdgovori.empty()) {
		for (int i = 0; i < pitanja.size(); i++)
		{
			sviOdgovori.push_back({ {tokeni[i], 0}}); // Ako su prazni odgovori, ubacujemo svuda po jedan odgovor sa vrednosti 1
		}
	}
	else {
		for (int i = 0; i < pitanja.size(); i++) {
			if (sviOdgovori[i].count(tokeni[i]) > 0) { // Ako vec postoji odgovor, idemo dalje
				continue;
			}
			else { // Ako ne postoji odgovor, ubacujemo ga
				int maxVrednost = -1;

				for (const auto& pair : sviOdgovori[i]) { // Ovako najdemo koji je sledeci broj koji vezujemo za odgovor
					if (pair.second > maxVrednost) {
						maxVrednost = pair.second;
					}
				}

				sviOdgovori[i][tokeni[i]] = maxVrednost + 1;
			}
		}
	}
}

void GuessingSystem::stampajSveOdgovore()
{

	for (const auto& mape : sviOdgovori) {
		for (const auto& pair : mape) {
			cout << pair.first << ": " << pair.second << " ";
		}
		cout << endl;
	}
}

void GuessingSystem::stampajImena()
{
	for (const auto& ime : imena) {
		cout << ime << endl;
	}
}

void GuessingSystem::mapiraj(const vector<string>& tokeni)
{
	for (int i = 0; i < pitanja.size(); i++)
	{
		mapaImena[imena.back()] += to_string(sviOdgovori[i][tokeni[i]]);
	}
}

void GuessingSystem::stampajMapu()
{
	for (const auto& ime : imena) {
		cout << ime << " " << mapaImena[ime] << endl;
	}
}

void GuessingSystem::resetuj()
{
	pitanja.clear();
	sviOdgovori.clear();
	imena.clear();
	mapaImena.clear();
}

void GuessingSystem::nadjiOdgovore(char brojOdgovor, int brojac)
{
	for (const auto& pair : sviOdgovori[brojac]) {
		if (pair.second == (brojOdgovor - '0')) {
			cout << pair.first << endl;
		}
	}
}
