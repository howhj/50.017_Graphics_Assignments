#include "texture.hpp"
#include "bitmap_image.hpp"

template<typename T>
T clamp_new(const T& v, const T& lower_range, const T& upper_range)
{
    if (v < lower_range)
        return lower_range;
    else if (v >  upper_range)
        return upper_range;
    else
        return v;
}

void
Texture::load(const char * filename)
{
	bimg=new bitmap_image(filename);
	height = bimg->height();
    width = bimg->width();
}

void
Texture::operator()(int x, int y,  unsigned char * color)
{
    x = clamp_new(x,0,width-1);
    y = clamp_new(y,0,height-1);
    bimg->get_pixel(x,y,color[0],color[1],color[2]);
}
bool Texture::valid()
{
	return bimg!=0;
}
///@param x assumed to be between 0 and 1
vec3
Texture::operator()(float x, float y)
{
	vec3 color;
    int ix,iy;
    x=x*width;
    y=(1-y)*height;
    ix = (int)x;
    iy = (int)y;
    unsigned char pixels[4][3];
    float alpha = x-ix;
    float beta = y-iy;
    operator()(ix  ,iy  ,pixels[0]);
    operator()(ix+1,iy  ,pixels[1]);
    operator()(ix  ,iy+1,pixels[2]);
    operator()(ix+1,iy+1,pixels[3]);
    for(int ii=0;ii<3;ii++){
      color[ii] = (1-alpha)*(1-beta)*pixels[0][ii]
                +    alpha *(1-beta)*pixels[1][ii]
                + (1-alpha)*   beta *pixels[2][ii]
                +    alpha *   beta *pixels[3][ii];
    }
	return vec3 (float(color[0]/float(255)), float(color[1]/float(255)), float(color[2]/float(255)));
}

Texture::~Texture()
{
	if(bimg!=0){
		delete bimg;
	}
}

Texture::Texture():bimg(0),width(0),height(0)
{
}

