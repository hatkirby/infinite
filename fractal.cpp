#include "tinyxml2.h"
#include "fractal.h"
#include <iostream>
#include <fstream>
#include "triangle.h"

Variation::Variation(Type _type, double _weight, std::vector<double> _params) : type(_type), weight(_weight), params(_params)
{
  
}

void Fractal::set_palette(std::string colors)
{
  for (int i=0; i<256; i++)
  {
    std::string hexstr = colors.substr(i*6, 6);
    palette.push_back(Color::fromHex(hexstr.c_str()));
  }
}

void Fractal::add_transform(double weight, Matrix3x3 transform, double color, std::vector<Variation> variations)
{
  max += weight;
  transforms[max] = Transform(transform, color, variations);
}

void Fractal::sample(double& x, double& y, double& c) const
{
  double rtran = (double)rand()/RAND_MAX*max;
  const Transform& transform = transforms.upper_bound(rtran)->second;
  Vector3D in = transform.transform * Vector3D(x, y, 1.0);
  double r2 = pow(in.x,2.0)+pow(in.y,2.0);
  double r = sqrt(r2);
  
  double tx, ty;
  x = 0.0;
  y = 0.0;
  for (auto& variation : transform.variations)
  {
    switch (variation.type)
    {
      case Variation::Type::linear:
      {
        tx = in.x;
        ty = in.y;
        
        break;
      }

      case Variation::Type::sinusoidal:
      {
        tx = sin(in.x);
        ty = sin(in.y);
        
        break;  
      }
      
      case Variation::Type::spherical:
      {
        tx = in.x/r2;
        ty = in.y/r2;
        
        break;
      }
      
      case Variation::Type::eyefish:
      {
        double xp = 2.0/(r+1.0);
        tx = in.x*xp;
        ty = in.y*xp;
        
        break;
      }
      
      case Variation::Type::fisheye:
      {
        double xp = 2.0/(r+1.0);
        tx = in.y*xp;
        ty = in.x*xp;
        
        break;
      }
      
      case Variation::Type::bubble:
      {
        double xp = 4.0/(r2+4);
        tx = in.x*xp;
        ty = in.y*xp;
        
        break;
      }
      
      case Variation::Type::cylinder:
      {
        tx = sin(in.x);
        ty = in.y;
        
        break;
      }

      case Variation::Type::noise:
      {
        double phi1 = (double)rand()/RAND_MAX;
        double phi2 = (double)rand()/RAND_MAX;
        tx = in.x * phi1 * cos(2*M_PI*phi2);
        ty = in.y * phi1 * sin(2*M_PI*phi2);
        
        break;
      }
      
      case Variation::Type::blur:
      {
        double phi1 = (double)rand()/RAND_MAX;
        double phi2 = (double)rand()/RAND_MAX;
        tx = phi1 * cos(2*M_PI*phi2);
        ty = phi1 * sin(2*M_PI*phi2);
        
        break;
      }
      
      case Variation::Type::horseshoe:
      {
        tx = (1.0/r)*((in.x-in.y)*(in.x+in.y));
        ty = 2*in.x*in.y/r;
        
        break;
      }
      
      case Variation::Type::swirl:
      {
        tx = in.x*sin(r2) - in.y*cos(r2);
        ty = in.x*cos(r2) + in.y*sin(r2);
        
        break;
      }
      
      case Variation::Type::julian:
      {
        double p1 = variation.params[0];
        double p2 = variation.params[1];
        double p3 = (double)(rand()%(int)floor(std::abs(p1)));
        double t = (atan2(in.y, in.x) + 2*M_PI*p3) / p1;
        double pw = pow(r2, p2 / p1 / 2.0);
        tx = pw * cos(t);
        ty = pw * sin(t);
        
        break;
      }
      
      case Variation::Type::hyperbolic:
      {
        double theta = atan2(in.x, in.y);
        tx = sin(theta)/r;
        ty = r*cos(theta);
        
        break;
      }
      
      case Variation::Type::polar:
      {
        double theta = atan2(in.x, in.y);
        tx = theta/M_PI;
        ty = r - 1;
        
        break;
      }
      
      case Variation::Type::handkerchief:
      {
        double theta = atan2(in.x, in.y);
        tx = r * sin(theta + r);
        ty = r * cos(theta - r);
        
        break;
      }
      
      case Variation::Type::heart:
      {
        double theta = atan2(in.x, in.y);
        tx = r * sin(theta * r);
        ty = -r * cos(theta * r);
        
        break;
      }
      
      case Variation::Type::disc:
      {
        double theta = atan2(in.x, in.y);
        tx = theta/M_PI*sin(r*M_PI);
        ty = theta/M_PI*cos(r*M_PI);
        
        break;
      }
      
      case Variation::Type::spiral:
      {
        double theta = atan2(in.x, in.y);
        tx = 1.0/r * (cos(theta) + sin(r));
        ty = 1.0/r * (sin(theta) - cos(r));
        
        break;
      }
      
      case Variation::Type::diamond:
      {
        double theta = atan2(in.x, in.y);
        tx = sin(theta)*cos(r);
        ty = cos(theta)*sin(r);
        
        break;
      }
      
      case Variation::Type::ex:
      {
        double theta = atan2(in.x, in.y);
        double p0 = pow(sin(theta + r), 3.0);
        double p1 = pow(cos(theta - r), 3.0);
        tx = r * (p0 + p1);
        ty = r * (p0 - p1);
        
        break;
      }
      
      case Variation::Type::julia:
      {
        double theta = atan2(in.x, in.y);
        double omega = (double)(rand()%2) * M_PI;
        double sr = sqrt(r);
        tx = sr * cos(theta/2.0 + omega);
        ty = sr * sin(theta/2.0 + omega);
        
        break;
      }
      
      case Variation::Type::bent:
      {
        if (in.x >= 0)
        {
          tx = in.x;
        } else {
          tx = 2*in.x;
        }
        
        if (in.y >= 0)
        {
          ty = in.y;
        } else {
          ty = in.y/2.0;
        }
        
        break;
      }
    }
    
    x += tx * variation.weight;
    y += ty * variation.weight;
  }

  c = (c + transform.color) * (1.0/2.0);
}

Color Fractal::get_color(double c) const
{
  int sc = std::min((int)floor(c * 256.0), 255);
  return palette[sc];
}

LogScale::LogScale(double brightness, double quality)
{
  double contrast = 1.0;
  double brightadjust = 2.3;
  double white = 200.0;
  k1 = contrast * (268.0 * brightadjust) * 100.0 * brightness / (256.0*256.0);
  k2 = 1.0 / (contrast * white * quality);
  
  for (int i=0; i<1024; i++)
  {
    memo.push_back(k1 * std::log(1+white*i*k2)/(std::log(10)*white*i));
  }
}

double LogScale::log(double n) const
{
  int in = (int)floor(n);
  if (in < 1024)
  {
    return memo[in];
  } else {
    double white = 200.0;
    return k1 * std::log(1+white*n*k2)/(std::log(10)*white*n);
  }
}

template <class Container>
Container split(std::string input, std::string delimiter)
{
  Container result;
  
  while (!input.empty())
  {
    int divider = input.find(delimiter);
    if (divider == std::string::npos)
    {
      result.push_back(input);
      
      input = "";
    } else {
      result.push_back(input.substr(0, divider));
      
      input = input.substr(divider+delimiter.length());
    }
  }
  
  return result;
}

int Fractal::load(const char* filename, Fractal& fractal)
{
  std::ifstream in(filename);
  if(!in.is_open())
  {
     return -1;
  }
  
  in.close();

  tinyxml2::XMLDocument doc;
  doc.LoadFile(filename);
  if(doc.Error())
  {
     doc.PrintError();
     exit(1);
  }

  tinyxml2::XMLElement* root = doc.FirstChildElement( "flame" );
  if( !root )
  {
     std::cerr << "Error: not a flame file!" << std::endl;
     exit( 1 );
  }
  
  root->QueryDoubleAttribute("filter", &fractal.filterlevel);
  root->QueryDoubleAttribute("gamma", &fractal.gamma);
  root->QueryDoubleAttribute("gamma_threshold", &fractal.gammathresh);
  root->QueryDoubleAttribute("brightness", &fractal.brightness);
  
  const char* sizestr = root->Attribute("size");
  sscanf(sizestr, "%lf %lf", &fractal.width, &fractal.height);

  tinyxml2::XMLElement* elem = root->FirstChildElement();
  while (elem)
  {
    std::string elementType (elem->Value());
    if (elementType == "xform")
    {
      double weight;
      double color;
      Matrix3x3 transform;
      elem->QueryDoubleAttribute("weight", &weight);
      elem->QueryDoubleAttribute("color", &color);
      std::string transstr(elem->Attribute("coefs"));
      auto transvals = split<std::vector<std::string>>(transstr, " ");
      transform(0,0) = std::stod(transvals[0]);
      transform(0,1) = std::stod(transvals[2]);
      transform(0,2) = std::stod(transvals[4]);
      transform(1,0) = std::stod(transvals[1]);
      transform(1,1) = std::stod(transvals[3]);
      transform(1,2) = std::stod(transvals[5]);
      transform(2,0) = 0.0;
      transform(2,1) = 0.0;
      transform(2,2) = 1.0;
      std::vector<Variation> varies;
      const char* varyval = 0;
      if ((varyval = elem->Attribute("linear")) != 0)
      {
        varies.push_back(Variation(Variation::Type::linear, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("sinusoidal")) != 0)
      {
        varies.push_back(Variation(Variation::Type::sinusoidal, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("spherical")) != 0)
      {
        varies.push_back(Variation(Variation::Type::spherical, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("eyefish")) != 0)
      {
        varies.push_back(Variation(Variation::Type::eyefish, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("bubble")) != 0)
      {
        varies.push_back(Variation(Variation::Type::bubble, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("cylinder")) != 0)
      {
        varies.push_back(Variation(Variation::Type::cylinder, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("noise")) != 0)
      {
        varies.push_back(Variation(Variation::Type::noise, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("blur")) != 0)
      {
        varies.push_back(Variation(Variation::Type::blur, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("pre_blur")) != 0)
      {
        varies.push_back(Variation(Variation::Type::blur, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("horseshoe")) != 0)
      {
        varies.push_back(Variation(Variation::Type::horseshoe, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("swirl")) != 0)
      {
        varies.push_back(Variation(Variation::Type::swirl, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("hyperbolic")) != 0)
      {
        varies.push_back(Variation(Variation::Type::hyperbolic, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("julian")) != 0)
      {
        varies.push_back(Variation(Variation::Type::julian, std::stod(std::string(varyval)), {elem->DoubleAttribute("julian_power"), elem->DoubleAttribute("julian_dist")}));
      }
      
      if ((varyval = elem->Attribute("polar")) != 0)
      {
        varies.push_back(Variation(Variation::Type::polar, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("handkerchief")) != 0)
      {
        varies.push_back(Variation(Variation::Type::handkerchief, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("heart")) != 0)
      {
        varies.push_back(Variation(Variation::Type::heart, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("disc")) != 0)
      {
        varies.push_back(Variation(Variation::Type::disc, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("spiral")) != 0)
      {
        varies.push_back(Variation(Variation::Type::spiral, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("diamond")) != 0)
      {
        varies.push_back(Variation(Variation::Type::diamond, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("ex")) != 0)
      {
        varies.push_back(Variation(Variation::Type::ex, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("julia")) != 0)
      {
        varies.push_back(Variation(Variation::Type::julia, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("bent")) != 0)
      {
        varies.push_back(Variation(Variation::Type::bent, std::stod(std::string(varyval))));
      }
      
      if ((varyval = elem->Attribute("fisheye")) != 0)
      {
        varies.push_back(Variation(Variation::Type::fisheye, std::stod(std::string(varyval))));
      }
      
      fractal.add_transform(weight, transform, color, varies);
    } else if (elementType == "palette")
    {
      std::string val(elem->GetText());
      for (int i=0; i<val.length(); i++)
      {
        if (val[i] == ' ' || val[i] == '\n')
        {
          val.erase(i, 1);
          i--;
        }
      }
      
      fractal.set_palette(val);
    }
    
    elem = elem->NextSiblingElement();
  }

  return 0;
}

Fractal Fractal::random(std::string colorsfile)
{
  Fractal fractal;
  
  int xforms = rand() % 2 + 2;
  double remweight = 1.0;
  for (int i=0; i<xforms; i++)
  {
    double weight;
    if (i == (xforms-1))
    {
      weight = remweight;
    } else {
      weight = ((double)rand()/RAND_MAX)*remweight;
      remweight -= weight;
    }
    
    Triangle gentri(
      ((double)rand()/RAND_MAX)*2-1,
      ((double)rand()/RAND_MAX)*2-1,
      ((double)rand()/RAND_MAX)*2-1,
      ((double)rand()/RAND_MAX)*2-1,
      ((double)rand()/RAND_MAX)*2-1,
      ((double)rand()/RAND_MAX)*2-1
        );
    Matrix3x3 affine = affineTransform(Triangle(0, 0, 1, 0, 0, 1), gentri);
    double color = (double)i/(xforms-1);
    std::vector<Variation> variations;
    
    double remaffix = 1.0;
    for (int j=0; j<2; j++)
    {
      double affix;
      if (j == 1)
      {
        affix = remaffix;
      } else {
        affix = ((double)rand()/RAND_MAX)*remaffix;
        remaffix -= affix;
      }
      
      Variation::Type type;
      switch (rand()%16)
      {
        case 0: type = Variation::Type::linear; break;
        case 1: type = Variation::Type::sinusoidal; break;
        case 2: type = Variation::Type::spherical; break;
        case 3: type = Variation::Type::eyefish; break;
        case 4: type = Variation::Type::bubble; break;
        case 5: type = Variation::Type::cylinder; break;
        case 6: type = Variation::Type::blur; break;
        case 7: type = Variation::Type::horseshoe; break;
        case 8: type = Variation::Type::swirl; break;
        case 9: type = Variation::Type::hyperbolic; break;
        case 10: type = Variation::Type::polar; break;
        case 11: type = Variation::Type::handkerchief; break;
        case 12: type = Variation::Type::heart; break;
        case 13: type = Variation::Type::disc; break;
        case 14: type = Variation::Type::spiral; break;
        case 15: type = Variation::Type::diamond; break;
      }

      variations.push_back(Variation(type, affix));
    }
    
    fractal.add_transform(weight, affine, color, variations);
  }
  
  std::vector<std::string> colors;
  std::ifstream colorfile(colorsfile);
  if (!colorfile.is_open())
  {
    std::cout << "Could not find colors file" << std::endl;
    exit(-1);
  }
  
  std::string line;
  while (getline(colorfile, line))
  {
    if (line.back() == '\r')
    {
      line.pop_back();
    }
    
    colors.push_back(line);
  }
  
  colorfile.close();
  fractal.set_palette(colors[rand() % colors.size()]);
  
  return fractal;
}
