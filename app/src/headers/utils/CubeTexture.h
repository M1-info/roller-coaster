#pragma once
#ifndef CUBETEXTURE_H
#define CUBETEXTURE_H

#include "glad/glad.h"
#include "stb_image/stb_image.h"
#include <string>
#include <vector>

class CubeTexture
{
public:
    CubeTexture(std::vector<std::string> files);
    ~CubeTexture();

    void Bind();
    void Unbind();

    inline void SetFiles(std::vector<std::string> files) { m_Files = files; }

private:
    GLuint m_TextureID;
    std::vector<std::string> m_Files;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BitsPerPixel;
};


#endif // CUBETEXTURE_H