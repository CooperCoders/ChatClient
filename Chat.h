#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <winsock2.h>

extern SDL_Surface* screen;

bool init(int screen_height, int screen_width, int screen_bpp)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    return true;

  screen = SDL_SetVideoMode(screen_width, screen_height, screen_bpp, SDL_SWSURFACE|SDL_DOUBLEBUF);

  if(screen == NULL)
  {
    return true;
  }

  if(TTF_Init() == -1 )
    return true;



  SDL_WM_SetCaption("CIM (Cooper Instant Messager)", NULL);

  return false;
}

void draw_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
  //Holds offsets
  SDL_Rect offset;
  //Get offsets
  offset.x = x;
  offset.y = y;

  //Blit
  SDL_BlitSurface(source, clip, destination, &offset);
}

int winsock_init(WSADATA* wsaData)
{
  return WSAStartup(MAKEWORD(2, 0), wsaData) != 0;
}

