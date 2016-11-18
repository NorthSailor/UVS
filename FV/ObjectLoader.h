#ifndef FV_OBJECTLOADER_H
#define FV_OBJECTLOADER_H

#include <map>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Texture.h"
#include "Program.h"

namespace FV {

/**
 * An object responsible for loading game assets. It mainly has two important
 * features:
 * 1) If an object (except for multiple-file programs), has been already loaded,
 * a shared pointer to that object will be returned.
 * 2) When a texture is loaded, a thread is created to load it in an
 * 	  asynchronous manner. A call to FinishLoading() ensures that all textures
 *    have finished loading and are available.
 * All resources are given relative to an initial path, given to the loader's
 * constructor.
 */
class ObjectLoader
{
public:
    /**
     * Creates a new object loader.
     * Normally this should be done only once upon loading.
     * @param path The path where the assets are located.
     */
    ObjectLoader(std::string path = "Resources/");

    /**
     * Starts loading a 2D texture from a storage location.
     * @param path Path to the texture file relative to the resource path.
     * @return A shared pointer to the texture object.
     * @note Make sure that FinishLoading() is called before you attempt to
     * 		 use the texture.
     */
    std::shared_ptr<Texture> LoadTexture2D(std::string path);

    /**
     * Loads a program from a single program file.
     * @param path Path to the program file relative to the resource path.
     * @return A shared pointer to the program object.
     */
    std::shared_ptr<Program> LoadProgram(std::string path);

    /**
     * Loads a program from several program files.
     * @param paths Vector containing the relative paths to the files.
     * @return A shared pointer to the program object.
     * @note Due to the virtually unlimited combinations, this method does
     * 		 not check whether the program has already been loaded,
     * 		 unlike LoadProgram() with a single argument.
     */
    std::shared_ptr<Program> LoadProgram(std::vector<std::string> paths);

    /**
     * Waits for all asynchronous texture loading operations to complete and
     * then returns. Blocks the calling thread.
     */
    void FinishLoading();
    
    /**
     * Returns the absolute (or relative to the working directory) path of a
     * resource.
     * The path relative to the resource folder.
     */
    inline std::string GetResourcePath(std::string path) {
        return m_path + path;
    }

protected:
    std::string m_path;
    std::string m_shaderPath;

    std::map<std::string, std::weak_ptr<Texture>> m_textures;
    std::map<std::string, std::weak_ptr<Program>> m_programs;

    typedef std::map<std::string, std::weak_ptr<Texture>>::iterator
        TexIterator;
    std::queue<TexIterator> m_texturesToLoad;

    void ProcessTexture(TexIterator it);

    int m_texLoadingCount = 0;
    std::mutex m_texMutex;
    std::condition_variable m_texCV;
};


}


#endif // FV_OBJECTLOADER_H
