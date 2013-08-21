#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <GL/gl.h>
#include <stdio.h>
#include <sstream>
#include <cstring>

using std::stringstream;
using std::string;

unsigned int SW = 400;
unsigned int SH = 400;
const int FramesPerSecond = 24;

string toString(int nr) {
    stringstream ss;
    ss << nr;
    return ss.str();
}

class Timer{
    private:
        unsigned int starttime;
        bool running;
    public:
        Timer();
        void start();
        void stop();

        unsigned int getTime();
        bool isRunning();
};

Timer::Timer(){
    starttime = 0;
    running = 0;
}

void Timer::start(){
    running = 1;
    starttime = SDL_GetTicks();
}

void Timer::stop(){
    running = 0;
    starttime = 0;
}

unsigned int Timer::getTime(){
    return SDL_GetTicks() - starttime;
}

int main(int argc, char ** argv) {
	SDL_Window* window = 0;
	SDL_Surface* screenSurface = 0;//pixels

	if(SDL_Init(SDL_INIT_EVERYTHING)){//returns 0 if success
        printf("SDL error %s", SDL_GetError());
        return -1;
	}

	window = SDL_CreateWindow("Crows", 200, 200, SW, SH, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if(!window){
        printf("SDL error %s", SDL_GetError());
        return -1;
	}

	screenSurface = SDL_GetWindowSurface(window);

    SDL_Surface *banciu = IMG_Load("banciu.jpg");

    banciu = SDL_ConvertSurfaceFormat(banciu, screenSurface->format->format, 0);

	printf("Image size:%d %d", banciu->w, banciu->h);

	SDL_BlitSurface(banciu, 0, screenSurface, 0);

	SDL_FreeSurface(banciu);

	bool running = 1;
    SDL_Event event;

    SDL_Surface *surfacefont = 0;

    TTF_Init();

    TTF_Font *fontubuntu = 0;
    SDL_Color fontcolor = {200, 200, 200};

    fontubuntu = TTF_OpenFont("Ubuntu-R.ttf", 32);

    surfacefont = TTF_RenderText_Solid(fontubuntu, "Wake up, Neo...", fontcolor);

    SDL_BlitSurface(surfacefont, 0, screenSurface, 0);

    SDL_FreeSurface(surfacefont);

    TTF_CloseFont(fontubuntu);

	SDL_UpdateWindowSurface(window);

	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096 );

	Mix_Music *music;

	music = Mix_LoadMUS("saltarello.mp3");

	Timer timer1, timer2;

	timer1.start();

	while(running){
        timer2.start();

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_WINDOWEVENT_RESIZED:

                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            running = 0;
                            break;
                    }
            }
        }

        if(!Mix_PlayingMusic())
          Mix_PlayMusic(music, 1);

        int ticks = timer2.getTime();

        if(ticks < FramesPerSecond)
            SDL_Delay(FramesPerSecond - ticks);
	}

	SDL_DestroyWindow(window);
	TTF_Quit();
	Mix_FreeMusic(music);
	Mix_Quit();
	SDL_Quit();

	return 0;
}
