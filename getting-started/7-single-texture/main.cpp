#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gld/gld.h>
#include <stb/stb_image.h>

#include <fstream>
#include <iostream>
#include <sstream>

int main() {
  glfwSetErrorCallback([](int error_code, const char *description) {
    std::cout << "GLFW ERROR " << error_code << ": " << description
              << std::endl;
  });
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE)
#endif

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  gldViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window,
                                 [](GLFWwindow *window, int width, int height) {
                                   gldViewport(0, 0, width, height);
                                 });

  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode,
                                int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
      gldPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
      gldPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
  });

  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    vShaderFile.open(CONTENT_SOURCE_DIR "colored-texture.vs.glsl");
    fShaderFile.open(CONTENT_SOURCE_DIR "colored-texture.fs.glsl");
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
  }
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();
  unsigned int vertex, fragment, program;
  vertex = gldCreateShader(GL_VERTEX_SHADER);
  gldShaderSource(vertex, 1, &vShaderCode, NULL);
  gldCompileShader(vertex);
  {
    int success;
    char infoLog[512];
    gldGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      gldGetShaderInfoLog(vertex, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    }
  }
  fragment = gldCreateShader(GL_FRAGMENT_SHADER);
  gldShaderSource(fragment, 1, &fShaderCode, NULL);
  gldCompileShader(fragment);
  {
    int success;
    char infoLog[512];
    gldGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      gldGetShaderInfoLog(fragment, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    }
  }
  program = gldCreateProgram();
  gldAttachShader(program, vertex);
  gldAttachShader(program, fragment);
  gldLinkProgram(program);
  {
    int success;
    char infoLog[512];
    gldGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
      gldGetProgramInfoLog(fragment, 512, NULL, infoLog);
      std::cout << "ERROR::PROGRAM::LINK\n" << infoLog << std::endl;
    }
  }
  gldUseProgram(program);
  gldDeleteShader(vertex);
  gldDeleteShader(fragment);

  unsigned int texture;
  glGenTextures(1, &texture);

  float vertices[] = {
      // positions   // texture coords
      0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  unsigned int VAO;
  gldGenVertexArrays(1, &VAO);

  unsigned int EBO;
  gldGenBuffers(1, &EBO);

  unsigned int VBO;
  gldGenBuffers(1, &VBO);

  gldBindVertexArray(VAO);
  gldBindBuffer(GL_ARRAY_BUFFER, VBO);
  gldBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  gldBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  gldBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                GL_STATIC_DRAW);
  gldVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                         (void *)0);
  gldEnableVertexAttribArray(0);
  gldVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                         (void *)(6 * sizeof(float)));
  gldEnableVertexAttribArray(1);

  unsigned int texture1;
  gldGenTextures(1, &texture1);
  gldBindTexture(GL_TEXTURE_2D, texture1);
  gldTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  gldTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  gldTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  gldTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(CONTENT_SOURCE_DIR "sir-arowhun-of-jenn-kellon.png", &width,
                &height, &nrChannels, 0);
  if (data) {
    gldTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                  GL_UNSIGNED_BYTE, data);
    gldGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    gldClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    gldClear(GL_COLOR_BUFFER_BIT);

    gldActiveTexture(GL_TEXTURE0);
    gldBindTexture(GL_TEXTURE_2D, texture1);

    gldUseProgram(program);
    gldUniform1i(gldGetUniformLocation(program, "ourTexture"), 0);
    gldBindVertexArray(VAO);
    gldDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    gldBindVertexArray(0);
    gldUseProgram(0);

    gldActiveTexture(GL_TEXTURE0);
    gldBindTexture(GL_TEXTURE_2D, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}