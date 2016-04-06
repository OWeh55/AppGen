#include "ag_variable.h"

using namespace std;

Variable* makeVariable(const string& type, string& line, bool isGlobal)
{
  if (type == "int")
    return new IntVariable(line, isGlobal);
  if (type == "char")
    return new CharVariable(line, isGlobal);
  else if (type == "void")
    return new VoidVariable(line, isGlobal);
  else if (type == "string")
    return new StringVariable(line, isGlobal);
  else if (type == "double")
    return new DoubleVariable(line, isGlobal);
  else if (type == "usage")
    return new UsageVariable(line, isGlobal);
  else if (type == "bool")
    return new BoolVariable(line, isGlobal);
  else if (type == "list")
    return new ListVariable(line, isGlobal);
  else if (type == "filelist")
    return new FileListVariable(line, isGlobal);
  else
    throw "unbekannter Typ";
}
