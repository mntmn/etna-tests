#include <stdio.h>
#include <stdlib.h>
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <GLFW/glfw3.h>

static void error_callback(int error, const char *description) {
  fputs(description, stderr);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

static GLuint texName;

static char *circles[] = {
"................",
"................",
"......xxxx......",
"....xxxxxxxx....",
"...xxxxxxxxxx...",
"...xxx....xxx...",
"..xxx......xxx..",
"..xxx......xxx..",
"..xxx......xxx..",
"..xxx......xxx..",
"...xxx....xxx...",
"...xxxxxxxxxx...",
"....xxxxxxxx....",
"......xxxx......",
"................",
"................",
};

static void init_texture(void)
{
  GLubyte floorTexture[16][16][4];
  GLubyte *loc;
  int s, t;
  /* Setup RGB image for the texture. */
  loc = (GLubyte*) floorTexture;
  for (t = 0; t < 16; t++) {
    for (s = 0; s < 16; s++) {
      if (circles[t][s] == 'x') {
        // opaque part
        loc[0] = 0xff;
        loc[1] = 0xff;
        loc[2] = 0x00;
        loc[3] = 0xff;
      } else {
        // grey alpha part. this shouldn't be visible
        loc[0] = 0xaa;
        loc[1] = 0xaa;
        loc[2] = 0xaa;
        loc[3] = 0x00;
      }
      loc += 4;
    }
  }
  glGenTextures  (1, &texName);
  glBindTexture  (GL_TEXTURE_2D, texName);
  glPixelStorei  (GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, floorTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvf      (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

static void render(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_ACCUM_BUFFER_BIT);
  
  glBegin(GL_TRIANGLES);
  //glColor3f(1.0, 0.0, 0.0);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1.0, -1.0, 0.0);
  
  //glColor3f(0.0, 1.0, 0.0);
  glTexCoord2f(1.0, 0.0);
  glVertex3f( 0.0,  1.0, 0.0);
  
  //glColor3f(0.0, 0.0, 1.0);
  glTexCoord2f(0.0, 1.0);
  glVertex3f( 1.0, -1.0, 0.0);
  glEnd();
}

int main(void) {
  GLFWwindow *window;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(640, 480, "OpenGL Boilerplate", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  init_texture();
  
  while (!glfwWindowShouldClose(window)) {
    render(window);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
