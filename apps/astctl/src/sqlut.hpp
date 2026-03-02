#ifndef SQLUT_HPP
#define SQLUT_HPP

#include <map>
#include <memory>
#include <string>
extern "C" {
#include <sqlite3.h>
}

int get_data_callback(void *dataret, int count, char **data, char **columns);
class SQLUT {
private:
  std::map<int, std::string> tokenMap;
  int get_chars(struct sqlite3 *db_handle, std::unique_ptr<std::string> &data,
                char ch);
  void populateTokenMap(std::string &input);

public:
  SQLUT(std::string &input);
  std::map<int, std::string> getMap();
};

#endif
