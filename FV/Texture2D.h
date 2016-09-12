#ifndef FV_TEXTURE2D_H
#define FV_TEXTURE2D_H

#include "Texture.h"
#include <string>

namespace FV {

/**
 * Represents a 2D opengl texture.
 */
class Texture2D : public Texture
{
public:
    /**
     * Creates a new texture.
     */
    Texture2D();

    /**
     * Sets the contents of the texture.
     * @param width The width of the texture in texels.
     * @param height The height of the texture in texels.
     * @param data Pointer to the data to be copied.
     * @param iformat The internal format of the texture.
     * @param type The type of the pixel components.
     * @param format The format of the data.
     * @note The data will be freed by the class when no longer needed.
     */
    void SetImage2D(int width, int height, void *data,
                    Format iformat = RGBA, Type type = UBYTE,
                    Format format = RGBA);

    /**
     * Loads a texture from the given filename.
     * @param filename Path to the texture file.
     * @param format Format of the file's contents.
     */
    void LoadImage2D(std::string filename, Format format = RGBA);


protected:
    void *m_data = nullptr;
    Format m_iformat = RGBA;
    Type m_type = UBYTE;
    Format m_format = RGBA;
    int m_width;
    int m_height;
    void SendToGPU();
    virtual void Finalize();
};

}

#endif // FV_TEXTURE2D_H
