#include "PlikZAdresatami.h"

bool PlikZAdresatami:: dopiszAdresataDoPliku(Adresat adresat)
{
    string liniaZDanymiAdresata = "";
    fstream plikTekstowy;
    plikTekstowy.open(pobierzNazwePliku().c_str(), ios::out | ios::app);

    if (plikTekstowy.good() == true)
    {
        liniaZDanymiAdresata = zamienDaneAdresataNaLinieZDanymiOddzielonePionowymiKreskami(adresat);

        if (czyPlikJestPusty() == true)
        {
            plikTekstowy << liniaZDanymiAdresata ;
        }
        else
        {
            plikTekstowy << endl << liniaZDanymiAdresata ;
        }
        idOstatniegoAdresata++;
        plikTekstowy.close();
        return true;
    }
    return false;
}

bool PlikZAdresatami::usunAdresataZPliku(vector <Adresat> &adresaci)
{
    int idUsuwanegoAdresata = 0;
    idUsuwanegoAdresata = usunAdresata(adresaci);

    if(idUsuwanegoAdresata > 0)
    {
        if(idUsuwanegoAdresata == pobierzIdOstatniegoAdresata())
    idOstatniegoAdresata = idUsuwanegoAdresata - 1;

    return true;
    }
    else
        return false;
}

int PlikZAdresatami::usunAdresata(vector <Adresat> &adresaci)
{
    int idUsuwanegoAdresata = 0;
    int numerLiniiUsuwanegoAdresata = 0;

    system("cls");
    cout << ">>> USUWANIE WYBRANEGO ADRESATA <<<" << endl << endl;
    idUsuwanegoAdresata = podajIdWybranegoAdresata();

    char znak;
    bool czyIstniejeAdresat = false;

    for (vector <Adresat>::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++)
    {
        if (itr -> pobierzId() == idUsuwanegoAdresata)
        {
            czyIstniejeAdresat = true;
            cout << endl << "Potwierdz naciskajac klawisz 't': ";
            znak = MetodyPomocnicze::wczytajZnak();
            if (znak == 't')
            {
                usunWybranaLinieWPliku(idUsuwanegoAdresata);
                adresaci.erase(itr);

                return idUsuwanegoAdresata;
            }
            else
            {
                return 0;
            }
        }
    }
    if (czyIstniejeAdresat == false)
    {
        cout << endl << "Nie ma takiego adresata w ksiazce adresowej" << endl << endl;
        system("pause");
    }
    return 0;
}

void PlikZAdresatami::usunWybranaLinieWPliku(int idUsuwanegoAdresata)
{
    fstream odczytywanyPlikTekstowy, tymczasowyPlikTekstowy;
    string nazwaTymczasowegoPlikuZAdresatami = "Adresaci_tymczasowo.txt";
    string wczytanaLinia = "";
    int numerWczytanejLinii = 1;
    int idSprawdzanegoAdresata = 0;

    odczytywanyPlikTekstowy.open(pobierzNazwePliku().c_str(), ios::in);
    tymczasowyPlikTekstowy.open(nazwaTymczasowegoPlikuZAdresatami.c_str(), ios::out | ios::app);

    if (odczytywanyPlikTekstowy.good() == true && idUsuwanegoAdresata != 0)
    {
        while (getline(odczytywanyPlikTekstowy, wczytanaLinia))
        {
            idSprawdzanegoAdresata = pobierzIdAdresataZDanychOddzielonychPionowymiKreskami(wczytanaLinia);

            if (idSprawdzanegoAdresata == idUsuwanegoAdresata) {}
            else if (numerWczytanejLinii == 1 && idSprawdzanegoAdresata != idUsuwanegoAdresata)
                tymczasowyPlikTekstowy << wczytanaLinia;
            else if (numerWczytanejLinii > 1 && idSprawdzanegoAdresata != idUsuwanegoAdresata)
                tymczasowyPlikTekstowy << endl << wczytanaLinia;

            numerWczytanejLinii++;
        }
        odczytywanyPlikTekstowy.close();
        tymczasowyPlikTekstowy.close();

        usunPlik(pobierzNazwePliku());
        zmienNazwePliku(nazwaTymczasowegoPlikuZAdresatami, pobierzNazwePliku());
    }
}

int PlikZAdresatami::podajIdWybranegoAdresata()
{
    int idWybranegoAdresata = 0;
    cout << "Podaj numer ID Adresata: ";
    idWybranegoAdresata  = MetodyPomocnicze::wczytajLiczbeCalkowita();
    return idWybranegoAdresata;
}

void PlikZAdresatami::usunPlik(string nazwaPlikuZRozszerzeniem)
{
    if (remove(nazwaPlikuZRozszerzeniem.c_str()) == 0) {}
    else
        cout << "Nie udalo sie usunac pliku " << nazwaPlikuZRozszerzeniem << endl;
}

void PlikZAdresatami::zmienNazwePliku(string staraNazwa, string nowaNazwa)
{
    if (rename(staraNazwa.c_str(), nowaNazwa.c_str()) == 0) {}
    else
        cout << "Nazwa pliku nie zostala zmieniona." << staraNazwa << endl;
}

string PlikZAdresatami::zamienDaneAdresataNaLinieZDanymiOddzielonePionowymiKreskami(Adresat adresat)
{
    string liniaZDanymiAdresata = "";

    liniaZDanymiAdresata += MetodyPomocnicze::konwersjaIntNaString(adresat.pobierzId()) + '|';
    liniaZDanymiAdresata += MetodyPomocnicze::konwersjaIntNaString(adresat.pobierzIdUzytkownika()) + '|';
    liniaZDanymiAdresata += adresat.pobierzImie() + '|';
    liniaZDanymiAdresata += adresat.pobierzNazwisko() + '|';
    liniaZDanymiAdresata += adresat.pobierzNumerTelefonu() + '|';
    liniaZDanymiAdresata += adresat.pobierzEmail() + '|';
    liniaZDanymiAdresata += adresat.pobierzAdres() + '|';

    return liniaZDanymiAdresata;
}


vector <Adresat> PlikZAdresatami::wczytajAdresatowZalogowanegoUzytkownikaZPliku(int idZalogowanegoUzytkownika)
{
    vector <Adresat> adresaci;
    Adresat adresat;
    string daneJednegoAdresataOddzielonePionowymiKreskami = "";
    string daneOstatniegoAdresataWPliku = "";
    fstream plikTekstowy;
    plikTekstowy.open(pobierzNazwePliku().c_str(), ios::in);

    if (plikTekstowy.good() == true)
    {
        while (getline(plikTekstowy, daneJednegoAdresataOddzielonePionowymiKreskami))
        {
            if (idZalogowanegoUzytkownika == pobierzIdUzytkownikaZDanychOddzielonychPionowymiKreskami(daneJednegoAdresataOddzielonePionowymiKreskami))
            {
                adresat = pobierzDaneAdresata(daneJednegoAdresataOddzielonePionowymiKreskami);
                adresaci.push_back(adresat);
            }
        }
        daneOstatniegoAdresataWPliku = daneJednegoAdresataOddzielonePionowymiKreskami;
        plikTekstowy.close();
    }

    if (daneOstatniegoAdresataWPliku != "")
    {
        idOstatniegoAdresata = pobierzIdAdresataZDanychOddzielonychPionowymiKreskami(daneOstatniegoAdresataWPliku);
    }
    return adresaci;
}

int PlikZAdresatami::pobierzIdUzytkownikaZDanychOddzielonychPionowymiKreskami(string daneJednegoAdresataOddzielonePionowymiKreskami)
{
    int pozycjaRozpoczeciaIdUzytkownika = daneJednegoAdresataOddzielonePionowymiKreskami.find_first_of('|') + 1;
    int idUzytkownika = MetodyPomocnicze::konwersjaStringNaInt(pobierzLiczbe(daneJednegoAdresataOddzielonePionowymiKreskami, pozycjaRozpoczeciaIdUzytkownika));

    return idUzytkownika;
}

string PlikZAdresatami::pobierzLiczbe(string tekst, int pozycjaZnaku)
{
    string liczba = "";
    while(isdigit(tekst[pozycjaZnaku]) == true)
    {
        liczba += tekst[pozycjaZnaku];
        pozycjaZnaku ++;
    }
    return liczba;
}

Adresat PlikZAdresatami::pobierzDaneAdresata(string daneAdresataOddzielonePionowymiKreskami)
{
    Adresat adresat;
    string pojedynczaDanaAdresata = "";
    int numerPojedynczejDanejAdresata = 1;

    for (int pozycjaZnaku = 0; pozycjaZnaku < daneAdresataOddzielonePionowymiKreskami.length(); pozycjaZnaku++)
    {
        if (daneAdresataOddzielonePionowymiKreskami[pozycjaZnaku] != '|')
        {
            pojedynczaDanaAdresata += daneAdresataOddzielonePionowymiKreskami[pozycjaZnaku];
        }
        else
        {
            switch(numerPojedynczejDanejAdresata)
            {
            case 1:
                adresat.ustawId(atoi(pojedynczaDanaAdresata.c_str()));
                break;
            case 2:
                adresat.ustawIdUzytkownika(atoi(pojedynczaDanaAdresata.c_str()));
                break;
            case 3:
                adresat.ustawImie(pojedynczaDanaAdresata);
                break;
            case 4:
                adresat.ustawNazwisko(pojedynczaDanaAdresata);
                break;
            case 5:
                adresat.ustawNumerTelefonu(pojedynczaDanaAdresata);
                break;
            case 6:
                adresat.ustawEmail(pojedynczaDanaAdresata);
                break;
            case 7:
                adresat.ustawAdres(pojedynczaDanaAdresata);
                break;
            }
            pojedynczaDanaAdresata = "";
            numerPojedynczejDanejAdresata++;
        }
    }
    return adresat;
}

int PlikZAdresatami::pobierzIdAdresataZDanychOddzielonychPionowymiKreskami(string daneJednegoAdresataOddzielonePionowymiKreskami)
{
    int pozycjaRozpoczeciaIdAdresata = 0;
    int idAdresata = MetodyPomocnicze::konwersjaStringNaInt(pobierzLiczbe(daneJednegoAdresataOddzielonePionowymiKreskami, pozycjaRozpoczeciaIdAdresata));
    return idAdresata;
}

int PlikZAdresatami::pobierzIdOstatniegoAdresata()
{
    return idOstatniegoAdresata;
}
