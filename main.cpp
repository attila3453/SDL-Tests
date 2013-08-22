#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <glm/glm.hpp>

using std::string;
using std::ifstream;

unsigned int SW = 400;
unsigned int SH = 400;

GLuint loadShaders(const char *vertexpath, const char *fragmentpath){
    GLuint vertexid = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentid = glCreateShader(GL_FRAGMENT_SHADER);

    string vertexcode, fragmentcode, line;

    ifstream stream;
    stream.open(vertexpath);
    for(;getline(stream, line);vertexcode += "\n" + line);
    stream.close();

    stream.open(fragmentpath);
    for(;getline(stream, line);fragmentcode += "\n" + line);
    stream.close();

    GLint ret = GL_FALSE;
    int loglen = 0;
    GLchar *log;

    printf("Compiling shader:%s\n", vertexpath);
    const char *vertexcodeptr = vertexcode.c_str();
    glShaderSource(vertexid, 1, &vertexcodeptr, NULL);
    glCompileShader(vertexid);

    glGetShaderiv(vertexid, GL_COMPILE_STATUS, &ret);
    glGetShaderiv(vertexid, GL_INFO_LOG_LENGTH, &loglen);

    if(loglen){
        glGetShaderInfoLog(vertexid, loglen, NULL, log);
        printf("%s\n", log);
    }

    printf("Compiling shader:%s\n", fragmentpath);
    const char *fragmentcodeptr = fragmentcode.c_str();
    glShaderSource(fragmentid, 1, &fragmentcodeptr, NULL);
    glCompileShader(fragmentid);

    loglen = 0;

    glGetShaderiv(fragmentid, GL_COMPILE_STATUS, &ret);
    glGetShaderiv(fragmentid, GL_INFO_LOG_LENGTH, &loglen);

    if(loglen){
        glGetShaderInfoLog(fragmentid, loglen, NULL, log);
        printf("%s\n", log);
    }

    printf("Linking GL program\n");

    GLuint programid = glCreateProgram();
    glAttachShader(programid, vertexid);
    glAttachShader(programid, fragmentid);
    glLinkProgram(programid);

    loglen = 0;

    glGetProgramiv(programid, GL_LINK_STATUS, &ret);
    glGetProgramiv(programid, GL_INFO_LOG_LENGTH, &loglen);

    if(loglen){
        glGetProgramInfoLog(programid, loglen, 0, log);
        printf("%s\n", log);
    }

    glDeleteShader(vertexid);
    glDeleteShader(fragmentid);

    return programid;
}

int main(int argc, char **argv){
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SW, SH,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_GLContext context = SDL_GL_CreateContext(window);

    SDL_Event event;

    bool running = 1;

    const int desiredfps = 24;
    int lasttime = 0;

    glewExperimental = 1;
    glewInit();

    GLuint triangleid;
    glGenVertexArrays(1, &triangleid);
    glBindVertexArray(triangleid);

    GLfloat triangledata[] = {
            -1, -1, 0,
            1,  -1, 0,
            0,   1, 0};

    GLuint vertexbuffer;

    GLuint programid = loadShaders("shader.vertex", "shader.fragment");

    glClearColor(0, 0, 1, 1);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangledata), triangledata, GL_STATIC_DRAW);

    //glm::mat4 matprojection = glm::perspective(45.0, SW / SH, 0.1, 100.0);
    //glm::mat4 matview = glm::lookAt(glm::vec3(4,4,4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    //glm::mat4 matmodel = glm::mat4();
    //glm::mat4 mvp = matprojection * matview * matmodel;

    //GLuint matid = glGetUniformLocation(programid, "mvp");

    //glUniformMatrix4fv(matid, 1, GL_FALSE, mvp);

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
                        case SDLK_a:
                            triangledata[0] = 0;
                            printf("0\n");
                            break;
                        case SDLK_b:
                            triangledata[0] = -1;
                            break;
                    }
                    break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programid);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(0);

        SDL_GL_SwapWindow(window);

        int time = SDL_GetTicks() - lasttime;
        if(time < desiredfps){
            //printf("Waited :%d miliseconds\n", desiredfps - time);
            SDL_Delay(desiredfps - time);
        }
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
