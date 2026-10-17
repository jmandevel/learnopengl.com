#pragma once

#include <glad/glad.h>
#include <gld/gld.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  unsigned int ID;

  inline Shader(const char *vertexPath, const char *fragmentPath) : ID(0) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
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
    unsigned int vertex, fragment;
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
    this->ID = gldCreateProgram();
    gldAttachShader(this->ID, vertex);
    gldAttachShader(this->ID, fragment);
    gldLinkProgram(this->ID);
    {
      int success;
      char infoLog[512];
      gldGetProgramiv(this->ID, GL_LINK_STATUS, &success);
      if (!success) {
        gldGetProgramInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINK\n" << infoLog << std::endl;
      }
    }
    gldUseProgram(this->ID);
    gldDeleteShader(vertex);
    gldDeleteShader(fragment);
  }

  inline void use() { gldUseProgram(this->ID); }

  inline void setBool(const std::string &name, bool value) const {
    gldUniform1i(gldGetUniformLocation(this->ID, name.c_str()), (int)value);
  }

  inline void setInt(const std::string &name, int value) const {
    gldUniform1i(gldGetUniformLocation(this->ID, name.c_str()), value);
  }

  inline void setFloat(const std::string &name, float value) const {
    gldUniform1f(gldGetUniformLocation(this->ID, name.c_str()), value);
  }
};