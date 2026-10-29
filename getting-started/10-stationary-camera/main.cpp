#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gld/gld.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include <fstream>
#include <iostream>
#include <sstream>

constexpr unsigned SCR_WIDTH = 800;
constexpr unsigned SCR_HEIGHT = 600;

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
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

  gldViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

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
    vShaderFile.open(CONTENT_SOURCE_DIR "simple-mvp.vs.glsl");
    fShaderFile.open(CONTENT_SOURCE_DIR "simple-mvp.fs.glsl");
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
      gldGetProgramInfoLog(program, 512, NULL, infoLog);
      std::cout << "ERROR::PROGRAM::LINK\n" << infoLog << std::endl;
    }
  }
  gldUseProgram(program);
  gldDeleteShader(vertex);
  gldDeleteShader(fragment);

  // clang-format off
  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
  
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };
  // world space positions of our cubes
  glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
  };
  // clang-format on

  unsigned int VAO;
  gldGenVertexArrays(1, &VAO);

  unsigned int VBO;
  gldGenBuffers(1, &VBO);

  gldBindVertexArray(VAO);
  gldBindBuffer(GL_ARRAY_BUFFER, VBO);
  gldBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  gldVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                         (void *)0);
  gldEnableVertexAttribArray(0);
  gldVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                         (void *)(3 * sizeof(float)));
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
  {
    unsigned char *data =
        stbi_load(CONTENT_SOURCE_DIR "sir-arowhun-of-jenn-kellon.png", &width,
                  &height, &nrChannels, 4);
    if (data) {
      gldTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, data);
      gldGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
  }

  gldEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    glm::mat4 view = glm::mat4(
        1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 projection = glm::mat4(1.0f);
    projection =
        glm::perspective(glm::radians(45.0f),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    gldClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    gldClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gldActiveTexture(GL_TEXTURE0);
    gldBindTexture(GL_TEXTURE_2D, texture1);

    gldUseProgram(program);
    gldUniform1i(gldGetUniformLocation(program, "texture1"), 0);
    gldUniformMatrix4fv(gldGetUniformLocation(program, "projection"), 1,
                        GL_FALSE, glm::value_ptr(projection));
    gldUniformMatrix4fv(gldGetUniformLocation(program, "view"), 1, GL_FALSE,
                        glm::value_ptr(view));
    gldBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      gldUniformMatrix4fv(gldGetUniformLocation(program, "model"), 1, GL_FALSE,
                          glm::value_ptr(model));
      gldDrawArrays(GL_TRIANGLES, 0, 36);
    }
    gldBindVertexArray(0);
    gldUseProgram(0);

    gldActiveTexture(GL_TEXTURE0);
    gldBindTexture(GL_TEXTURE_2D, 0);
    gldActiveTexture(GL_TEXTURE1);
    gldBindTexture(GL_TEXTURE_2D, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  gldDeleteVertexArrays(1, &VAO);
  gldDeleteBuffers(1, &VBO);
  gldDeleteTextures(1, &texture1);
  gldDeleteProgram(program);

  glfwTerminate();
  return 0;
}