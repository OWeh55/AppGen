#ifndef AG_PARSE
#define AG_PARSE

#include <string>

const char del = ',';

std::string getstring(std::string& s);
std::string nextWord(std::string& s);

std::string getword(std::string& s);
char getchar(std::string& s);
std::string gettype(std::string& s);
std::string getkind(std::string& s);

std::string trim(const std::string& s);
std::string trimRight(const std::string& s);
std::string trimLeft(const std::string& s);

std::string tolower(const std::string& s);
std::string CEscape(const std::string& s);

#endif
