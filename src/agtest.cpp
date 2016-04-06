/*
  dies ist ein test für AppGen
*/

#include <cstdlib>
#include <string>
#include <iostream>

/*AppGen:Global*/

using namespace std;

/*AppGen
  %%  Beschreibung des Programmes:
  prog: Testprogramm für AppGen

  %% Beschreibung Parameter
  % symbolischerName, Art, Typ,   Variablenname, Erklärung, Default-Wert
  para: quelle, required, string, source, Quelldatei, nothing
  para2: Dies ist eine nähere Erklärung zum Parameter quelle
  para2: Dies ist noch eine Erklärung zum Parameter quelle
  para2: Dies ist die letzte Erklärung zum Parameter quelle
  %  para: worte, optional, list, worte, Bilddateien,
  para: ziel,   optional, string, dest, Zieldatei, nothing
  para: dateien, optional, filelist, files, Bilddateien,

  %% Beschreibung der Optionen
  % kurz-Option, lang-Option, Typ, Variablenname, Erklärung, Default-Wert
  opt: h, help,   usage,   ,    Hilfe
  opt: H, longhelp,   Usage,   , Ausführliche  Hilfe
: Dies ist ein Programm, welches nur zum Testen des Programmes
: AppGen dient. Es erfüllt sonst quasi keinen Zweck.
: üüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüü
: Bitte teste ganz genau!
  opt: x, xsize,  int,     xsize,    Bildgröße X,  800
  opt2: Dies ist eine Erklärung zu xsize
  opt: y, ysize,  int,      ysize,    Bildgröße Y,  600
  opt: c, char, char, character, Ein Zeichen, 'b'
  opt: v, verbose,  Void,   verbose,  Ausführliche Meldungen,  false
  opt: o, output,  string,  ofn,  Ausgabe-Datei, out.txt
  opt: f, factor, double,   fac,  Faktor, 3.14
AppGen*/

/*AppGen:Main*/
int main()
{
  /*AppGen:MainEnd*/
// Diese Zeile muss nach Anwendung von AppGen direkt nach dem Ende
// des generierten Teiles stehen

#if 0
  cout << "xsize: " << xsize << endl;
  cout << "ysize: " << ysize << endl;
  cout << "verbose: " << verbose << endl;
  cout << "outfile: " << ofn << endl;
  cout << "factor: " << fac << endl;
#endif
}
