#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include <GL/glew.h>
#include <string>
#include <memory>

namespace FV {

/**
 * Encapsulates an OpenGL texture of any type.
 */
class Texture
{
public:
    enum Target {
        TEX_2D = GL_TEXTURE_2D,
        TEX_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE
    };

    enum Format {
        RGBA8 = GL_RGBA8,
        RGBA8_SNORM = GL_RGBA8_SNORM,
        RGB10_A2 = GL_RGB10_A2,
        RGB10_A2UI = GL_RGB10_A2UI,
        RGBA12 = GL_RGBA12,
        RGBA16 = GL_RGBA16,
        RGBA16_SNORM = GL_RGBA16_SNORM,
        SRGB8 = GL_SRGB8,
        SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
        R16F = GL_R16F,
        RG16F = GL_RG16F,
        RGB16F = GL_RGB16F,
        RGBA16F = GL_RGBA16F,
        R32F = GL_R32F,
        RG32F = GL_RG32F,
        RGB32F = GL_RGB32F,
        RGBA32F = GL_RGBA32F,
        RGB9_E5 = GL_RGB9_E5,
        R8I = GL_R8I,
        R8UI = GL_R8UI,
        R16I = GL_R16I,
        R16UI = GL_R16UI,
        R32I = GL_R32I,
        R32UI = GL_R32UI,
        RG8I = GL_RG8I,
        RG8UI = GL_RG8UI,
        RG16I = GL_RG16I,
        RG16UI = GL_RG16UI,
        RG32I = GL_RG32I,
        RG32UI = GL_RG32UI,
        RGB8I = GL_RGB8I,
        RGB8UI = GL_RGB8UI,
        RGB16I = GL_RGB16I,
        RGB16UI = GL_RGB16UI,
        RGB32I = GL_RGB32I,
        RGB32UI = GL_RGB32UI,
        RGBA8I = GL_RGBA8I,
        RGBA8UI = GL_RGBA8UI,
        RGBA16I = GL_RGBA16I,
        RGBA16UI = GL_RGBA16UI,
        RGBA32I = GL_RGBA32I,
        RGBA32UI = GL_RGBA32UI,
        DEPTH = GL_DEPTH_COMPONENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL,
        DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
        RED = GL_RED,
        GREEN = GL_GREEN,
        BLUE = GL_BLUE,
        RG = GL_RG,
        RGB = GL_RGB,
        RGBA = GL_RGBA,
        BGR = GL_BGR,
        BGRA = GL_BGRA,
        BGRA_INTEGER = GL_BGRA_INTEGER,
        RED_INTEGER	 = GL_RED_INTEGER,
        GREEN_INTEGER = GL_GREEN_INTEGER,
        BLUE_INTEGER = GL_BLUE_INTEGER,
        RG_INTEGER = GL_RG_INTEGER,
        RGB_INTEGER = GL_RGB_INTEGER,
        RGBA_INTEGER = GL_RGBA_INTEGER,
        BGR_INTEGER = GL_BGR_INTEGER,
        STENCIL_INDEX = GL_STENCIL_INDEX,
        COMPRESSED_RED = GL_COMPRESSED_RED,
        COMPRESSED_RG = GL_COMPRESSED_RG,
        COMPRESSED_RGB = GL_COMPRESSED_RGB,
        COMPRESSED_RGBA = GL_COMPRESSED_RGBA,
        COMPRESSED_SRGB = GL_COMPRESSED_SRGB,
        COMPRESSED_SRGB_ALPHA = GL_COMPRESSED_SRGB_ALPHA
    };

    enum Type {
        BYTE = GL_BYTE,
        SHORT = GL_SHORT,
        INT = GL_INT,
        UBYTE = GL_UNSIGNED_BYTE,
        USHORT = GL_UNSIGNED_SHORT,
        UINT = GL_UNSIGNED_INT,
        FLOAT = GL_FLOAT,
        HALF_FLOAT = GL_HALF_FLOAT,
        UINT24_8 = GL_UNSIGNED_INT_24_8
    };

    enum FilteringMethod {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR,
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
        LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
    };

    enum WrapMode {
        REPEAT = GL_REPEAT,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
    };

    /**
     * Creates a new texture object of the given type.
     * @param target
     */
    Texture(Target target = TEX_2D);
    /**
     * Destroyes the underlying GL object.
     */
    ~Texture();

    /**
     * Binds the texture to the given texture unit.
     * @param unit Must be less than the maximum number of texture units.
     */
    void Bind(int unit = 0);

    /**
     * Releases the texture unit at which the texture is bound.
     * An exception is thrown if the texture is not bound.
     * @note Subject to removal.
     */
    void ReleaseUnit();

    //! Basically calls glGenerateMipmap() on the texture.
    //! @note The texture must be bound with Bind().
    void GenerateMipmaps() const;

    /** Gets the underlying OpenGL ID. */
    inline GLuint GetID() const {
        return m_ID;
    }

    //! Gets the texture's target.
    inline Target GetTarget() const {
        return m_target;
    }

    bool operator==(const Texture& texture);

    // 2D textures

    /**
     * Sets the texture filtering mode.
     * @param minifying The minifying filtering mode (scaling down).
     * @param magnifying The magnifying filtering mode (scaling up).
     * @note The texture must be bound.
     */
    void SetFiltering(FilteringMethod minifying, FilteringMethod magnifying);

    /**
     * Sets the wrapping mode for the texture.
     * @param s Wrapping mode for the s axis.
     * @param t Wrapping mode for the t axis.
     * @note The texture must be bound.
     */
    void SetWrap(WrapMode s, WrapMode t);

    /**
     * Sets the wrapping mode for the texture.
     * @param s Wrapping mode for the s axis.
     * @param t Wrapping mode for the t axis.
     * @param r Wrapping mode for the r axis.
     * @note The texture must be bound.
     */
    void SetWrap(WrapMode s, WrapMode t, WrapMode r);

    /**
     * Sets the contents of the texture.
     * The data won't be send to the GPU until right before the texture is
     * used. This allows you to load the data in a separate thread.
     * @see SetImage2D()
     * @param width The width of the texture in texels.
     * @param height The height of the texture in texels.
     * @param data Pointer to the data to be copied.
     * @param iformat The internal format of the texture.
     * @param type The type of the pixel components.
     * @param format The format of the data.
     * @note The data will be freed by the class when no longer needed.
     */
    void SetImage2DAsync(int width, int height, void *data,
                         Format iformat = RGBA, Type type = UBYTE,
                         Format format = RGBA);
    /**
     * Sets the contents of the texture.
     * The data is immediately send to the GPU. This means that you need to
     * call this function from your main OpenGL thread.
     * @see SetImage2DAsync()
     * @param width The width of the texture in texels.
     * @param height The height of the texture in texels.
     * @param data Pointer to the data to be copied.
     * @param iformat The internal format of the texture.
     * @param type The type of the pixel components.
     * @param format The format of the data.
     * @param border The texture's border color.
     * @note The texture is also bound on unit 0.
     */
    void SetImage2D(int width, int height, void *data,
                    Format iformat = RGBA, Type type = UBYTE,
                    Format format = RGBA, int border = 0);

    /**
     * Loads a texture from the given filename.
     * @param filename Path to the texture file.
     * @param format Format of the file's contents.
     */
    void LoadImage2D(std::string filename, Format format = RGBA);

    /**
     * Sets the multisample image parameters.
     * @param width The width of the texture in texels.
     * @param height The height of the texture in texels.
     * @param samples The number of samples to use.
     * @param iformat The internal format of the texture.
     * @note The texture is bound at unit 0.
     */
    void SetImage2DMultisample(int width, int height, int samples = 8,
                               Format iformat = RGBA);

protected:
    GLuint m_ID;
    Target m_target;
    int m_currentUnit;
    bool m_final;
    void *m_data = nullptr;
    Format m_iformat = RGBA;
    Type m_type = UBYTE;
    Format m_format = RGBA;
    int m_width;
    int m_height;
    bool m_hasMipmaps;

    virtual void Finalize();
    void SendToGPU();
};

}

#endif // GL_TEXTURE_H
