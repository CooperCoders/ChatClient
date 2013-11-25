#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "Chat.h"
#include "StringInput.h"
#include "Socket.h"
#include <list>
#include <process.h>
#include <winsock2.h>
#define MESSAGE_MAX 255

using namespace std;

SDL_Surface* screen;
list<string> conversation(1, "Enter the IP of the server you would like to connect to");
list<string>::iterator it;
stringstream ss;

Socket sock;
struct sockaddr_in fromAddr;
struct sockaddr_in serverAddr;
char serverIp[1000];
char name[1000];
unsigned short serverPort = 12345;
unsigned short myPort = 12346;
bool hasMyPort = false;
bool hasServerIp = false;
bool hasServerPort = false;
bool hasName = false;
bool isConnected = false;
WSADATA wsaData;
char* message2;
char messageBuffer[MESSAGE_MAX];
char CIM[3] = {'C', 'I', 'M'};

void thread(void* arg)
{
  stringstream stream;
  while(true)
  {
    if(sock.isBound)
    {
      if(sock.recvMessage(&fromAddr, messageBuffer) < 0)
      {
        exit(WSAGetLastError());
      }
    }
    if(!memcmp(messageBuffer, CIM, 3))
      {
        stream.str(string());
        stream << messageBuffer + 3;
        conversation.push_back(stream.str());
      }
      memset(messageBuffer, '\0', MESSAGE_MAX);
  }
  _endthread();
}



int main(int argc, char* args[])
{
  bool quit = false;
  TTF_Font* font;
  SDL_Color textColor = {0, 0, 0};
  SDL_Event event;
  SDL_Surface* text;
  string message;
  int height;

  if (init(480, 640, 32))
    return 0;
  if (winsock_init(&wsaData))
    return 0;

  if(sock.bindSocket(0) < 0)
  {
    exit(WSAGetLastError());
  }
  sock.isBound = true;
  font = TTF_OpenFont("arial.ttf", 16);
  StringInput test(10, 480 - 26, textColor, font);
  text = TTF_RenderText_Solid(font, "Hello", textColor);

  _beginthread(thread, 0, NULL);

  while(quit == false)
  {
    while(SDL_PollEvent(&event))
    {
      test.handle_input(event);

      if(event.type == SDL_QUIT)
      {
        quit = true;
      }
      if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
      {
        if(!hasServerIp)
        {
          strcpy(serverIp, test.getString().c_str());
          test.clear();
          hasServerIp = true;
          conversation.push_back("Enter the port of the server you would like to connect to");
        }
        else if(!hasServerPort)
        {
          serverPort = atoi(test.getString().c_str());
          test.clear();
          hasServerPort = true;
          conversation.push_back("Enter the name you would like to use");
        }
        else if(!hasName)
        {
          strcpy(name, test.getString().c_str());
          test.clear();
          hasName = true;
          conversation.clear();
        }
        else if(test.getString().compare("") != 0)
        {
          ss.str(string());
          ss << "CIM" << test.getString();
          //conversation.push_back(ss.str());
          if(sock.sendMessage(&serverAddr, ss.str()) < 0)
          {
            conversation.push_back("Error");
          }
          test.clear();
        }
      }
    }
    if(hasName && !isConnected)
    {
      memset(&serverAddr, 0, sizeof(serverAddr));
      serverAddr.sin_family = AF_INET;
      serverAddr.sin_addr.s_addr = inet_addr(serverIp);
      serverAddr.sin_port = htons(serverPort);
      ss.str(string());
      ss << "CCIM" << name;
      sock.sendMessage(&serverAddr, ss.str().c_str());
      isConnected = true;
    }

    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 220, 220, 220));
    height = 10;
    if (conversation.size() > 15)
      conversation.pop_front();
    for (it = conversation.begin(); it != conversation.end(); it++)
    {
      SDL_FreeSurface(text);
      text = TTF_RenderText_Solid(font, it->c_str(), textColor);
      draw_surface(10, height, text, screen);
      height += 26;
    }
    test.show(screen);

    SDL_Flip(screen);
  }

  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
  sock.close();

  return 0;
}
