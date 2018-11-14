#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/common.hpp>

#include <exception>

//#define WINDOW_WIDTH 640
//#define WINDOW_HEIGHT 480

int main(int argc, char *argv[])
{
	int windowWidth = 640;
	int windowHeight = 480;

  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    throw std::exception();
  }

  SDL_Window *window = SDL_CreateWindow("Triangle",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    windowWidth, windowHeight,
    SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  if(!SDL_GL_CreateContext(window))
  {
    throw std::exception();
  }

  if(glewInit() != GLEW_OK)
  {
    throw std::exception();
  }

  /*VertexBuffer *positions = new VertexBuffer();
  positions->add(glm::vec3(0.0f, 0.5f, 0.0f));
  positions->add(glm::vec3(-0.5f, -0.5f, 0.0f));
  positions->add(glm::vec3(0.5f, -0.5f, 0.0f));

  VertexBuffer *texCoords = new VertexBuffer();
  texCoords->add(glm::vec2(0.5f, 0.0f));
  texCoords->add(glm::vec2(0.0f, 1.0f));
  texCoords->add(glm::vec2(1.0f, 1.0f));

  VertexArray *shape = new VertexArray();
  shape->SetBuffer("in_Position", positions);
  shape->SetBuffer("in_TexCoord", texCoords);*/

  ShaderProgram *shader = new ShaderProgram("../shaders/simple.vert", "../shaders/simple.frag");
  VertexArray *hallShape = new VertexArray("../object/re_hall_baked.obj");
  Texture *hallTexture = new Texture("../diffuse/re_hall_diffuse.png");
  VertexArray *shape = new VertexArray("../object/curuthers.obj");
  Texture *texture = new Texture("../diffuse/curuthers_diffuse.png");

  bool quit = false;
  float angle = 0;

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  while(!quit)
  {
    SDL_Event event = {0};

    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        quit = true;
      }
    }

	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw with perspective projection matrix
    shader->SetUniform("in_Projection", glm::perspective(glm::radians(45.0f),
     (float)windowWidth / (float)windowHeight, 0.1f, 100.f));

	//Camera
	/*glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
	shader->SetUniform("in_View", glm::inverse(model));*/

    glm::mat4 model (1.0f);
    model = glm::translate(model, glm::vec3(0, 0, -12.5f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
    shader->SetUniform("in_Model", model);
	shader->SetUniform("in_Texture", texture);
    shader->draw(shape);

    // Draw with orthographic projection matrix

    shader->SetUniform("in_Projection", glm::ortho(0.0f,
     (float)windowWidth, 0.0f, (float)windowHeight, 0.0f, 1.0f));

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(100, windowHeight - 100, 0));
    model = glm::scale(model, glm::vec3(100, 100, 1));
    shader->SetUniform("in_Model", model);
    shader->draw(shape);

    angle+=0.01f;

    SDL_GL_SwapWindow(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
