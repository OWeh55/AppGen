/*
  this is a tool to generate code for
  handling of commandline parameters
  and options
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <cstdlib>
#include <getopt.h>

#include <locale>

#include "ag_parse.h"
#include "ag_option.h"

#define APPGEN_START "/*AppGen"
#define APPGEN_END "AppGen*/"
#define APPGEN_MAIN "/*AppGen:Main*/"
#define APPGEN_MAINEND "/*AppGen:MainEnd*/"
#define APPGEN_GLOBAL "/*AppGen:Global*/"
#define APPGEN_GLOBALEND "/*AppGen:GlobalEnd*/"

using namespace std;

bool lineNumber = true;

vector<string> source;
int  startMain = -1; // location of APPGEN_MAIN
int  endMain = -1; // location of APPGEN_MAINEND
int startGlobal = -1; // location of APPGEN_GLOBAL
int endGlobal = -1; // location of APPGEN_GLOBALEND

string programDesc;
vector<Parameter*> parameterDesc;
vector<Option*> optionDesc;
set<char> optionLetter;
vector<string> longHelp;
bool hasIntPara = false;
bool hasDoublePara = false;

bool readSource(const string& fn)
{
  const int st_pre = 1;  // reading before parameter description
  const int st_desc = 2; // reading parameter description
  const int st_post = 3; // reading past parameter description

  source.clear();

  int state = st_pre;
  ifstream is(fn.c_str());
  if (!is.good())
    {
      cerr << "Kann Datei nicht öffnen" << endl;
      return false;
    }

  string line;
  // linewise read
  while (state == st_pre && getline(is, line))
    {
      source.push_back(line);
      string hline = nextWord(line);
      if (hline == APPGEN_START)
        state = st_desc;
      else if (hline == APPGEN_MAIN)
        startMain =  source.size() - 1;
      else if (hline == APPGEN_MAINEND)
        endMain = source.size() - 1;
      else if (hline == APPGEN_GLOBAL)
        startGlobal =  source.size() - 1;
      else if (hline == APPGEN_GLOBALEND)
        endGlobal = source.size() - 1;
    }

  // read within parameter description block
  while (state == st_desc && getline(is, line))
    {

      source.push_back(line);
      string hline = nextWord(line);
      if (!hline.empty())
        {
          if (hline == APPGEN_END)
            state = st_post;
          else if (hline == "prog:")
            programDesc = trim(line);
          else if (hline == "para:")
            parameterDesc.push_back(new Parameter(line));
          else if (hline == "para2:")
            parameterDesc.back()->addDescription(trim(line));
          else if (hline == "opt:")
            {
              optionDesc.push_back(new Option(line));
              char so = optionDesc.back()->getShortOption();
              if (optionLetter.count(so) > 0)
                throw "short option character already used";
              optionLetter.insert(so);
              string type = optionDesc.back()->getType();
              if (type == "int")
                hasIntPara = true;
              if (type == "double")
                hasDoublePara = true;
            }
          else if (hline == "opt2:")
            {
              if (optionDesc.empty())
                throw "opt2: no previous opt:";
              optionDesc.back()->addDescription(trim(line));
            }
          else if (hline == ":")
            longHelp.push_back(trim(line));
          else if (hline[0] == '%')
            {
              // ignore comment
            }
          else
            throw "wrong syntax: ";
        }
    }

  while (state == st_post && getline(is, line))
    {
      source.push_back(line);
      string hline = nextWord(line);
      if (hline == APPGEN_MAIN)
        startMain =  source.size() - 1;
      else if (hline == APPGEN_MAINEND)
        endMain = source.size() - 1;
      else if (hline == APPGEN_GLOBAL)
        startGlobal =  source.size() - 1;
      else if (hline == APPGEN_GLOBALEND)
        endGlobal = source.size() - 1;
    }

  return true;
}

const int maxLineLength = 80;

/*
wstring S2WS(const string &s)
{
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  //  std::string narrow = converter.to_bytes(L"foo");
  std::wstring wide = converter.from_bytes(s);

  return ws;
}

string WS2S(const wstring &s)
{
  return string(s.begin(), s.end());
}

wstring getNextLine(const wstring &all,int &pos, int lng)
{
  wstring result;
  for (int i=0;pos<(int)all.size() && i<lng;++i)
    {
      result += all[pos];
      ++pos;
    }
  return result;
}

void reformatLongHelp()
{
  // long help -> one long line
  wstring allHelp;
  for (unsigned int i=0;i<longHelp.size();++i)
    {
      wstring thisLine= S2WS(longHelp[i]) + L" ";
      allHelp += thisLine;
    }
  // truncate sequences of white space
  wstring allHelp2;
  bool blank=true;
  for (unsigned int i=0;i<allHelp.size();++i)
    {
      if (blank && ! isblank(allHelp[i]))
  {
    blank=false;
  }
      if (!blank)
  allHelp2 += allHelp[i];
      blank = isblank(allHelp[i]);
    }
  longHelp.clear();
  // split to multiple lines again
  int currentPosition=0;
  while (currentPosition<(int)allHelp2.size())
    {
      longHelp.push_back(WS2S(getNextLine(allHelp2,currentPosition,79)));
    }
}
*/

string padded(const string& s, unsigned int len)
{
  string res = s;
  while (res.length() < len)
    res += " ";
  return '\"' + res + '\"';
}

void createUsage(ostream& os, bool debug = false)
{
  os << "void usage()" << endl;
  os << "{" << endl;
  os << "  cout << ag_programName";
  if (!programDesc.empty())
    os << " << \" - " << programDesc << "\"" ;
  os << " << endl;" << endl;
  os << "  cout << \"Usage:\" << endl;" << endl;
  os << "  cout << ag_programName << \" [<options>] ";
  for (unsigned int i = 0; i < parameterDesc.size(); i++)
    os << parameterDesc[i]->getExtendedName() << " ";
  os << "\" << endl;" << endl;

  int maxlen = 0;
  for (unsigned int i = 0; i < parameterDesc.size(); i++)
    {
      int alen = parameterDesc[i]->getName().length();
      if (alen > maxlen)
        maxlen = alen;
    }

  for (unsigned int i = 0; i < parameterDesc.size(); i++)
    os << parameterDesc[i]->getUsage(2, maxlen) << endl;

  os << "  cout << \"Options:\" << endl;" << endl;

  unsigned int solen = 0;
  unsigned int lolen = 0;
  unsigned int d0len = 0;

  for (unsigned int i = 0; i < optionDesc.size(); i++)
    {
      string so;
      string lo;
      vector<string> desc;
      optionDesc[i]->getUsage(so, lo, desc);
      if (so.length() > solen) solen = so.length();
      if (lo.length() > lolen) lolen = lo.length();
      if (desc[0].length() > d0len) d0len = desc[0].length();
    }

  if (lolen > 12)
    lolen = 12;

  int dpos = solen + lolen + 2 + 3;

  for (unsigned int i = 0; i < optionDesc.size(); i++)
    {
      string so;
      string lo;
      vector<string> desc;
      optionDesc[i]->getUsage(so, lo, desc);
      os << "  cout << " << padded(so, solen) << " << " << padded("", 2);
      os << " << " << padded(lo, lolen) << " << " << padded("", 3) << ";";
      if (lo.length() > lolen) // extra long option -> description on new line
        os << endl << "cout << endl << " << padded("", (dpos)) << "; ";
      os << endl << "  cout << \"" << desc[0] << "\"";
      os << " << endl ; " << endl;
      for (unsigned int i = 1; i < desc.size(); i++)
        {
          os << "  cout << " << padded("", dpos);
          os << " << " << padded(desc[i], 0);
          os << " << endl; " << endl;
        }
    }

  os << "  exit(1);" << endl;
  os << "}" << endl << endl;
}

void createHelp(ostream& os, bool debug = false)
{
  if (!longHelp.empty())
    {
      //    reformatLongHelp();
      os << "void help()" << endl;
      os << "{" << endl;
      for (unsigned int i = 0; i < longHelp.size(); ++i)
        os << "  std::cout << \"" << longHelp[i] << "\"<< endl;" << endl;
      os << "  exit(1);" << endl;
      os << "}" << endl;
    }
}

void createError(ostream& os, bool debug = false)
{
  os << "void error(const string &msg)" << endl;
  os << "{" << endl;
  os << "  cout << endl << ag_programName << \" - error: \" << msg << endl << endl;" << endl;
  os << "  usage();" << endl;
  os << "}" << endl << endl;
}

void writeptol(ostream& os, bool debug = false)
{
  os << "long int ptol(const char *para)" << endl;
  os << "{" << endl;
  os << "  char *end;" << endl;
  os << "  long int res = strtol(para, &end, 10);" << endl;
  os << "  if (end == para)" << endl;
  os << "    error(string(\"no int: \") + para);" << endl;
  os << "  if (*end != 0)" << endl;
  os << "    error(string(\"garbage in integer value: \") + para);" << endl;
  os << "  return res;" << endl;
  os << "}" << endl << endl;
}

void writeptod(ostream& os, bool debug = false)
{
  os << "double ptod(const char *para)" << endl;
  os << "{" << endl;
  os << "  char *end;" << endl;
  os << "  double res = strtod(para, &end);" << endl;
  os << "  if (end == para)" << endl;
  os << "    error(string(\"no double: \") + para);" << endl;
  os << "  if (*end != 0)" << endl;
  os << "    error(string(\"garbage in floating point value: \") + para);" << endl;
  os << "  return res;" << endl;
  os << "}" << endl << endl;
}

void createMain(ostream& os, bool debug = false)
{
  os << "string ag_programName;" << endl << endl;

  createUsage(os, debug);
  createHelp(os, debug);
  createError(os, debug);

  if (hasIntPara)
    writeptol(os, debug);

  if (hasDoublePara)
    writeptod(os, debug);

  os << "int main(int argc, char **argv)" << endl;
  os << "{" << endl;

  // variables for option values
  for (unsigned int i = 0; i < optionDesc.size(); i++)
    {
      if (! optionDesc[i]->isGlobal())
        os << optionDesc[i]->getDefinition() << endl;
    }

  // variables for parameter values
  for (unsigned int i = 0; i < parameterDesc.size(); i++)
    {
      if (! parameterDesc[i]->isGlobal())
        os << parameterDesc[i]->getDefinition() << endl;
    }

  os << "  static struct option ag_long_options[] =" << endl;
  os << "  {" << endl;
  for (unsigned int i = 0; i < optionDesc.size(); i++)
    os << optionDesc[i]->getLongOptionStruct() << endl;
  os << "    {0,         0,                 0,  0 }" << endl;
  os << "  };" << endl;

  os << "  ag_programName = argv[0];" << endl;
  os << "  int rc;" << endl;
  os << "  opterr = 0;" << endl;

  string optionstring = ":";
  for (unsigned int i = 0; i < optionDesc.size(); i++)
    optionstring += optionDesc[i]->getOptionString();

  os << "  while ((rc = getopt_long(argc, argv, \"" << optionstring;
  os << "\", ag_long_options, NULL)) >= 0)" << endl;
  os << "    {" << endl;
  os << "      switch (rc)" << endl;
  os << "        {" << endl;
  os << "        case '?':" << endl;
  os << "          error(\"Unknown option\");" << endl;
  os << "          break;" << endl;
  os << "        case ':':" << endl;
  os << "          error(\"Expecting option parameter\");" << endl;
  os << "          break;" << endl;

  for (unsigned int i = 0; i < optionDesc.size(); i++)
    os << optionDesc[i]->getCase() << endl;

  os << "        default:" << endl;
  os << "          error(\"error in options\");" << endl;
  os << "        }" << endl;
  os << "    }" << endl;

  for (unsigned int i = 0; i < parameterDesc.size(); i++)
    os <<  parameterDesc[i]->getEvalPara() << endl;

  if (debug)
    {
      for (unsigned int i = 0; i < optionDesc.size(); i++)
        os << optionDesc[i]->createOutput() << endl;
      for (unsigned int i = 0; i < parameterDesc.size(); i++)
        os << parameterDesc[i]->createOutput() << endl;
    }

  os << APPGEN_MAINEND << endl;
}

void createGlobal(ostream& os, bool debug = false)
{
  // collect includes
  vector<string> includes;
  includes.push_back("<getopt.h>");
  includes.push_back("<string>");
  includes.push_back("<iomanip>");

  for (unsigned int i = 0; i < optionDesc.size(); i++)
    optionDesc[i]->neededIncludes(includes);
  for (unsigned int i = 0; i < parameterDesc.size(); i++)
    parameterDesc[i]->neededIncludes(includes);

  for (unsigned int i = 0; i < includes.size(); i++)
    os << "#include " << includes[i] << endl;

  // Zielvariablen für Optionen anlegen
  for (unsigned int i = 0; i < optionDesc.size(); i++)
    {
      if (optionDesc[i]->isGlobal())
        os << optionDesc[i]->getDefinition() << endl;
    }
  // Zielvariablen für Parameter anlegen
  for (unsigned int i = 0; i < parameterDesc.size(); i++)
    {
      if (parameterDesc[i]->isGlobal())
        os << parameterDesc[i]->getDefinition() << endl;
    }
  os << APPGEN_GLOBALEND << endl;
}

bool writeDest(const string& dest, const string& src, bool debug = false)
{
  ofstream os(dest.c_str());
  for (unsigned int i = 0; i < source.size(); i++)
    {
      if (lineNumber)
        os << "# " << (i + 1) << " \"" << src << "\"" << endl;
      os << source[i] << endl;
      if (source[i] == APPGEN_MAIN)
        {
          createMain(os, debug);
          if (endMain >= 0)
            i = endMain;
        }
      else if (source[i] == APPGEN_GLOBAL)
        {
          createGlobal(os, debug);
          if (endGlobal >= 0)
            i = endGlobal;
        }
    }
  return true;
}

string ag_programName;

void usage()
{
  cout << ag_programName << " - application program generator" << endl;
  cout << "usage: " << endl;
  cout << ag_programName << " [<options>] source target " << endl;
  cout << "  source - C++ source file with parameter/option description" << endl;
  cout << "  target - C++ file to generate" << endl;
  cout << "options:" << endl;
  cout << "  -h --help     this help" << endl;
  cout << "  -v --verbose  verbose messages (default: false)" << endl;
  cout << "  -d --debug    created application outputs parameters (default: false)" << endl;
  cout << "  -l --no_line_number  No line numbers in created C++ code" << endl;
  exit(1);
}

void error(const string& msg)
{
  cout << endl;
  if (source.size() > 0)
    cout << ">>" << source.back() << "<<" << endl;
  cout << ag_programName << " - error: " << msg << endl << endl;
  usage();
}

int ptoi(const char* para)
{
  char* end;
  int res = strtol(para, &end, 10);
  if (end == para)
    error(string("no int: ") + para);
  if (*end != '\0')
    error(string("garbage in int: ") + para);
  return res;
}

double ptod(const char* para)
{
  char* end;
  double res = strtod(para, &end);
  if (end == para)
    error(string("no double: ") + para);
  if (*end != '\0')
    error(string("garbage in double: ") + para);
  return res;
}

int main(int argc, char** argv)
{
  bool verbose = false;
  bool debug = false;

  string source = "nothing";
  string dest = "<<";
  static struct option ag_long_options[] =
  {
    {"help", required_argument, 0, 'h' },
    {"verbose", no_argument, 0, 'v' },
    {"debug", no_argument, 0, 'd' },
    {"no_line_number", no_argument, 0, 'l' },
    {0,         0,                 0,  0 }
  };

  ag_programName = argv[0];
  int rc;
  opterr = 0;
  while ((rc = getopt_long(argc, argv, ":h:vdl", ag_long_options, NULL)) >= 0)
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
          break;
        case 'v':
          verbose = true;
          break;
        case 'd':
          debug = true;
          break;
        case 'l':
          lineNumber = false;
          break;
        default:
          error("error in options");
        }
    }

  if (optind < argc)
    source = argv[optind++];
  else error("expecting parameter source");

  if (optind < argc)
    dest = argv[optind++];
  else error("expection parameter target");

  try
    {
      if (readSource(source))
        {
          if (verbose)
            {
              cout << programDesc << endl;
              for (unsigned int i = 0; i < parameterDesc.size(); i++)
                cout << string(*parameterDesc[i]) << endl;
              for (unsigned int i = 0; i < optionDesc.size(); i++)
                cout << string(*optionDesc[i]) << endl;
            }
        }
      else
        error("Cannot read " + source);

      // simple tests
      if (parameterDesc.size() == 0 && optionDesc.size() == 0)
        error("No parameter description");

      if (startMain < 0)
        error("No insertion point for main() found (" APPGEN_MAIN ")");

      bool hasOptional = false;
      for (unsigned int i = 0; i < parameterDesc.size(); i++)
        {
          if (hasOptional && !parameterDesc[i]->isOptional())
            error("optional parameter before mandatory parameters");
          hasOptional = parameterDesc[i]->isOptional();
        }

      for (int i = 0; i < ((int)parameterDesc.size()) - 1; i++)
        {
          if (parameterDesc[i]->getType() == "list" ||
              parameterDesc[i]->getType() == "filelist")
            error("Lists have to be last parameters");
        }

      if (endMain >= 0 && endMain < startMain)
        error(APPGEN_MAINEND " before " APPGEN_MAIN);

      if (!writeDest(dest, source, debug))
        error("Cannot write " + dest);

    }
  catch (const char* msg)
    {
      error(msg);
    }
}
