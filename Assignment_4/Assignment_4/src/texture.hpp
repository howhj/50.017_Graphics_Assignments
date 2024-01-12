#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#define GLM_ENABLE_EXPERIMENTAL
#include "bitmap_image.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;
///@brief helper class that stores a texture and faciliates lookup
///assume 4byte RGBA image data
class Texture{
public:
  Texture();
  bool valid();
  void load(const char * filename);
  void operator()(int x, int y,  unsigned char * color);
  ///@param x assumed to be between 0 and 1
  vec3 operator()(float x, float y);
  ~Texture();
private:
  bitmap_image * bimg;
  int width , height;
};
#endif
