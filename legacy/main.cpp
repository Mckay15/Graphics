#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "GameObject.h"
#include "FrameBufferObject.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/common.hpp>

#include <exception>

//#define WINDOW_WIDTH 640
//#define WINDOW_HEIGHT 480

int main(int argc, char *argv[])
{
	int HitCount = 0;
	int windowWidth = 640;
	int windowHeight = 480;
	bool LeftKey = false;
	bool RightKey = false;
	float X_Movement = 0.0f;
	double time = 0.0;
	double deltaT = 1.0 / 60.0;
	double Z_Movement = 0.0;

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
  ShaderProgram *shader_Normal = new ShaderProgram("../shaders/simple.vert", "../shaders/normal.frag");
  ShaderProgram *shader_Framebuffer = new ShaderProgram("../shaders/FrameBuffer.vert", "../shaders/FrameBuffer.frag");
  VertexArray *hallShape = new VertexArray("../object/re_hall_baked.obj");
  GameObject *Car = new GameObject("../diffuse/Car_Texture.png","../object/Car.obj");
  GameObject *Box = new GameObject("../textures/box.png", "../object/box.obj");
  GameObject *Box2 = new GameObject("../textures/box.png", "../object/box.obj","../textures/box_Normal.png");
  FrameBufferObject *FBO = new FrameBufferObject();
 // VertexArray *car = new VertexArray("../object/Car.obj");
 // Texture *carTexture = new Texture("../diffuse/Car_Texture.png");
  //Texture *hallTexture = new Texture("../diffuse/re_hall_diffuse.png");
  //VertexArray *shape = new VertexArray("../object/curuthers.obj");
  //Texture *texture = new Texture("../diffuse/curuthers_diffuse.png");
  //Texture *normal = new Texture("../diffuse/curuthers_normal.png");

  std::vector <GameObject*> Boxes;
  glm::mat4 BoxCoords[2] = { glm::mat4(1.0), glm::mat4(1.0) };

  BoxCoords[0] = glm::translate(BoxCoords[0], glm::vec3(0, -2.0f, -8.5f));
  BoxCoords[1] = glm::translate(BoxCoords[1], glm::vec3(0, -2.0f, -18.5f));

  Boxes.push_back(Box);
  Boxes.push_back(Box2);

  bool quit = false;
  float angle = 0;

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
  };

  unsigned int quadVAO, quadVBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

  shader_Framebuffer->SetUniform("screenTexture", 0);

  unsigned int fbo;

  glGenFramebuffers(1, &fbo);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  unsigned int textureColorbuffer;
  glGenTextures(1, &textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

  unsigned int rbo;

  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
	  throw std::exception();
  }

  //glBindFramebuffer(GL_FRAMEBUFFER, 0);

  while(!quit)
  {
    SDL_Event event = {0};

    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        quit = true;
      }
	  switch (event.type)
	  {
	  case SDL_KEYDOWN :
		  if (event.key.keysym.sym == SDLK_LEFT)
		  {
			  LeftKey = true;
		  }
		  if (event.key.keysym.sym == SDLK_RIGHT)
		  {
			  RightKey = true;
		  }
	  break;
	  case SDL_KEYUP :
		  LeftKey = false;
		  RightKey = false;
	  break;
	  default:
		  break;
	  }
	 /* if (SDL_KEYDOWN)
	  {
		  if (event.key.keysym.sym == SDLK_LEFT)
		  {
			  LeftKey = true;
		  }
	  }
	  else if (SDL_KEYUP)
	  {
		  LeftKey = false;
	  }
	  if (event.key.keysym.sym == SDLK_RIGHT)
	  {
		  RightKey = true;
	  }
	  else
	  {
		  RightKey = false;
	  }*/
    }

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glEnable(GL_DEPTH);

	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw with perspective projection matrix
    shader->SetUniform("in_Projection", glm::perspective(glm::radians(45.0f),
     (float)windowWidth / (float)windowHeight, 0.1f, 100.f));

	//lighting
	shader->SetUniform("in_Emissive", glm::vec3(0.1, 0.1, 0.1));
	shader->SetUniform("in_Ambient", glm::vec3(0.2, 0.2, 0.2));
	shader->SetUniform("in_LightPos", glm::vec3(50, 10, 10));

	//Camera
	glm::mat4 model(1.0f);
	shader->SetUniform("in_View", glm::inverse(model));
	shader_Normal->SetUniform("in_View", glm::inverse(model));
  //  shader->SetUniform("in_Tangent", glm::mat4(1));
    //glm::mat4 model (1.0f);
    model = glm::translate(model, glm::vec3(0, -2.0f, -8.5f));
	Car->Movement(glm::vec3(0, -2.0f, -8.5f));
	Box->Movement(glm::vec3(0, -2.0f, -18.5f));
	if (LeftKey == true && X_Movement > -2.7f)
	{
		X_Movement -= 0.1f * deltaT;
	}
	if (RightKey == true && X_Movement < 2.7)
	{
		X_Movement += 0.1f * deltaT;
	}
	Z_Movement += 0.05 * deltaT;
	Car->Movement(glm::vec3(X_Movement, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(X_Movement, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
    //model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
    shader->SetUniform("in_Model", Car->GetModel());
	shader->SetUniform("in_Texture", Car->GetTexture());
	shader->draw(Car->GetVAO());

	
	//shader->SetUniform("in_View", glm::inverse(Box->GetModel()));
	Box->Movement(glm::vec3(0, 0.0f, Z_Movement));
	shader->SetUniform("in_Model", Box->GetModel());
	shader->SetUniform("in_Texture", Box->GetTexture());
	shader->draw(Box->GetVAO());

	shader_Normal->SetUniform("in_Projection", glm::perspective(glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 0.1f, 100.f));

	shader_Normal->SetUniform("in_Emissive", glm::vec3(0.1, 0.1, 0.1));
	shader_Normal->SetUniform("in_Ambient", glm::vec3(0.2, 0.2, 0.2));
	shader_Normal->SetUniform("in_LightPos", glm::vec3(50, 10, 10));

	Box2->Movement(glm::vec3(-1.0, -2.0f, -28.5f));
	Box2->Movement(glm::vec3(0, 0.0f, Z_Movement));
	shader_Normal->SetUniform("in_Model", Box2->GetModel());
	shader_Normal->SetUniform("in_Texture", Box2->GetTexture());
	shader_Normal->SetUniform("in_NormalMap", Box2->GetNormalMap());
	shader_Normal->draw(Box2->GetVAO());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader_Framebuffer->UseThis();
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//shader->SetUniform("in_NormalMap", normal);
   // shader->draw(shape);
//	model = glm::translate(model, glm::vec3(0, 0, -8.5f));

	for (int i = 0; i < Boxes.size(); i++)
	{
		if (Boxes[i]->GetModel()[3].z <= -7.5f && Boxes[i]->GetModel()[3].z >= -9.5f)
		{
			if (Boxes[i]->GetModel()[3].x >= Car->GetModel()[3].x - 1 && Boxes[i]->GetModel()[3].x <= Car->GetModel()[3].x + 1)
			{
				HitCount++;
			}
		}
		if (Boxes[i]->GetModel()[3].z >= 15.0f)
		{
			quit = true;
		}
	}

    // Draw with orthographic projection matrix

    /*shader->SetUniform("in_Projection", glm::ortho(0.0f,
     (float)windowWidth, 0.0f, (float)windowHeight, 0.0f, 1.0f));*/

   /* model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(100, windowHeight - 100, 0));
    model = glm::scale(model, glm::vec3(100, 100, 1));
    shader->SetUniform("in_Model", model);
    shader->draw(shape);*/

    angle+=0.01f;

    SDL_GL_SwapWindow(window);
	time += deltaT;
	Car->Reset();
	Box->Reset();
	Box2->Reset();
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
