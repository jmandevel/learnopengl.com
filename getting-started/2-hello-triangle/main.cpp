#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gld/gld.h>

#include <iostream>

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE)
#endif

      glfwSetErrorCallback([](int error_code, const char *description) {
        std::cout << "GLFW ERROR " << error_code << ": " << description
                  << std::endl;
      });

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode,
                                int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
      gldPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
      gldPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
  });

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

  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";

  int vertexShader;
  vertexShader = gldCreateShader(GL_VERTEX_SHADER);
  gldShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  gldCompileShader(vertexShader);

  {
    int success;
    char infoLog[512];
    gldGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      gldGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    }
  }

  const char *fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "\n"
      "void main()\n"
      "{\n"
      "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}";

  int fragmentShader;
  fragmentShader = gldCreateShader(GL_FRAGMENT_SHADER);
  gldShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  gldCompileShader(fragmentShader);

  {
    int success;
    char infoLog[512];
    gldGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      gldGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    }
  }

  unsigned int shaderProgram;
  shaderProgram = gldCreateProgram();

  gldAttachShader(shaderProgram, vertexShader);
  gldAttachShader(shaderProgram, fragmentShader);
  gldLinkProgram(shaderProgram);

  {
    int success;
    char infoLog[512];
    gldGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      gldGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::PROGRAM::LINK\n" << infoLog << std::endl;
    }
  }

  gldUseProgram(shaderProgram);

  gldDeleteShader(vertexShader);
  gldDeleteShader(fragmentShader);

  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  unsigned int VAO;
  gldGenVertexArrays(1, &VAO);

  unsigned int VBO;
  gldGenBuffers(1, &VBO);

  gldBindVertexArray(VAO);
  gldBindBuffer(GL_ARRAY_BUFFER, VBO);
  gldBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  gldVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                         (void *)0);
  gldEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    gldClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    gldClear(GL_COLOR_BUFFER_BIT);

    gldUseProgram(shaderProgram);
    gldBindVertexArray(VAO);
    gldDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}