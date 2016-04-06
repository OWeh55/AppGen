#ifndef AG_OPTION_H
#define AG_OPTION_H

#include <string>
#include <sstream>
#include "ag_parse.h"
#include "ag_variable.h"

class Parameter
{
public:
  Parameter(std::string line);

  virtual ~Parameter()
  {
    delete var;
  }

  virtual bool isGlobal() const
  {
    return var->isGlobal();
  }

  virtual bool isOptional() const
  {
    return optional;
  }

  virtual std::string getName() const
  {
    return name;
  }

  virtual operator std::string() const;

  // code generation
  virtual void neededIncludes(std::vector<std::string>& v)
  {
    var->neededIncludes(v);
  }

  virtual std::string getDefinition() const
  {
    return var->getDefinition();
  }

  virtual std::string getExtendedName() const;

  virtual std::string createOutput() const
  {
    return var->createOutput();
  }

  virtual std::string getUsage(int space = 2, int maxlen = 0) const;

  virtual std::string getEvalPara() const;

  virtual std::string getType() const
  {
    return type;
  }

  virtual void addDescription(const std::string& d)
  {
    var->addDescription(d);
  }

private:
  std::string name;
  bool optional;
  std::string type;
  Variable* var;
};

class Option
{
public:
  Option(std::string line);

  virtual ~Option()
  {
    delete var;
  }

  virtual operator std::string() const;

  virtual bool isGlobal() const
  {
    return var->isGlobal();
  }

  virtual char getShortOption() const
  {
    return shortOption;
  }

  virtual std::string getLongOption() const
  {
    return longOption;
  }

  // code generation
  virtual void neededIncludes(std::vector<std::string>& v)
  {
    var->neededIncludes(v);
  }

  virtual std::string getDefinition() const
  {
    return var->getDefinition();
  }

  virtual void addDescription(const std::string& d)
  {
    var->addDescription(d);
  }

  virtual std::string getUsage(int space = 2, int maxlen = 0) const;

  virtual std::string getLongOptionStruct() const ;

  virtual std::string getOptionString() const;

  virtual std::string getCase() const;

  virtual std::string createOutput() const
  {
    return var->createOutput();
  }

  virtual std::string getType() const
  {
    return type;
  }
protected:
  char shortOption;
  std::string longOption;
  bool needspara;
  std::string type;
  Variable* var;
};
#endif
