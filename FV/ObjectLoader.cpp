#include "ObjectLoader.h"
#include "Common.h"
#include <fstream>
#include <sstream>
using namespace FV;
using namespace std;

ObjectLoader::ObjectLoader(std::string path) :
    m_path(path),
    m_shaderPath("Shaders/")
{
}

shared_ptr<Texture2D> ObjectLoader::LoadTexture2D(string path)
{
    // Search if the texture already exists
    auto searchResult = m_textures2D.find(path);
    if (searchResult != m_textures2D.end() &&
            searchResult->second.expired() == false) {
        // The texture is already loaded.
        return searchResult->second.lock();
    } else {
        // The texture must be loaded.
        shared_ptr<Texture2D> texture = make_shared<Texture2D>();
        // Add it to the lists
        Tex2DIterator it = m_textures2D.emplace(path, texture).first;
        m_texLoadingCount++;
        thread textureThread(&ObjectLoader::ProcessTexture, this, it);
        textureThread.detach();
        return texture;
    }
}

shared_ptr<Program> ObjectLoader::LoadProgram(string path)
{
    // Search if the program is already loaded.
    auto searchResult = m_programs.find(path);
    if (searchResult != m_programs.end() &&
            searchResult->second.expired() == false) {
        // The program is already loaded.
        return searchResult->second.lock();
    } else {
        // We need to load the program.
        vector<string> v;
        v.push_back(path);
        return LoadProgram(v);
    }
}

shared_ptr<Program> ObjectLoader::LoadProgram(vector<string> paths)
{
        string vShaderCode, fShaderCode;
        stringstream programCode;

        for (string path : paths) {
            ifstream file;
            file.open(m_shaderPath + path);
            if (file.is_open() == false) {
                throw Exception("Failed to open program file: " + path);
            }
            programCode << file.rdbuf();
            file.close();
            programCode << "\n#COMMON\n";
        }

        enum Dest { BOTH, VERTEX, FRAGMENT };
        Dest dest = BOTH;
        for (string line; getline(programCode, line); ) {
            if (line == "#VERTEX") {
                dest = VERTEX;
                continue;
            } else if (line == "#FRAGMENT") {
                dest = FRAGMENT;
                continue;
            } else if (line == "#COMMON") {
                dest = BOTH;
                continue;
            }

            if (dest == VERTEX || dest == BOTH) {
                vShaderCode.append(line + "\n");
            }
            if (dest == FRAGMENT || dest == BOTH) {
                fShaderCode.append(line + "\n");
            }
        }

        shared_ptr<Program> prg =
                make_shared<Program>(vShaderCode, fShaderCode);

        return prg;
}

void ObjectLoader::FinishLoading()
{
    unique_lock<mutex> lk(m_texMutex);
    while (m_texLoadingCount > 0)
        m_texCV.wait(lk);
}

void ObjectLoader::ProcessTexture(Tex2DIterator it)
{
    shared_ptr<Texture2D> texture = it->second.lock();
    texture->LoadImage2D(m_path + it->first, Texture2D::RGBA);
    lock_guard<mutex> lk(m_texMutex);
    m_texLoadingCount--;
    m_texCV.notify_all();
}
