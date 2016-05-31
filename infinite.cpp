#include "fractal.h"
#include <Magick++.h>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <verbly.h>
#include <fstream>
#include <dirent.h>
#include <yaml-cpp/yaml.h>
#include <unistd.h>
#include <twitter.h>
#include <algorithm>

class fill_blanks {
  private:
    verbly::data& database;
    
  public:
    fill_blanks(verbly::data& database) : database(database)
    {
      
    }
    
    verbly::filter<verbly::noun> parse_selrestrs(verbly::frame::selrestr selrestr)
    {
      switch (selrestr.get_type())
      {
        case verbly::frame::selrestr::type::empty:
        {
          return verbly::filter<verbly::noun>{};
        }
        
        case verbly::frame::selrestr::type::singleton:
        {
          verbly::noun n;
          
          if (selrestr.get_restriction() == "concrete")
          {
            n = database.nouns().with_singular_form("physical entity").limit(1).run().front();
          } else if (selrestr.get_restriction() == "time")
          {
            n = database.nouns().with_singular_form("time").limit(1).run().front();
          } else if (selrestr.get_restriction() == "state")
          {
            n = database.nouns().with_singular_form("state").limit(1).run().front();
          } else if (selrestr.get_restriction() == "abstract")
          {
            n = database.nouns().with_singular_form("abstract entity").limit(1).run().front();
          } else if (selrestr.get_restriction() == "time")
          {
            n = database.nouns().with_singular_form("time").limit(1).run().front();
          } else if (selrestr.get_restriction() == "scalar")
          {
            n = database.nouns().with_singular_form("number").limit(1).run().front();
          } else if (selrestr.get_restriction() == "currency")
          {
            auto nn2 = database.nouns().with_singular_form("currency").limit(2).run();
            std::vector<verbly::noun> nn(std::begin(nn2), std::end(nn2));
            n = nn[1];
          } else if (selrestr.get_restriction() == "location")
          {
            n = database.nouns().with_singular_form("location").limit(1).run().front();
          } else if (selrestr.get_restriction() == "organization")
          {
            n = database.nouns().with_singular_form("organization").limit(1).run().front();
          } else if (selrestr.get_restriction() == "int_control")
          {
            n = database.nouns().with_singular_form("causal agent").limit(1).run().front();
          } else if (selrestr.get_restriction() == "natural")
          {
            n = database.nouns().with_singular_form("natural object").limit(1).run().front();
          } else if (selrestr.get_restriction() == "phys_obj")
          {
            n = database.nouns().with_singular_form("physical object").limit(1).run().front();
          } else if (selrestr.get_restriction() == "solid")
          {
            n = database.nouns().with_singular_form("solid").limit(1).run().front();
          } else if (selrestr.get_restriction() == "shape")
          {
            n = database.nouns().with_singular_form("shape").limit(1).run().front();
          } else if (selrestr.get_restriction() == "substance")
          {
            n = database.nouns().with_singular_form("substance").limit(1).run().front();
          } else if (selrestr.get_restriction() == "idea")
          {
            n = database.nouns().with_singular_form("idea").limit(1).run().front();
          } else if (selrestr.get_restriction() == "sound")
          {
            auto nn2 = database.nouns().with_singular_form("sound").limit(4).run();
            std::vector<verbly::noun> nn(std::begin(nn2), std::end(nn2));
            n = nn[3];
          } else if (selrestr.get_restriction() == "communication")
          {
            n = database.nouns().with_singular_form("communication").limit(1).run().front();
          } else if (selrestr.get_restriction() == "region")
          {
            n = database.nouns().with_singular_form("region").limit(1).run().front();
          } else if (selrestr.get_restriction() == "place")
          {
            n = database.nouns().with_singular_form("place").limit(1).run().front();
          } else if (selrestr.get_restriction() == "machine")
          {
            n = database.nouns().with_singular_form("machine").limit(1).run().front();
          } else if (selrestr.get_restriction() == "animate")
          {
            n = database.nouns().with_singular_form("animate being").limit(1).run().front();
          } else if (selrestr.get_restriction() == "plant")
          {
            auto nn2 = database.nouns().with_singular_form("plant").limit(2).run();
            std::vector<verbly::noun> nn(std::begin(nn2), std::end(nn2));
            n = nn[1];
          } else if (selrestr.get_restriction() == "comestible")
          {
            n = database.nouns().with_singular_form("food").limit(1).run().front();
          } else if (selrestr.get_restriction() == "artifact")
          {
            n = database.nouns().with_singular_form("artifact").limit(1).run().front();
          } else if (selrestr.get_restriction() == "vehicle")
          {
            n = database.nouns().with_singular_form("vehicle").limit(1).run().front();
          } else if (selrestr.get_restriction() == "human")
          {
            n = database.nouns().with_singular_form("person").limit(1).run().front();
          } else if (selrestr.get_restriction() == "animal")
          {
            n = database.nouns().with_singular_form("animal").limit(1).run().front();
          } else if (selrestr.get_restriction() == "body_part")
          {
            n = database.nouns().with_singular_form("body part").limit(1).run().front();
          } else if (selrestr.get_restriction() == "garment")
          {
            n = database.nouns().with_singular_form("clothing").limit(1).run().front();
          } else if (selrestr.get_restriction() == "tool")
          {
            n = database.nouns().with_singular_form("tool").limit(1).run().front();
          } else {
            return verbly::filter<verbly::noun>{};
          }
          
          return verbly::filter<verbly::noun>{n, !selrestr.get_pos()};
        }
        
        case verbly::frame::selrestr::type::group:
        {
          verbly::filter<verbly::noun> ret;
          ret.set_orlogic(selrestr.get_orlogic());

          std::transform(std::begin(selrestr), std::end(selrestr), std::back_inserter(ret), [&] (verbly::frame::selrestr sr) {
            return parse_selrestrs(sr);
          });
          
          return ret;
        }
      }
    }
    
    void visit(verbly::token& it)
    {
      switch (it.get_type())
      {
        case verbly::token::type::utterance:
        {
          for (auto& tkn : it)
          {
            if (!tkn.is_complete())
            {
              visit(tkn);
              
              break;
            }
          }
          
          break;
        }
        
        case verbly::token::type::fillin:
        {
          switch (it.get_fillin_type())
          {
            case verbly::token::fillin_type::participle_phrase:
            { 
              for (;;)
              {
                verbly::verb v = database.verbs().has_frames().random().limit(1).run().front();
                auto frames = v.frames().run();
                std::vector<verbly::frame> filtered;
                std::remove_copy_if(std::begin(frames), std::end(frames), std::back_inserter(filtered), [] (verbly::frame& f) {
                  if (f.parts().size() < 2)
                  {
                    return true;
                  }
                
                  if (f.parts()[0].get_type() != verbly::frame::part::type::noun_phrase)
                  {
                    return true;
                  }
                
                  if (f.parts()[0].get_role() != "Agent")
                  {
                    return true;
                  }
                
                  if (f.parts()[1].get_type() != verbly::frame::part::type::verb)
                  {
                    return true;
                  }
                
                  return false;
                });
                
                if (filtered.empty())
                {
                  continue;
                }
                
                verbly::frame fr = filtered[rand() % filtered.size()];
                verbly::token utter;
                for (auto part : fr.parts())
                {
                  switch (part.get_type())
                  {
                    case verbly::frame::part::type::noun_phrase:
                    {
                      if (part.get_role() == "Agent")
                      {
                        continue;
                      }
                      
                      if (part.get_synrestrs().count("adjp") == 1)
                      {
                        utter << verbly::token{verbly::token::fillin_type::adjective_phrase};
                        
                        continue;
                      } else if ((part.get_synrestrs().count("be_sc_ing") == 1)
                        || (part.get_synrestrs().count("ac_ing") == 1)
                        || (part.get_synrestrs().count("sc_ing") == 1)
                        || (part.get_synrestrs().count("np_omit_ing") == 1)
                        || (part.get_synrestrs().count("oc_ing") == 1))
                      {
                        utter << verbly::token{verbly::token::fillin_type::participle_phrase};
                        
                        continue;
                      } else if ((part.get_synrestrs().count("poss_ing") == 1)
                        || (part.get_synrestrs().count("possing") == 1)
                        || (part.get_synrestrs().count("pos_ing") == 1))
                      {
                        utter << verbly::token{"their"};
                        utter << verbly::token{verbly::token::fillin_type::participle_phrase};
                        
                        continue;
                      } else if (part.get_synrestrs().count("genitive") == 1)
                      {
                        utter << verbly::token{"their"};
                        
                        continue;
                      } else if (part.get_synrestrs().count("adv_loc") == 1)
                      {
                        if (rand() % 2 == 0)
                        {
                          utter << verbly::token{"here"};
                        } else {
                          utter << verbly::token{"there"};
                        }
                        
                        continue;
                      } else if (part.get_synrestrs().count("refl") == 1)
                      {
                        utter << verbly::token{"themselves"};
                        
                        continue;
                      } else if ((part.get_synrestrs().count("sc_to_inf") == 1)
                        || (part.get_synrestrs().count("ac_to_inf") == 1)
                        || (part.get_synrestrs().count("vc_to_inf") == 1)
                        || (part.get_synrestrs().count("rs_to_inf") == 1)
                        || (part.get_synrestrs().count("oc_to_inf") == 1))
                      {
                        utter << verbly::token{verbly::token::fillin_type::infinitive_phrase};
                        
                        continue;
                      } else if (part.get_synrestrs().count("oc_bare_inf") == 1)
                      {
                        verbly::token tkn{verbly::token::fillin_type::infinitive_phrase};
                        tkn.set_extra(1);
                        
                        utter << tkn;
                        
                        continue;
                      }
                      
                      auto selrestrs = fr.roles()[part.get_role()];
                      auto query = database.nouns().limit(1).random().is_not_proper().full_hyponym_of(parse_selrestrs(selrestrs));
                      verbly::noun n = query.run().front();
                      if ((rand() % 2 == 0) && (part.get_synrestrs().count("definite") == 0))
                      {
                        utter << verbly::token{"the"};
                      } else {
                        if (n.starts_with_vowel_sound())
                        {
                          utter << verbly::token{"an"};
                        } else {
                          utter << verbly::token{"a"};
                        }
                      }
                      
                      if (part.get_synrestrs().count("plural") == 1)
                      {
                        utter << verbly::token{n, verbly::token::noun_inflection::plural};
                      } else {
                        utter << verbly::token{n};
                      }
                      
                      if (part.get_synrestrs().count("acc_ing") == 1)
                      {
                        utter << verbly::token{verbly::token::fillin_type::participle_phrase};
                      }
                    
                      break;
                    }
                  
                    case verbly::frame::part::type::verb:
                    {
                      utter << verbly::token{v, verbly::token::verb_inflection::ing_form};
                    
                      break;
                    }
                  
                    case verbly::frame::part::type::literal_preposition:
                    {
                      utter << verbly::token{part.get_choices()[rand() % part.get_choices().size()]};
                    
                      break;
                    }
                    
                    case verbly::frame::part::type::selection_preposition:
                    {
                      auto query = database.prepositions();
                      for (auto preprestr : part.get_preprestrs())
                      {
                        query.in_group(preprestr);
                      }
                      utter << verbly::token{query.random().limit(1).run().front()};
                      
                      break;
                    }
                  
                    case verbly::frame::part::type::adjective:
                    {
                      utter << verbly::token{verbly::token::fillin_type::adjective_phrase};
                    
                      break;
                    }
                  
                    case verbly::frame::part::type::adverb:
                    {
                      utter << verbly::token{verbly::token::fillin_type::adverb_phrase};
                    
                      break;
                    }
                  
                    case verbly::frame::part::type::literal:
                    {
                      utter << verbly::token{part.get_literal()};
                    
                      break;
                    }
                  }
                }
              
                it = utter;
  
                break;
              }
              
              break;
            }
            
            case verbly::token::fillin_type::adjective_phrase:
            {
              verbly::token phrase;
              
              if (rand() % 4 == 0)
              {
                phrase << verbly::token{verbly::token::fillin_type::adverb_phrase};
              }
              
              if (rand() % 2 == 0)
              {
                phrase << verbly::token{verbly::token::fillin_type::participle_phrase};
              } else {
                phrase << verbly::token{database.adjectives().random().limit(1).run().front()};
              }
              
              it = phrase;
              
              break;
            }
            
            case verbly::token::fillin_type::adverb_phrase:
            {
              it = verbly::token{database.adverbs().random().limit(1).run().front()};
              
              break;
            }
            
            case verbly::token::fillin_type::infinitive_phrase:
            {
              verbly::token utter;
              if (it.get_extra() != 1)
              {
                utter << verbly::token{"to"};
              }
              
              utter << verbly::token{database.verbs().random().limit(1).run().front()};
              
              it = utter;
              
              break;
            }

            default:
            {
              it = verbly::token{"*the reality of the situation*"};
  
              break;
            }
          }
          
          break;
        }
      }
    }
};

int main(int argc, char** argv)
{
  srand(time(NULL));
  rand(); rand(); rand(); rand();
  
  Magick::InitializeMagick(nullptr);
  
  int delay = 60 * 60;
  
  YAML::Node config = YAML::LoadFile("config.yml");
  twitter::auth auth;
  auth.setConsumerKey(config["consumer_key"].as<std::string>());
  auth.setConsumerSecret(config["consumer_secret"].as<std::string>());
  auth.setAccessKey(config["access_key"].as<std::string>());
  auth.setAccessSecret(config["access_secret"].as<std::string>());
  
  twitter::client client(auth);
  
  for (;;)
  {
    std::cout << "Generating text..." << std::endl;
    
	std::map<std::string, std::vector<std::string>> groups;
    std::ifstream datafile("forms.txt");
    if (!datafile.is_open())
    {
      std::cout << "Could not find forms.txt" << std::endl;
      return 1;
    }
	
    bool newgroup = true;
    std::string line;
    std::string curgroup;
    while (getline(datafile, line))
    {
      if (line.back() == '\r')
      {
        line.pop_back();
      }
    
      if (newgroup)
      {
        curgroup = line;
        newgroup = false;
      } else {
        if (line.empty())
        {
          newgroup = true;
        } else {
          groups[curgroup].push_back(line);
        }
      }
    }
  
    datafile.close();
  
    verbly::data database {"data.sqlite3"};
    std::string action = "{FORM}";
    int tknloc;
    while ((tknloc = action.find("{")) != std::string::npos)
    {
      std::string token = action.substr(tknloc+1, action.find("}")-tknloc-1);
    
      std::string canontkn;
      std::transform(std::begin(token), std::end(token), std::back_inserter(canontkn), [] (char ch) {
        return std::toupper(ch);
      });
    
      std::string result;
      if (canontkn == "NOUN")
      {
        result = database.nouns().is_not_proper().random().limit(1).with_complexity(1).run().front().singular_form();
//      } else if (canontkn == "PLURAL_NOUN")
//      {
//        result = database.nouns().is_not_proper().requires_plural_form().random().limit(1).with_complexity(1).run().front().plural_form();
//      } else if (canontkn == "ADJECTIVE")
      // {
        // result = database.adjectives().with_complexity(1).random().limit(1).run().front().base_form();
      } else if (canontkn == "SUPERLATIVE")
      {
        result = database.adjectives().requires_superlative_form().random().limit(1).run().front().superlative_form();
      } else if (canontkn == "VERB")
      {
        result = database.verbs().random().limit(1).run().front().infinitive_form();
      } else if (canontkn == "VERBED")
      {
        result = database.verbs().random().limit(1).run().front().past_tense_form();
      } else if (canontkn == "VERBS")
      {
        result = database.verbs().random().limit(1).run().front().s_form();
      } else if (canontkn == "VERBING")
      {
        result = database.verbs().random().limit(1).run().front().ing_form();
      } else if (canontkn == "LOCATION")
      {
        auto hem1 = database.nouns().with_singular_form("eastern hemisphere").limit(1).run().front();
        auto hem2 = database.nouns().with_singular_form("western hemisphere").limit(1).run().front();
        verbly::filter<verbly::noun> region{hem1, hem2};
        region.set_orlogic(true);
      
        result = database.nouns().full_part_holonym_of(region).random().limit(1).run().front().singular_form();
      } else if (canontkn == "LANGUAGE")
      {
        auto natlan = database.nouns().with_singular_form("natural language").limit(1).run().front();
        result = database.nouns().full_hyponym_of(natlan).is_proper().random().limit(1).run().front().singular_form();
      } else if (canontkn == "ADVERB")
      {
        result = database.adverbs().with_complexity(1).random().limit(1).run().front().base_form();
      } else if (canontkn == "VERBLY_SENTENCE")
      {
        fill_blanks yeah {database};
        verbly::token action{
          {"I am"},
          {verbly::token::fillin_type::adjective_phrase}
        };
        while (!action.is_complete())
        {
          yeah.visit(action);
        }
        result = action.compile();
      } else {
		    auto group = groups[canontkn];
        result = group[rand() % group.size()];
        std::cout << canontkn << ": " << group.size() << std::endl;
      }
    
      std::string finalresult;
      if (islower(token[0]))
      {
        std::transform(std::begin(result), std::end(result), std::back_inserter(finalresult), [] (char ch) {
          return std::tolower(ch);
        });
      } else if (isupper(token[0]) && !isupper(token[1]))
      {
        auto words = verbly::split<std::list<std::string>>(result, " ");
        for (auto& word : words)
        {
          word[0] = std::toupper(word[0]);
        }
      
        finalresult = verbly::implode(std::begin(words), std::end(words), " ");
      } else {
        finalresult = result;
      }
    
      action.replace(tknloc, action.find("}")-tknloc+1, finalresult);
    }
    
    std::cout << action << std::endl;
  
    double zoom = 2.0;
    double target_w = 1280;
    double target_h = 800;
    double sample_rate = 3;
    
    std::cout << "Generating flame fractal..." << std::endl;
  
    Magick::Image image(Magick::Geometry(target_w, target_h), "black");
    image.type(Magick::TrueColorMatteType);
  
    for (;;)
    {
      Fractal fractal = Fractal::random();
      std::vector<Color> irradiance(target_w*target_h*sample_rate*sample_rate, Color(0.0, 0.0, 0.0, 0.0));

      double x = (double)rand()/(double)RAND_MAX*2.0-1.0;
      double y = (double)rand()/(double)RAND_MAX*2.0-1.0;  
      double c = (double)rand()/RAND_MAX;
  
      double widthmul = fractal.width/target_w*zoom;
      double heightmul = fractal.height/target_w*zoom;
      double widthmul2 = widthmul * 2.0;
      double heightmul2 = heightmul * 2.0;
  
      int maxrad = 0;
      for (int i=0; i<50000000; i++)
      {
        fractal.sample(x, y, c);
    
        int fx = (x+widthmul)/widthmul2*target_w*sample_rate;
        int fy = (y+heightmul)/heightmul2*target_h*sample_rate;

        if (fx >= 0 && fx < target_w*sample_rate && fy >= 0 && fy < target_h*sample_rate && i > 20)
        {
          auto& irr = irradiance[fx+fy*target_w*sample_rate];
          irr += fractal.get_color(c);
          double alph = irr.a;
          irr *= 0.5;
          irr.a = alph;
      
          if (irr.a > maxrad)
          {
            maxrad = irr.a;
          }
        }
      }
  
      image.modifyImage();
      Magick::Pixels view(image);
      Magick::PixelPacket* pixels = view.get(0, 0, target_w, target_h);

      double white = 200.0;
      double quality = sample_rate*sample_rate;
      double vibrancy = 1.0;
      double contrast = 1.0;
      double brightadjust = 2.3;
      double k1 = contrast * (268.0 * brightadjust) * 100.0 / fractal.brightness / (256.0*256.0);
      double k2 = 1.0 / (contrast * white * quality);
      int covered = 0;
      for (int j=0; j<target_h; j++)
      {
        for (int i=0; i<target_w; i++)
        {
          double freq_avg = 0.0;
          double freq_max = 0.0;
          Color color_avg(0.0,0.0,0.0,0.0);
          for (int sy = 0; sy<sample_rate; sy++)
          {
            for (int sx = 0; sx<sample_rate; sx++)
            {
              auto& fc = irradiance[i*sample_rate+sx+(j*sample_rate+sy)*target_w*sample_rate];
              freq_avg += fc.a;
              color_avg += fc;
              if (fc.a > freq_max)
              {
                freq_max = fc.a;
              }
            }
          }
      
          if (freq_max > 0)
          {
            covered++;
          }
      
          freq_avg *= (white * fractal.filterlevel)/(sample_rate * sample_rate);
          color_avg *= (fractal.filterlevel/(sample_rate*sample_rate));
      
          double alph = k1 * std::log(1+white*freq_avg*k2)/(std::log(10*white*freq_max));
          double alphg = pow(alph, 1.0/fractal.gamma);
          if (alph <= fractal.gammathresh)
          {
            double frac = alph / fractal.gammathresh;
            alphg = (1-frac)*alph*pow(fractal.gammathresh, 1.0/fractal.gamma - 1.0) + frac*alphg;
          }
      
          double ls = vibrancy*alphg/alph;
          Color finc = color_avg * ls;
          finc.a = 1.0;
      
          if (finc.r > 1.0)
          {
            finc *= (1.0 / finc.r);
          }
      
          if (finc.g > 1.0)
          {
            finc *= (1.0 / finc.g);
          }
      
          if (finc.b > 1.0)
          {
            finc *= (1.0 / finc.b);
          }
      
          *pixels++ = Magick::ColorRGB(finc.r, finc.g, finc.b);
        }
      }
  
      double coverage = ((double)covered/(double)(target_w*target_h));
      std::cout << coverage << " coverage" << std::endl;
  
      view.sync();
    
      if (coverage >= 0.05)
      {
        break;
      }
    
      std::cout << "Regenerating..." << std::endl;
    }
  
    DIR* fontdir;
    struct dirent* ent;
    if ((fontdir = opendir("fonts")) == nullptr)
    {
      std::cout << "Couldn't find fonts." << std::endl;
      return -1;
    }
  
    std::vector<std::string> fonts;
    while ((ent = readdir(fontdir)) != nullptr)
    {
      std::string dname(ent->d_name);
      if ((dname.find(".otf") != std::string::npos) || (dname.find(".ttf") != std::string::npos))
      {
        fonts.push_back(dname);
      }
    }
  
    closedir(fontdir);
    
    std::string subaction = action;
    std::string font = fonts[rand() % fonts.size()];
    if (font == "Le_Super_Type_SemiBold.ttf")
    {
      std::transform(std::begin(subaction), std::end(subaction), std::begin(subaction), [] (char ch) {
        return std::toupper(ch);
      });
    }
  
    Magick::Image textimage(Magick::Geometry(target_w, target_h), "transparent");
    textimage.type(Magick::TrueColorMatteType);
    textimage.fillColor(Magick::Color("white"));
    textimage.fontPointsize(72.0);
    textimage.font("fonts/" + font);
  
    auto words = verbly::split<std::list<std::string>>(subaction, " ");
    std::string towrite = "";
    std::string curline = "";
    Magick::TypeMetric metric;
    while (!words.empty())
    {
      std::string temp = curline + " " + words.front();
      textimage.fontTypeMetrics(temp, &metric);
      if (metric.textWidth() > ((target_w/10)*9))
      {
        towrite += "\n" + words.front();
        curline = words.front();
      } else {
        towrite += " " + words.front();
        curline = temp;
      }
    
      words.pop_front();
    }
  
    textimage.annotate(towrite, Magick::CenterGravity);
    textimage.opacity(((double)MaxRGB) * 0.8);
    image.composite(textimage, 0, 0, Magick::OverCompositeOp);
  
    image.magick("jpg");
    
    Magick::Blob outputimg;
    image.write(&outputimg);
    
    std::cout << "Generated image!" << std::endl << "Tweeting..." << std::endl;
    
    long media_id;
    twitter::response resp = client.uploadMedia("image/jpeg", (const char*) outputimg.data(), outputimg.length(), media_id);
    if (resp != twitter::response::ok)
    {
      std::cout << "Twitter error while uploading image: " << resp << std::endl;
      sleep(delay);
      
      continue;
    }
    
    std::string tweetText;
    size_t tweetLim = 140 - client.getConfiguration().getCharactersReservedPerMedia() - client.getUser().getScreenName().length() - 5;
    if (action.length() > tweetLim)
    {
      tweetText = "\"" + action.substr(0, tweetLim - 1) + "…\" --@" + client.getUser().getScreenName();
    } else {
      tweetText = "\"" + action + "\" --@" + client.getUser().getScreenName();
    }
    
    twitter::tweet tw;
    resp = client.updateStatus(tweetText, tw, twitter::tweet(), {media_id});
    if (resp != twitter::response::ok)
    {
      std::cout << "Twitter error while tweeting: " << resp << std::endl;
      sleep(delay);
      
      continue;
    }
    
    std::cout << "Done!" << std::endl << "Waiting..." << std::endl << std::endl;
    
    sleep(delay);
  }
}
