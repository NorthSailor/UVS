#include "UVSWindow.h"
#include <FV/Common.h>
#include <FV/Model.h>
#include <thread>
#include <assert.h>
#include <chrono>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace std;
using namespace FV;
using namespace std::chrono;
using namespace std::this_thread;

UVSWindow::UVSWindow(bool fullscreen) :
    SDLWindow("Universal Vehicle Simulator - Pre-alpha", fullscreen)
{
}

void UVSWindow::Initialize()
{
    glClearColor(0.6f, 0.5f, 0.4f, 1.0f);
    m_f16 = make_shared<Model>("Resources/F-16/F-16C_FightingFalcon.obj", m_loader);
    auto start = high_resolution_clock::now();
    m_loader.FinishLoading();
    auto end = high_resolution_clock::now();
    Log(INFO, "Total model loading time %.3fs.",
        0.001f * duration_cast<milliseconds>(end - start).count());
    m_program = m_loader.LoadProgram("model.glsl");
    m_program->Use();
    m_renderingUniforms.transform = m_program->GetUniform("ModelTransform");
    m_renderingUniforms.diffuseSampler = m_program->GetUniform("diffuseSampler");

    glm::mat4 view = glm::lookAt(glm::vec3(2, 12, -2),
                                 glm::vec3(0, 0, -1),
                                 glm::vec3(0, 0, 1));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            1.5f, 1.0f, 10000.0f);

    m_program->GetUniform("ViewProjection").Set(projection * view);
}

void UVSWindow::UpdateLoop()
{
    while (!m_shouldQuit) {
        sleep_for(milliseconds(50));
    }
}

void UVSWindow::Render(double, float)
{
    static float totalTime = 0.0f;
    static int count = 0;
    auto start = high_resolution_clock::now();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_program->Use();
    m_f16->Draw(m_program, m_renderingUniforms);
    glDisable(GL_BLEND);
    auto end = high_resolution_clock::now();

    totalTime += duration_cast<microseconds>(end - start).count() * 0.001f;
    count++;
    if (count == 10) {
        float average = totalTime / count;
        Log(INFO, "Average frame time: %.3f", average);
        count = 0;
        totalTime = 0;
    }

    assert(glGetError() == 0);
}
