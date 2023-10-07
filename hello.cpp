// g++ hello.cpp ./glad/src/glad.c -I ./glad/include/ -o hello -lSDL2

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include "./glad/include/glad/glad.h"
#include <SDL2/SDL.h>

void processInput(SDL_Event *event, bool *gameIsRunning);

//vertex shader source
const char *vertexShaderSource = "#version 410 core\n"
                                "layout (location = 0) in vec3 aPos;\n"
                                "void main()\n"
                                "{\n"
                                "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                "}\0";

//fragment shader source
const char *fragmentShaderSource = "#version 410 core\n"
                                "out vec4 FragColor;\n"
                                "void main()\n"
                                "{\n"
                                "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                "}\0";

int main()
{
    SDL_Window *window =nullptr;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

    window = SDL_CreateWindow("Learn OpenGL",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1200, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if(window == NULL)
    {
        std::cout<<"Failed to create SDL window";
        SDL_Quit();
        return -1;
    }

    SDL_GLContext context;

    context = SDL_GL_CreateContext(window);

    if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout<<"Failed to load GLAD";
        SDL_Quit();
        return -1;
    }

    glViewport(0, 0, 1200,600);


    SDL_SetWindowResizable(window,SDL_TRUE);    //technically i need to change the viewport height and width but in a demo it worked fine

    //building and compiling shaders

    //vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader); 

    //for checking if compilation was successfull
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }

    //fragment shader 
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }

    //linking shader
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    //linking error
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout<<"ERROR::SHADER::PROGRAM::LINKING_FAILED\n"<<infoLog<<std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //vertex shader data and buffer

    float vertices[] = { 
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    unsigned int VBO, VAO;

    glGenVertexArrays(1,&VAO);

    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO); //huh
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0); //huh
    glBindVertexArray(0);

    bool gameIsRunning = true;

    while(gameIsRunning)
    {
        SDL_Event event;

        processInput(&event, &gameIsRunning);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);          //swap the buffers
    }

    SDL_DestroyWindow(window);

    SDL_Quit;


    return 0;
}


void processInput(SDL_Event *event,bool *gameIsRunning)
{
    while(SDL_PollEvent(event))
        {
            if(event->type == SDL_QUIT || event->key.keysym.sym == SDLK_ESCAPE)
            {
                *gameIsRunning = false;
            }
        }


}
