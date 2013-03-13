#pragma once
#include <string>
#include <fstream>
#define Pause(a) SDL_Delay(a)
#define Maximun(a, b) (a>b)?a:b
#define Minimum(a, b) (a<b)?a:b 
enum ErrorState{Log, Caption, LogCaption};
class Error{
private:
	void LogError(std::string str, int code);
public:
	void HandleError(ErrorState state, std::string errormsg, int code=0);
	Error();
	Error(std::string errormsg, int code = 0);
	Error(ErrorState state, std::string errormsg, int code=0);
};
#ifdef SDL
#define IMAGE
#define TEXT
#include <SDL\SDL.h>
#undef main
#define main SDL_main
#pragma comment(lib,"SDL.lib")
#pragma comment(lib,"SDLmain.lib")
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup" )

#ifdef IMAGE
#include <SDL\SDL_image.h>
#pragma comment(lib, "SDL_image.lib")
#endif
#ifdef TEXT
#include <SDL\SDL_ttf.h>
#pragma comment(lib, "SDL_ttf.lib")
#endif
#ifdef AUDIO
#include <SDL\SDL_mixer.h>
#pragma comment(lib, "SDL_mixer.lib")
#endif


class Point2D{
public:
	int X, Y;
	Point2D();
	Point2D(int x, int y);
	Point2D operator += (Point2D point);
	Point2D operator -= (Point2D point);
	Point2D operator *= (Point2D point);
	Point2D operator /= (Point2D point);
	bool Equals(Point2D point);
};
class Rectangle{
public:
	int X, Y, Width, Height;
	Rectangle();
	Rectangle(int x, int y, int width, int height);
	bool Contains(int x, int y);
	bool Contains(Point2D point);
	bool Rectangle::Intersect(Rectangle bound);
};

bool InitSDL();
void FreeSDL();

#ifdef TEXT
class Font{
	TTF_Font* _font;
	SDL_Color _color;
public:
	Font();
	bool Load(std::string filename, int size);
	operator TTF_Font*();
	TTF_Font* operator->();
	bool IsInit();
	void SetColor(int r = 0xff, int g = 0xff, int b = 0xff);
	void SetColor(SDL_Color color);
	void GetColor(int& r, int&g, int&b);
	SDL_Color GetColor();
	bool Free();
};
#endif
class BaseSurface {
protected:
	SDL_Surface* _surface;
	BaseSurface();
	BaseSurface(SDL_Surface* surface);
public:
	operator SDL_Surface*();
	SDL_Surface* operator -> ();
	bool IsInit();
};
class WindowSurface : public BaseSurface { 
private:
	int _width, _height;
public:
	WindowSurface(int width, int height, int bbp = 32, bool window = true, bool doublebuffer=false);
	bool CreateSurfaceWindow(int width, int height, int bbp = 32, bool window = true, bool doublebuffer=false);
	void SetCaption(std::string caption);
	std::string GetCaption();
	bool Update();
	bool ClearWindow(int r, int g, int b);
};
class Surface : public BaseSurface {
private:
	Uint8 _r, _g, _b;
public:
	Surface();
	Surface(SDL_Surface* surface);
	bool Draw(WindowSurface screen, unsigned int x, unsigned int y, SDL_Rect* clip=0);
	int Width();
	int Height();
	bool Free();
	bool Load(std::string filename, bool useImgAlpha=false, int a=0xff);
	bool Load(std::string filename, int r=0xff, int g=0xff, int b=0xff, int a=0xff, bool useImgAlpha=false);
	bool MaskColor(int r, int g, int b);
	bool SetTransparacy(int a);
	void GetMask(int& r, int& g, int& b);

#ifdef TEXT
	bool RenderText(Font font, std::string text);
#endif
	
};
#endif