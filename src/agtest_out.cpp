/*
  dies ist ein test für AppGen
*/

#include <cstdlib>
#include <string>
#include <iostream>

/*AppGen:Global*/
#include <getopt.h>
#include <string>
#include <string>
#include <string>
#include "filelist.h"

bool verbose = false;
/*AppGen:GlobalEnd*/

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
string ag_programName;

void usage()
{
  cout << ag_programName << " - Testprogramm für AppGen" << endl;
  cout << "Usage:" << endl;
  cout << ag_programName << " [<options>] quelle [ziel] [dateien] " << endl;
  cout << "  quelle  - Quelldatei" << endl;
  cout << "            Dies ist eine nähere Erklärung zum Parameter quelle" << endl;
  cout << "            Dies ist noch eine Erklärung zum Parameter quelle" << endl;
  cout << "            Dies ist die letzte Erklärung zum Parameter quelle" << endl;
  cout << "  ziel    - Zieldatei (optional, default:\"nothing\")" << endl;
  cout << "  dateien - Bilddateien (optional, default:)" << endl;
  cout << "Options:" << endl;
  cout << "  -h --help      Hilfe" << endl;
  cout << "  -H --longhelp  Ausführliche  Hilfe" << endl;
  cout << "  -x --xsize     Bildgröße X (default: 800)" << endl;
  cout << "                 Dies ist eine Erklärung zu xsize" << endl;
  cout << "  -y --ysize     Bildgröße Y (default: 600)" << endl;
  cout << "  -c --char      Ein Zeichen (default: 'b')" << endl;
  cout << "  -v --verbose   Ausführliche Meldungen (default: false)" << endl;
  cout << "  -o --output    Ausgabe-Datei (default: \"out.txt\")" << endl;
  cout << "  -f --factor    Faktor (default: 3.14)" << endl;
  exit(1);
}

void help()
{
  std::cout << "Dies ist ein Programm, welches nur zum Testen des Programmes"<< endl;
  std::cout << "AppGen dient. Es erfüllt sonst quasi keinen Zweck."<< endl;
  std::cout << "üüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüü"<< endl;
  std::cout << "Bitte teste ganz genau!"<< endl;
  exit(1);
}
void error(const string &msg)
{
  cout << endl << ag_programName << " - error: " << msg << endl << endl;
  usage();
}

int ptoi(const char *para)
{
  char *end;
  int res = strtol(para, &end, 10);
  if (end == para)
    error(string("no int: ") + para);
  if (*end != 0)
    error(string("garbage in int: ") + para);
  return res;
}

double ptod(const char *para)
{
  char *end;
  double res = strtod(para, &end);
  if (end == para)
    error(string("no double: ") + para);
  if (*end != 0)
    error(string("garbage in double: ") + para);
  return res;
}

int main(int argc, char **argv)
{

int xsize = 800;
int ysize = 600;
char character = 'b';
string ofn = "out.txt";
double fac = 3.14;
string source = "nothing";
string dest = "nothing";
std::vector<std::string> files;
  static struct option ag_long_options[] =
  {
    {"help", no_argument, 0, 'h' },
    {"longhelp", no_argument, 0, 'H' },
    {"xsize", required_argument, 0, 'x' },
    {"ysize", required_argument, 0, 'y' },
    {"char", required_argument, 0, 'c' },
    {"verbose", no_argument, 0, 'v' },
    {"output", required_argument, 0, 'o' },
    {"factor", required_argument, 0, 'f' },
    {0,         0,                 0,  0 }
  };
  ag_programName = argv[0];
  int rc;
  opterr = 0;
  while ((rc = getopt_long(argc, argv, ":hHx:y:c:vo:f:", ag_long_options, NULL)) >= 0)
    {
      switch (rc)
        {
        case '?':
          error("Unknown option");
          break;
        case ':':
          error("Expecting option parameter");
          break;
        case 'h':
              usage();
              break;

        case 'H':
              help();
              break;

        case 'x':
              xsize = ptoi(optarg);
              break;

        case 'y':
              ysize = ptoi(optarg);
              break;

        case 'c':
              character = optarg[0];
              break;

        case 'v':
              verbose = true;
              break;

        case 'o':
              ofn = optarg;
              break;

        case 'f':
              fac = ptod(optarg);
              break;

        default:
          error("error in options");
        }
    }
  if (optind < argc)
    source = argv[optind++];
  else error("Parameter quelle needed");

  if (optind < argc)
    dest = argv[optind++];

  if (optind < argc)
    FileList(argc, argv, optind, files, false);



  std::cout << "xsize: " << xsize << std::endl;
  std::cout << "ysize: " << ysize << std::endl;
  std::cout << "character: " << character << std::endl;
  std::cout << "verbose: " << verbose << std::endl;
  std::cout << "ofn: " << ofn << std::endl;
  std::cout << "fac: " << fac << std::endl;
  std::cout << "source: " << source << std::endl;
  std::cout << "dest: " << dest << std::endl;
  std::cout << "files: (" << files.size() << ")" << std::endl;
  for (unsigned int i=0;i<files.size();i++)
    std::cout << "files[" << i << "]" << ": " << files[i] << std::endl; 

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
