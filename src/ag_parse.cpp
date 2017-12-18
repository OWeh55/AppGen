#include "ag_parse.h"

using namespace std;

string tolower(const string& s)
{
  string res;
  for (unsigned int i = 0; i < s.length(); i++)
    res.push_back(tolower(s[i]));
  return res;
}

string trim(const string& s)
{
  // set first after leading spaces
  unsigned int first = 0;
  while (first < s.length() && s[first] == ' ')
    first++;
  if (first == s.length()) // only spaces
    return "";
  // set last before trailing spaces
  unsigned int last = s.length() - 1;
  while (last > first && s[last] == ' ')
    last--;
  // return substring first..last
  return s.substr(first, last - first + 1);
}

string trimLeft(const string& s)
{
  // remove leading spaces
  unsigned int first = 0;
  while (first < s.length() && s[first] == ' ')
    first++;
  if (first == s.length()) // only spaces
    return "";
  return s.substr(first);
}

string trimRight(const string& s)
{
  // remove trailing spaces
  int last = s.length() - 1;
  while (last > 0 && s[last] == ' ')
    last--;
  return s.substr(0, last + 1);
}

string nextWord(string& s)
{
  string res = "";
  unsigned int first = 0;
  while (first < s.length() && s[first] == ' ')
    first++;
  if (first == s.length()) // only spaces
    {
      s = "";
      return res;
    }
  unsigned int last = first;
  while (last < s.length() && s[last] != ' ')
    last++;
  res = s.substr(first, last - first);
  s = s.substr(last);
  return res;
}

string getstring(string& s)
{
  unsigned int i = 0;
  // alles bis zum Begrenzer übernehmen
  string result;
  while (i < s.length() && s[i] != del)
    result += s[i++];
  // Begrenzer überlesen
  if (i < s.length() && s[i] == ',')
    i++;
  // Rest behalten
  string remainder;
  while (i < s.length())
    remainder += s[i++];
  s = remainder;
  return result;
}

string getword(string& s)
{
  string word = trim(getstring(s));
  for (unsigned int i = 0; i < word.length(); i++)
    if (word[i] == ' ')
      throw "single word expected";
  return word;
}

char getchar(string& s)
{
  string res = getword(s);
  if (res.length() != 1)
    throw "character expected";
  return res[0];
}

string gettype(string& s)
{
  string res = getword(s);
  string h = tolower(res);
  if (h == "string" || h == "int" || h == "double"
      || h == "usage" || h == "bool" || h == "char"
      || h == "list" || h == "filelist" || h == "void")
    return res;
  throw "type expected";
}

string getkind(string& s)
{
  string res = getword(s);
  if (res == "required" || res == "optional")
    return res;
  throw "kind of parameter expected";
}

string CEscape(const string& s)
{
  // escape string for use as c string
  string res;
  for (unsigned int i = 0; i < s.length(); i++)
    {
      if (s[i] == '"')
        res += string("\\\"");
      else if (s[i] == '\\')
        res += string("\\\\");
      else
        res += s[i];
    }
  return res;
}
