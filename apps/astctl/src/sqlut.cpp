#include "sqlut.hpp"
#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string>
extern "C" {
#include <parser.h>
#include <sqlite3.h>
}

int get_data_callback(void *dataret, int count, char **data, char **columns) {
  std::string &d = *static_cast<std::string *>(dataret);
  if (data[0]) {
    d.append(data[0]);
  }
  return 0;
}

SQLUT::SQLUT(std::string &input) { populateTokenMap(input); }

int SQLUT::get_chars(struct sqlite3 *db_handle,
                     std::unique_ptr<std::string> &data, char ch) {

  data = std::make_unique<std::string>();
  void *data_handle = static_cast<void *>(data.get());
  char *errmsg_cstr = nullptr;
  std::string handle;

  char query[50];
  snprintf(query, sizeof(query), "SELECT data FROM FCLOOKUP_NEW WHERE id == %d",
           int(ch));
  int return_status = sqlite3_exec(db_handle, query, get_data_callback,
                                   data_handle, &errmsg_cstr);
  if (return_status != SQLITE_OK) {
    sqlite3_free(errmsg_cstr);
  }

  return return_status;
}

void SQLUT::populateTokenMap(std::string &input) {
  tokenMap.clear();
  std::map<int, std::string> bum;

  input.erase(std::remove_if(input.begin(), input.end(), ::isspace),
              input.end());
  std::set<char> chtokens(input.begin(), input.end());

  struct sqlite3 *db_handle;
  std::unique_ptr<std::string> data;
  sqlite3_open("fontdch.db", &db_handle);

  for (char chtok : chtokens) {
    get_chars(db_handle, data, chtok);
    tokenMap[int(chtok)] = *data;
  }

  sqlite3_close(db_handle);
}

std::map<int, std::string> SQLUT::getMap() { return tokenMap; }
