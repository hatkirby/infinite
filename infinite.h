#ifndef INFINITE_H_A2B995C7
#define INFINITE_H_A2B995C7

#include <random>
#include <string>
#include <twitter.h>
#include <verbly.h>
#include <memory>
#include <Magick++.h>
#include "sentence.h"

class infinite {
public:

  infinite(
    std::string configFile,
    std::mt19937& rng);

  void run() const;

private:

  std::mt19937& rng_;
  std::unique_ptr<verbly::database> database_;
  std::unique_ptr<sentence> generator_;
  std::unique_ptr<twitter::client> client_;
  std::string groups_;
  std::vector<std::string> fonts_;
};

#endif /* end of include guard: INFINITE_H_A2B995C7 */
