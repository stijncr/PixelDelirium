#include "Framework.h"

Error::Error() {}
Error::Error(std::string errormsg, int code){
	HandleError(Log, errormsg, code);
}
Error::Error(ErrorState state, std::string errormsg, int code){
	HandleError(state, errormsg, code);
}
void Error::LogError(std::string str, int code){
	std::ofstream out("log.txt",std::ios::out|std::ios::app|std::ios::binary);
	out<<str << "  " << code;	
	if (str.length()>0)	{
		out<<"\n\r";
		out<<std::endl;
	}
	out.flush();
	out.close();
}
void Error::HandleError(ErrorState state, std::string errormsg, int code){
#ifdef SDL
	if(state == Caption || state == LogCaption)
		SDL_WM_SetCaption(errormsg.c_str(), 0);
#endif
	if(state==Log||state==LogCaption)
		LogError(errormsg, code);
}

#ifdef SDL
bool InitSDL(){
	int flag = SDL_INIT_VIDEO;
#ifdef AUDIO
	flag |= SDL_INIT_AUDIO;
#endif
	if(SDL_Init(flag)==-1) {
		Error("Couldn't initialize default video/audio");
		return false;
	}
#ifdef AUDIO
	if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 )==-1)
	{
		Error("Couldn't initialize audio");
		Mix_CloseAudio();
		return true;
	}
#endif
#ifdef TEXT
	if (TTF_Init()==-1)//Initialize Font extension
	{
		Error("Couldn't initialize ttf text");
		return false;
	};
#endif
	return true;

};
void FreeSDL(){
	SDL_Quit();
#ifdef AUDIO
	Mix_CloseAudio();
#endif
#ifdef TEXT
	TTF_Quit();
#endif
};

Point2D::Point2D():X(0), Y(0) { }
Point2D::Point2D(int x, int y):X(x), Y(y) { }
Point2D Point2D::operator += (Point2D point) { X += point.X; Y += point.Y; return *this; };
Point2D Point2D::operator -= (Point2D point) { X -= point.X; Y -= point.Y; return *this; };
Point2D Point2D::operator *= (Point2D point) { X *= point.X; Y *= point.Y; return *this; };
Point2D Point2D::operator /= (Point2D point) { X /= point.X; Y /= point.Y; return *this; };

bool Point2D::Equals(Point2D point){ 
	return (point.X==X&&point.Y==Y)?true:false;
}

	
Rectangle::Rectangle() : X(0), Y(0), Width(0), Height(0) { }
Rectangle::Rectangle(int x, int y, int width, int height):X(x), Y(y), Width(width), Height(height) { }
bool Rectangle::Contains(int x, int y){
	if(X <= x && X+Width >= x && Y<=y && Y + Height >= y) return true;
	return false;
}
bool Rectangle::Contains(Point2D point){
	if(X <= point.X && X+Width >= point.X && Y<=point.Y && Y + Height >= point.Y) return true;
	return false;
}
bool Rectangle::Intersect(Rectangle bound){
	if(Contains(bound.X, bound.Y)) return true;
	else if(Contains(bound.X, bound.Y + bound.Height)) return true;
	else if(Contains(bound.X +bound.Width, bound.Y)) return true;
	else if(Contains(bound.X + bound.Width, bound.Y + bound.Height)) return true;
	return false;
}

#ifdef TEXT
Font::Font():_font(0){}
bool Font::Load(std::string filename, int size){
	_font = TTF_OpenFont(filename.c_str(), size);
	if(_font==0){
		Error("Couldn't load ttf font");
		return false;	
	}
	return true;
}
Font::operator TTF_Font*() { return _font; }
TTF_Font* Font::operator->(){return _font; }
bool Font::IsInit(){return (_font!=0);}
void Font::SetColor(int r, int g, int b){
	if(r<0)r=0;if(g<0)g=0;if(b<0)b=0;
	if(r>0xff)r=0xff;if(g>0xff)g=0xff;if(b>0xff)b=0xff;
	_color.r = r;
	_color.b = b;
	_color.g = g;
}
void Font::SetColor(SDL_Color color){_color = color; }
void Font::GetColor(int& r, int&g, int&b){
	r=_color.r;g=_color.g;b=_color.b;
}
SDL_Color Font::GetColor(){return _color;}
bool Font::Free(){
	if(_font!=0){
		TTF_CloseFont(_font);
		_font=0;
		return true;
	}
	return false;
}
#endif

BaseSurface::BaseSurface() : _surface(0){}
BaseSurface::BaseSurface(SDL_Surface* surface) : _surface(surface) { }
BaseSurface::operator SDL_Surface*() { return _surface; }
SDL_Surface* BaseSurface::operator -> () { return _surface;}
bool BaseSurface::IsInit() { return (_surface!=0); }

WindowSurface::WindowSurface(int width, int height, int bbp, bool window, bool doublebuffer){
	CreateSurfaceWindow(width, height, bbp, window, doublebuffer);
}
bool WindowSurface::CreateSurfaceWindow(int width, int height, int bbp, bool window, bool doublebuffer){
	_width = width; _height = height;
	unsigned int flags=0;
	if (doublebuffer==true){flags|=SDL_HWSURFACE|SDL_DOUBLEBUF;}else{flags|=SDL_SWSURFACE;}
	//Makes the frame resizable, or just non-existent, depending on the boolean
	if (window==false){flags|=SDL_NOFRAME;}else{flags|=SDL_RESIZABLE;}
		_surface = SDL_SetVideoMode(width, height, bbp, flags);
		if(_surface == 0 ){
			Error("Couldn't create window frame");
			return false;
		}
		return true;
	}
void WindowSurface::SetCaption(std::string caption){
	SDL_WM_SetCaption(caption.c_str(), NULL);
}
std::string WindowSurface::GetCaption(){
	char* w;
	SDL_WM_GetCaption(&w, NULL);
	return std::string(w);
}
bool WindowSurface::Update(){
	if(_surface != 0){
		if(SDL_Flip(_surface) == -1) {Error("Couldn't update/flip screen");return false;}
		return true;
	}
	return false;
}
bool WindowSurface::ClearWindow(int r, int g, int b){
	if(_surface!=0){
		if (SDL_FillRect(_surface,0,SDL_MapRGB(_surface->format,r,g,b))){
			Error("Couldn't clear window with color: (");
			return false;
		}
		return true;
	}
	return false;
}

Surface::Surface() :BaseSurface() {};
Surface::Surface(SDL_Surface* surface):BaseSurface(surface) {};
bool Surface::Draw(WindowSurface screen, unsigned int x, unsigned int y, SDL_Rect* clip){
	if(_surface != 0){
		SDL_Rect offset;
		offset.x = x;
		offset.y = y;
		int v = SDL_BlitSurface(_surface, clip, screen, &offset);
		if(v == 1) {//blit not succes
			Error("Couldn't blit surface");
			return false;
		}
		else if(v == 2){ // blitmem lost
			Error("Blitmemory lost in vmem");
			return false;
		}
		return true;
	}
	return false;
};
int Surface::Width() { return (_surface!=0)?_surface->w:0; }
int Surface::Height() { return (_surface!=0)?_surface->h:0; }
bool Surface::Free(){
	if(_surface!=0){
		SDL_FreeSurface(_surface);
		_surface=0;
		return true;
	}
	return false;
}
bool Surface::Load(std::string filename, bool useImgAlpha, int a){
	return Load(filename, -1, -1, -1, a, useImgAlpha);
}
bool Surface::Load(std::string filename, int r, int g, int b, int a, bool useImgAlpha){
	Surface loadedImage = NULL;
#ifdef IMAGE
	loadedImage = IMG_Load( filename.c_str() );
#else 
	loadedImage = SDL_LoadBMP(filename.c_str());
#endif
	if(loadedImage!=0)
	{
		if(r!=-1&&g!=-1&&b!=-1)loadedImage.MaskColor(r,g,b);
		if(a<SDL_ALPHA_OPAQUE&&a>=0)loadedImage.SetTransparacy(a);
		if(useImgAlpha) _surface = SDL_DisplayFormatAlpha(loadedImage);
		else _surface = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );
		return true;
	}
	Error("Couln't load image: " + filename);
	return false;
}
	
bool Surface::MaskColor(int r, int g, int b){
	if(_surface!=0){
		int f = SDL_SRCCOLORKEY;
		if(r<0||g<0||b<0) f=0; _r=r;_g=g;_b=b;
		if(r>0xff)r=0xff;if(g>0xff)g=0xff;if(b>0xff)b=0xff;
		Uint32 colorkey = SDL_MapRGB( _surface->format, r, g, b);
		if(SDL_SetColorKey( _surface, f, colorkey ) == -1){
			Error("Couldn't set color key:");
			return false;
		}
		return true;
	}
	Error("Couldn't mask color, surface not initialized");
	return false;
};
bool Surface::SetTransparacy(int a){
	if (_surface!=0)
	{
		if (a<SDL_ALPHA_TRANSPARENT)a=SDL_ALPHA_TRANSPARENT;
		if (a>SDL_ALPHA_OPAQUE)a=SDL_ALPHA_OPAQUE;
		if (SDL_SetAlpha(_surface,SDL_SRCALPHA,a)==-1){
			Error("Couldn't set trasparacy: " +a);
			return false;
		}
		return true;
	}
	Error("Couldn't mask color, surface not initialized");
	return false;
}
void Surface::GetMask(int& r, int& g, int& b){
	r=_r;g=_g;b=_b;
}

#ifdef TEXT
bool Surface::RenderText(Font font, std::string text){
	Free();
	_surface=TTF_RenderText_Solid( font, text.c_str(), font.GetColor() );
	if(_surface==0){
		Error("Couldn't render solid ttf text");
		return false;
	}
	return true;
}
#endif
#endif