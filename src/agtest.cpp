/*
  this is a test for AppGen
*/

#include <cstdlib>
#include <string>
#include <iostream>

/*AppGen:Global*/

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
int main()
{
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
