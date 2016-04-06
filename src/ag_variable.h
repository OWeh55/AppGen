#ifndef AG_VARIABLE_H
#define AG_VARIABLE_H

#include <sstream>
#include <vector>
#include <string>

#include "ag_parse.h"

class Variable
{

public:
  Variable(std::string& line, bool isGlobal)
  {
    variable = getword(line);
    description.push_back(trim(getstring(line)));
    defaultValue = trim(getstring(line));
    global = isGlobal;
  }

  virtual ~Variable() {}

  virtual void neededIncludes(std::vector<std::string>& includes)
  {
    // no includes
  }

  virtual std::string getDescription(int n) const
  {
    return description[n];
  }

  virtual int getDescriptionSize() const
  {
    return description.size();
  }

  virtual void addDescription(const std::string& d)
  {
    description.push_back(d);
  }

  virtual std::string getType() const = 0;

  virtual std::string getName() const
  {
    return variable;
  }

  virtual bool isGlobal() const
  {
    return global;
  }

  virtual bool needsPara() const
  {
    return true;
  }

  virtual std::string getDefault() const
  {
    return defaultValue;
  }

  // code generation
  virtual std::string getDefinition() const
  {
    std::string res = getType() + " " + variable;
    std::string def = getDefault();
    if (!def.empty())
      res += " = " + def;
    res += ";";
    return res;
  }

  virtual std::string getCase() const = 0;

  virtual std::string getEvalPara() const
  {
    return variable + " = argv[optind++];";
  }

  virtual std::string createOutput() const
  {
    std::ostringstream os;
    os << "  std::cout << \"" << variable << ": \" << " << variable << " << std::endl;";
    return os.str();
  }

  operator std::string() const
  {
    std::string res;
    res += getType() + " ";
    res += std::string("var: ") + variable + " ";
    res += std::string("desc: ") + description[0] + " ";
    if (!defaultValue.empty())
      res += std::string("default: ") + defaultValue;
    return res;
  }

protected:
  std::string variable;
  std::vector<std::string> description;
  std::string defaultValue;
  bool global;
};

class IntVariable: public Variable
{
public:
  IntVariable(std::string& line, bool isGlobal): Variable(line, isGlobal)
  {
  }

  virtual std::string getCase() const
  {
    return variable + " = ptoi(optarg);";
  }

  virtual std::string getType() const
  {
    return "int";
  }

private:
};

class CharVariable: public Variable
{
public:
  CharVariable(std::string& line, bool isGlobal): Variable(line, isGlobal)
  {
  }

  virtual std::string getCase() const
  {
    return variable + " = optarg[0];";
  }

  virtual std::string getType() const
  {
    return "char";
  }

private:
};

class DoubleVariable: public Variable
{
public:
  DoubleVariable(std::string& line, bool isGlobal): Variable(line, isGlobal)
  {
  }

  virtual std::string getCase() const
  {
    return variable + " = ptod(optarg);";
  }

  virtual std::string getType() const
  {
    return "double";
  }

private:
};

class StringVariable: public Variable
{
public:
  StringVariable(std::string& line, bool isGlobal): Variable(line, isGlobal)
  {
    if (defaultValue[0] != '"')
      defaultValue = '"' + defaultValue + '"';
  }

  virtual std::string getCase() const
  {
    return variable + " = optarg;";
  }

  virtual std::string getType() const
  {
    return "string";
  }

  virtual void neededIncludes(std::vector<std::string>& includes)
  {
    includes.push_back("<string>");
  }

private:
};

class BoolVariable: public Variable
{
public:
  BoolVariable(std::string& line, bool isGlobal): Variable(line, isGlobal)
  {
  }

  virtual std::string getCase() const
  {
    return variable + " = string(optarg)==\"true\";";
  }

  virtual std::string getType() const
  {
    return "bool";
  }

private:
};

class UsageVariable: public Variable
{
public:
  UsageVariable(std::string& line, bool isGlobal): Variable(line, isGlobal)
  {
  }

  virtual bool needsPara() const
  {
    return false;
  }

  virtual std::string getType() const
  {
    return "usage";
  }

  virtual std::string getCase() const
  {
    if (global)
      return "help();";
    else
      return "usage();";
  }

  virtual std::string createOutput() const
  {
    return "";
  }

  virtual std::string getDefinition() const
  {
    return "";
  }

private:
};

class VoidVariable: public Variable
{
public:
  VoidVariable(std::string& line, bool isGlobal): Variable(line, isGlobal)
  {
  }

  virtual bool needsPara() const
  {
    return false;
  }

  virtual std::string getCase() const
  {
    return variable + " = true;";
  }

  virtual std::string getType() const
  {
    return "bool";
  }

private:
};

class ListVariable: public Variable
{
public:
  ListVariable(std::string& line, bool isGlobal): Variable(line, isGlobal)
  {
  }

  virtual void neededIncludes(std::vector<std::string>& includes)
  {
    includes.push_back("<vector>");
  }

  virtual std::string getCase() const
  {
    throw "List is no option type";
  }

  virtual std::string getEvalPara() const
  {
    std::ostringstream os;
    os << "while (optind<argc)" << std::endl;
    os << "   " << variable << ".push_back(argv[optind++]);" << std::endl;
    return os.str();
  }

  virtual std::string createOutput() const
  {
    std::ostringstream os;
    os << "  std::cout << \"" << variable << ": (\" << " << variable << ".size() << \")\" << std::endl;" << std::endl;
    os << "  for (unsigned int i=0;i<" << variable << ".size();i++)" << std::endl;
    os << "    std::cout << \"" << variable << "[\" << i << \"]\" << \": \" << " << variable << "[i] << std::endl; " << std::endl;
    return os.str();
  }

  virtual std::string getType() const
  {
    return "std::vector<std::string>";
  }

private:
};

class FileListVariable: public Variable
{
public:
  FileListVariable(std::string& line, bool isGlobal): Variable(line, isGlobal)
  {
  }

  virtual void neededIncludes(std::vector<std::string>& includes)
  {
    includes.push_back("\"filelist.h\"");
  }

  virtual std::string getCase() const
  {
    throw "FileList is no option type";
  }

  virtual std::string getEvalPara() const
  {
    return "FileList(argc, argv, optind, " + variable + ", false);";
  }

  virtual std::string createOutput() const
  {
    std::ostringstream os;
    os << "  std::cout << \"" << variable << ": (\" << " << variable << ".size() << \")\" << std::endl;" << std::endl;
    os << "  for (unsigned int i=0;i<" << variable << ".size();i++)" << std::endl;
    os << "    std::cout << \"" << variable << "[\" << i << \"]\" << \": \" << " << variable << "[i] << std::endl; " << std::endl;
    return os.str();
  }

  virtual std::string getType() const
  {
    return "std::vector<std::string>";
  }

private:
};

Variable* makeVariable(const std::string& type, std::string& line, bool isGlobal);
#endif
