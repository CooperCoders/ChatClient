#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL_init.h"
#include "StringInput.h"
#include <list>
#include <process.h>
#include <winsock2.h>
#define MESSAGE_MAX 255

using namespace std;

SDL_Surface* screen;
list<string> conversation (1,"Please enter the IP of the server you would like to join");
list<string>::iterator it;
//list<string> members;


int sock;
struct sockaddr_in serverAddr;
struct sockaddr_in clientAddr;
struct sockaddr_in fromAddr;
int clientAddrLength;
unsigned short serverPort;
WSADATA wsaData;
char* message2;
char messageBuffer[MESSAGE_MAX];
char CIM[3] = {'C', 'I', 'M'};
stringstream ss;
//bool isServer = false;
//bool hasChosen = false;
bool hasIp = false;
bool hasPort = false;
bool isConnected = false;
bool isBound = false;
char serverIp[1000];

void thread(void* arg)
{
  while (true)
  {
    if (isConnected)
    {
      stringstream stream;
      if (recvfrom(sock, messageBuffer, MESSAGE_MAX, 0,NULL,NULL) < 0)
      {
        //cout << "recvfrom failed\n" << WSAGetLastError();
        exit(WSAGetLastError());
      }
      if(!memcmp(messageBuffer, CIM, 3))
      {
        messageBuffer[strlen(messageBuffer) - 1] = '\0';
        stream.str(string());
        stream << messageBuffer + 3;
        conversation.push_back(stream.str());
        /*if(isServer)
        {
          members.push_back(inet_ntoa(clientAddr.sin_addr));
          members.sort();
          members.unique();
        }*/
      }
      memset(messageBuffer, '\0', MESSAGE_MAX);
    }
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

  if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
  {
    //cout << "WSAStartup() failed\n";
    exit(1);
  }
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
  {
    //cout << "Socket creation failed\n";
    exit(1);
  }



  clientAddrLength = sizeof(clientAddr);



  if (init(480, 640, 32))
    return 0;

  font = TTF_OpenFont("arial.ttf", 16);
  StringInput test(10, 480 - 26, textColor, font);
  text = TTF_RenderText_Solid(font, "Hello", textColor);

  _beginthread(thread,0,NULL);

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
        if(!hasIp)
        {
          strcpy(serverIp, test.getString().c_str());
          hasIp = true;
          conversation.push_back(test.getString().c_str());
          test.clear();
          conversation.push_back("Please enter the port the server is using");
        }
        else if(!hasPort)
        {
          serverPort = atoi(test.getString().c_str());
          conversation.push_back(test.getString().c_str());
          test.clear();
          hasPort = true;
          conversation.clear();
        }
        else if(test.getString().compare("") != 0)
        {
          sendto(sock, test.getString().c_str(), MESSAGE_MAX, 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
          ss.str(string());
          ss << "Me: " << test.getString();
          conversation.push_back(ss.str());
          test.clear();
        }
      }
    }
    if(!isBound && hasPort)
    {
      memset(&serverAddr, 0, sizeof(serverAddr));
      serverAddr.sin_family = AF_INET;
      serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      serverAddr.sin_port = htons(serverPort);

      memset(&clientAddr, 0, sizeof(clientAddr));
      clientAddr.sin_family = AF_INET;
      clientAddr.sin_addr.s_addr = inet_addr(serverIp);
      clientAddr.sin_port = htons(serverPort+1);

      if (bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
      {
        //cout << "Bind failed\n";
        exit(1);
      }
      isBound = true;
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
  closesocket(sock);
  WSACleanup();

  return 0;
}
