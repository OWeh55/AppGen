/*
  this is a test for AppGen
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
  %%  description of program
  prog: simple test for AppGen

  %% description of parameters
  % name, kind, c++ type , name of variable, description, default
  para: source, required, string, source, source filename, ""
  para2: this is a description of source (for help)
  para2: this is another description of source (for help)
  para: target, optional, string, target, target file name, ""
  para: file, optional, filelist, files, additional file[s],

  %% description of options
  % short option, long option, type, name of variable, description, default
  opt: h, help,   usage,   ,    Help
  opt: H, longhelp,   Usage,   ,  Verbose help
: This is arbitrary text which is written, if
: the program is called with option '-H'
: Aller Anfang ist schwer
: test test test test
  opt: x, xsize,  int,     xsize,    Size X,  800
  opt2: More about size
  opt: y, ysize,  int,     ysize,    Size Y,  600
  opt: c, char, char, character, one character, 'b'
  opt: v, verbose,  Void,   verbose,  Be verbose,  false
  opt: o, output,  string,  ofn,  output filename, out.txt
  opt: f, factor, double,   fac,  factor, 3.14
AppGen*/

/*AppGen:Main*/
string ag_programName;

void usage()
{
  cout << ag_programName << " - simple test for AppGen" << endl;
  cout << "Usage:" << endl;
  cout << ag_programName << " [<options>] source [target] [file] " << endl;
  cout << "  source - source filename" << endl;
  cout << "           this is a description of source (for help)" << endl;
  cout << "           this is another description of source (for help)" << endl;
  cout << "  target - target file name (optional, default:\"\")" << endl;
  cout << "  file   - additional file[s] (optional, default:)" << endl;
  cout << "Options:" << endl;
  cout << "  -h --help      Help" << endl;
  cout << "  -H --longhelp  Verbose help" << endl;
  cout << "  -x --xsize     Size X (default: 800)" << endl;
  cout << "                 More about size" << endl;
  cout << "  -y --ysize     Size Y (default: 600)" << endl;
  cout << "  -c --char      one character (default: 'b')" << endl;
  cout << "  -v --verbose   Be verbose (default: false)" << endl;
  cout << "  -o --output    output filename (default: \"out.txt\")" << endl;
  cout << "  -f --factor    factor (default: 3.14)" << endl;
  exit(1);
}

void help()
{
  std::cout << "This is arbitrary text which is written, if"<< endl;
  std::cout << "the program is called with option '-H'"<< endl;
  std::cout << "Aller Anfang ist schwer"<< endl;
  std::cout << "test test test test"<< endl;
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
string source = "";
string target = "";
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
  else error("Parameter source needed");

  if (optind < argc)
    target = argv[optind++];

  if (optind < argc)
    FileList(argc, argv, optind, files, false);



  std::cout << "xsize: " << xsize << std::endl;
  std::cout << "ysize: " << ysize << std::endl;
  std::cout << "character: " << character << std::endl;
  std::cout << "verbose: " << verbose << std::endl;
  std::cout << "ofn: " << ofn << std::endl;
  std::cout << "fac: " << fac << std::endl;
  std::cout << "source: " << source << std::endl;
  std::cout << "target: " << target << std::endl;
  std::cout << "files: (" << files.size() << ")" << std::endl;
  for (unsigned int i=0;i<files.size();i++)
    std::cout << "files[" << i << "]" << ": " << files[i] << std::endl; 

/*AppGen:MainEnd*/
// This line must be directly after the generated code

#if 0
  cout << "xsize: " << xsize << endl;
  cout << "ysize: " << ysize << endl;
  cout << "verbose: " << verbose << endl;
  cout << "outfile: " << ofn << endl;
  cout << "factor: " << fac << endl;
#endif
}
