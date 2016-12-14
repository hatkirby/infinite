#ifndef FRACTAL_H_0585D362
#define FRACTAL_H_0585D362

#include <map>
#include <string>
#include <vector>
#include <list>
#include "matrix3x3.h"
#include "color.h"
  
class Variation {
  public:
    enum class Type {
      linear,
      sinusoidal,
      spherical,
      eyefish,
      bubble,
      cylinder,
      noise,
      blur,
      horseshoe,
      swirl,
      julian,
      hyperbolic,
      polar,
      handkerchief,
      heart,
      disc,
      spiral,
      diamond,
      ex,
      julia,
      bent,
      fisheye
    };
    
    Variation(Type _type, double _weight, std::vector<double> _params = {});
    
    const Type type;
    const double weight;
    const std::vector<double> params;
};

class Fractal {
  public:
    void set_palette(std::string colors);
    void add_transform(double weight, Matrix3x3 transform, double color, std::vector<Variation> variations);
    void sample(double& x, double& y, double& c) const;
    Color get_color(double c) const;
    
    static int load(const char* filename, Fractal& fractal);
    static Fractal random(std::string colorsfile);
    
    double filterlevel = 0.5;
    double gamma = 2.2;
    double gammathresh = 0.01;
    double brightness = 4.0;
    double width = 800.0;
    double height = 640.0;
    
  private:
    friend class FractalParser;
    
    struct Transform {
      Matrix3x3 transform;
      double color;
      std::vector<Variation> variations;
      
      Transform()
      {
        
      }
      
      Transform(Matrix3x3 transform, double color, std::vector<Variation> variations) : transform(transform), color(color), variations(variations)
      {
        
      }
    };
    
    std::map<double, Transform> transforms;
    double max = 0.0;
    std::vector<Color> palette;
};

class LogScale {
  public:
    LogScale(double brightness, double quality);
    double log(double n) const;
    
  private:
    std::vector<double> memo;
    double k1;
    double k2;
};

#endif /* end of include guard: FRACTAL_H_0585D362 */
