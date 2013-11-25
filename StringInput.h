#ifndef STRINGINPUT_H
#define STRINGINPUT_H
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
//#include "SDL_init.h"
#include <string>


class StringInput
{
  private:
  std::string str_;
  SDL_Surface *text_;
  SDL_Color textColor_;
  TTF_Font *font_;
  int x_, y_;
  public:
  StringInput(int x, int y, SDL_Color color, TTF_Font *font);
  ~StringInput();
  void handle_input(SDL_Event event);
  void show(SDL_Surface* screen);
  std::string getString();
  void clear();
};

#endif // STRINGINPUT_H
