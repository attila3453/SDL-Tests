#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <stdio.h>

unsigned int SW = 400;
unsigned int SH = 400;

int main(int argc, char **argv){
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SW, SH, SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(window);

    glMatrixMode(GL_PROJECTION | GL_MODELVIEW);

    glLoadIdentity();

    glOrtho(0, SW, SH, 0, 0, 1);

    SDL_Event event;

    bool running = 1;

    const int desiredfps = 24;
    int lasttime = 0;

    glClearColor(1, 1, 1, 1);

    while(running){
        lasttime = SDL_GetTicks();

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            running = 0;
                            break;
                    }
            }
        }

        int x = SW / 2;
        int y = SH / 2;

        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
            glColor3b(1, 0, 0);
            glVertex2d(200, 200);
            glColor3b(0, 1, 0);
            glVertex2d(300, 300);
            glColor3b(0, 0, 1);
            glVertex2d(200, 400);
        glEnd();

        SDL_GL_SwapWindow(window);

        int time = SDL_GetTicks() - lasttime;
        if(time < desiredfps){
            printf("Waited :%d miliseconds\n", desiredfps - time);
            SDL_Delay(desiredfps - time);
        }
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
