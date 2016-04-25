#include "ag_option.h"

Option::Option(std::string line)
{
  shortOption = getchar(line);
  longOption = getword(line);
  type = gettype(line);
  bool global = isupper(type[0]);
  type = tolower(type);
  var = makeVariable(type, line, global);
  needspara = var->needsPara();
}

Option::operator std::string() const
{
  std::string res;
  res += std::string("-") + shortOption + " ";
  res += std::string("--") + longOption + " ";
  res += std::string(*var) + " ";
  return res;
}

std::string Option::getUsage(int space, int maxlen) const
{
  std::ostringstream os;
  os << "  cout << \"";
  for (int i = 0; i < space; i++)
    os << " ";

  std::string formattedShortOption = std::string("-") + shortOption;
  while ((int)formattedShortOption.size() < maxlen + 2)
    formattedShortOption += " ";

  std::string formattedLongOption = std::string("--") + longOption;
  while ((int)formattedLongOption.size() < maxlen + 2)
    formattedLongOption += " ";

  std::string parameter = "   ";
  if (type == "int") parameter = "<i>";
  else if (type == "double") parameter = "<d>";
  else if (type == "string") parameter = "<s>";
  else if (type == "bool") parameter = "<b>";
  else if (type == "char") parameter = "<c>";

  os << "-" << shortOption << " " << parameter << "    ";
  os << formattedLongOption << " " << parameter;
  os << "\" << endl;" << std::endl;

  os << "  cout << \"   ";
  for (int i = 0; i < space; i++)
    os << " ";

  os << var->getDescription(0);
  std::string def = var->getDefault();
  if (!def.empty())
    os << " (default: " << CEscape(def) << ")";
  os << "\" << endl;" << std::endl;

  // write code for extended description <opt2>
  int dlines = var->getDescriptionSize();
  for (int i = 1; i < dlines; i++)
    {
      os << std::endl;
      os << "  cout << \"";
      for (int k = 0; k < space + 3; k++)
        os << " ";
      os << var->getDescription(i);
      os << "\" << endl;";
    }

  return os.str();
}

std::string Option::getLongOptionStruct() const
{
  std::ostringstream os;
  os << "    {\"" << longOption << "\", ";
  if (needspara)
    os << "required_argument, ";
  else
    os << "no_argument, ";

  os << "0, ";
  os << "'" << shortOption << "' },";
  return os.str();
}

std::string Option::getOptionString() const
{
  std::string res;
  res += shortOption;
  if (needspara) res += ':';
  return res;
}

std::string Option::getCase() const
{
  std::ostringstream os;
  os << "        case '" << shortOption << "':" << std::endl;
  os << "              " << var->getCase() << std::endl;
  os << "              break;" << std::endl;
  return os.str();
}

Parameter::Parameter(std::string line)
{
  //  para: #name:quelle, #art:required,#typ:string,#var: source, Quelldatei, nothing
  name = getword(line);
  std::string aval = getkind(line);
  optional = aval != "required";
  type = gettype(line);
  bool global = isupper(type[0]);
  type = tolower(type);
  var = makeVariable(type, line, global);
}

Parameter::operator std::string() const
{
  std::string res = "name: " + name + " ";
  res += optional ? "optional " : "required ";
  res += std::string(*var);
  return res;
}

std::string Parameter::getExtendedName() const
{
  if (optional)
    return "[" + name + "]";
  else
    return name ;
}

std::string Parameter::getUsage(int space, int maxlen) const
{
  std::ostringstream os;
  os << "  cout << \"";
  for (int i = 0; i < space; i++)
    os << " ";
  std::string formattedName = name;
  while ((int)formattedName.length() < maxlen)
    formattedName += " ";
  os << formattedName << " - " << var->getDescription(0);
  if (optional)
    os << " (optional, default:" << CEscape(var->getDefault()) << ")";
  os << "\" << endl;";
  int dlines = var->getDescriptionSize();
  for (int i = 1; i < dlines; i++)
    {
      os << std::endl;
      os << "  cout << \"";
      for (int k = 0; k < space + maxlen + 3; k++)
        os << " ";
      os << var->getDescription(i);
      os << "\" << endl;";
    }
  return os.str();
}

std::string Parameter::getEvalPara() const
{
  std::ostringstream os;
  os << "  if (optind < argc)" << std::endl;
  os << "    " << var->getEvalPara() << std::endl;
  if (!optional)
    os << "  else error(\"Parameter " << name << " needed\");" << std::endl;
  return os.str();
}
