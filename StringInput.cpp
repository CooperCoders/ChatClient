#include "StringInput.h"

extern void draw_surface(int, int, SDL_Surface*, SDL_Surface*, SDL_Rect* = NULL);

StringInput::StringInput(int x, int y,SDL_Color color, TTF_Font *font)
{
  str_ = "";
  text_ = NULL;
  x_ = x;
  y_ = y;
  textColor_ = color;
  font_ = font;
  SDL_EnableUNICODE(SDL_ENABLE);
}

StringInput::~StringInput()
{
  SDL_FreeSurface(text_);
  SDL_EnableUNICODE(SDL_DISABLE);
}

void StringInput::handle_input(SDL_Event event)
{
  //If a key was pressed
  if(event.type == SDL_KEYDOWN)
  {
    //Keep a copy of the current version of the string
    std::string temp = str_;
    //If the string less than maximum size
    if(str_.length() <= 55)
    {
      //If the key is a space
      if(event.key.keysym.unicode == (Uint16)' ')
      {
        //Append the character
        str_ += (char)event.key.keysym.unicode;
      }
      //If the key is a number
      else if((event.key.keysym.unicode >= (Uint16)'0') && (event.key.keysym.unicode <= (Uint16)'9'))
      {
        //Append the character
        str_ += (char)event.key.keysym.unicode;
      }
      //If the key is a uppercase letter
      else if((event.key.keysym.unicode >= (Uint16)'A') && (event.key.keysym.unicode <= (Uint16)'Z'))
      {
        //Append the character
        str_ += (char)event.key.keysym.unicode;
      }
      //If the key is a lowercase letter
      else if((event.key.keysym.unicode >= (Uint16)'a') && (event.key.keysym.unicode <= (Uint16)'z'))
      {
        //Append the character
        str_ += (char)event.key.keysym.unicode;
      }
      else if((event.key.keysym.unicode == (Uint16)'.'))
      {
        //Append the character
        str_ += (char)event.key.keysym.unicode;
      }
    }

    //If backspace was pressed and the string isn't blank
    if((event.key.keysym.sym == SDLK_BACKSPACE ) && (str_.length() != 0))
    {
      //Remove a character from the end
      str_.erase(str_.length() - 1);
    }

    //If the string was changed
    if(str_ != temp)
    {
      //Free the old surface
      SDL_FreeSurface(text_);
      //Render a new text surface
      text_ = TTF_RenderText_Solid(font_, str_.c_str(), textColor_);
    }
  }
}

void StringInput::show(SDL_Surface* screen)
{
  if(text_ != NULL)
  {
    draw_surface(x_, y_, text_, screen);
  }
}

std::string StringInput::getString()
{
  return str_;
}

void StringInput::clear()
{
  str_ = "";
  text_ = TTF_RenderText_Solid(font_, "", textColor_);
}
