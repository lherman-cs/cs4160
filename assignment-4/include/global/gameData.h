#pragma once
#include <map>
#include <string>
#include "util/parseXML.h"
#include "util/vector2f.h"

class Global;

class Gamedata {
 public:
  void displayData() const;

  bool getXmlBool(const std::string&) const;
  const std::string& getXmlStr(const std::string&) const;
  float getXmlFloat(const std::string&) const;
  int getXmlInt(const std::string&) const;
  float getRandInRange(int min, int max) const;
  float getRandFloat(float min, float max) const;
  bool checkTag(const std::string&) const;

  Gamedata(const Gamedata&) = delete;
  Gamedata& operator=(const Gamedata&) = delete;

 private:
  friend class Global;
  ParseXML parser;
  const map<std::string, std::string> gameData;

  Gamedata(const std::string& fn = "assets/config.xml");
};