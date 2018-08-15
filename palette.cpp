#include "palette.h"

palette::palette(std::string colors)
{
  for (int i=0; i<256; i++)
  {
    std::string hexstr = colors.substr(i*6, 6);
    gradient_.push_back(Color::fromHex(hexstr.c_str()));
  }
}

Color palette::getShade(double c) const
{
  size_t sc = std::min(static_cast<size_t>(floor(c * 256.0)), 255);

  return gradient_.at(sc);
}
