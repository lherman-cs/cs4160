#pragma once
#include <expat.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
using std::map;
using std::string;
const int BUFSIZE = 512;

class ParseXML {
 public:
  ParseXML(const string &fn)
      : filename(fn), parser(NULL), tagNames(), xmlData() {
    parseXML();
  }
  virtual ~ParseXML() { XML_ParserFree(parser); }

  const map<string, string> getXmlData() const { return xmlData; }

  static void wrapper4Start(void *data, const char *el, const char **attr);
  static void wrapper4End(void *data, const char *el);
  static void wrapper4Chars(void *data, const char *text, int textlen);

  ParseXML &operator=(const ParseXML &) = delete;
  ParseXML(const ParseXML &) = delete;

 private:
  const string filename;
  XML_Parser parser;
  char buff[BUFSIZE];

  std::deque<string> tagNames;
  std::map<string, string> xmlData;

  void parseXML();
  void displayData() const;
  void start(const char *el, const char *attr[]);
  void end(const char *);
  void chars(const char *text, int textlen);
  std::string makeTag() const;
  std::string makeTag(const std::string &) const;

  void stripTrailWhiteSpace(string &) const;
};
