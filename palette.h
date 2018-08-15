#ifndef PALETTE_H_09303AB4
#define PALETTE_H_09303AB4

#include <string>
#include <vector>
#include "color.h"

class palette {
public:

  explicit palette(std::string colors);

  Color getShade(double c) const;

private:

  std::vector<Color> gradient_;
};

#endif /* end of include guard: PALETTE_H_09303AB4 */
