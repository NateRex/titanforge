#include <boost/test/unit_test.hpp>
#include <graphics/core/renderer/ScopedRenderState.h>
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <glad/glad.h>
#include <stdexcept>

namespace
{
    GLint integerState(GLenum name)
    {
        GLint value = 0;
        glGetIntegerv(name, &value);
        return value;
    }

    struct Bindings
    {
        GLuint frameBuffers[2] = {};
        GLuint vertexArrays[2] = {};
        GLuint textures[2] = {};

        Bindings()
        {
            glGenFramebuffers(2, frameBuffers);
            glGenVertexArrays(2, vertexArrays);
            glGenTextures(2, textures);
        }

        ~Bindings()
        {
            glDeleteFramebuffers(2, frameBuffers);
            glDeleteVertexArrays(2, vertexArrays);
            glDeleteTextures(2, textures);
        }
    };
}

/**
 * Restores framebuffer bindings, the viewport, shader, VAO, and texture-unit state.
 */
BOOST_AUTO_TEST_CASE(ScopedRenderState_restoresBindings)
{
    Bindings bindings;
    ScopedRenderState restore;
    auto shader = ShaderManager::getShader(ShaderId::SHADOW_DEPTH);
    shader->activate();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, bindings.frameBuffers[0]);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, bindings.frameBuffers[1]);
    glBindVertexArray(bindings.vertexArrays[0]);
    glViewport(3, 5, 17, 19);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bindings.textures[0]);
    glActiveTexture(GL_TEXTURE3);
    {
        ScopedRenderState state;
        // Capturing unit zero's binding deliberately leaves that unit active.
        BOOST_TEST(integerState(GL_ACTIVE_TEXTURE) == GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bindings.textures[1]);
        glActiveTexture(GL_TEXTURE2);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, bindings.frameBuffers[1]);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, bindings.frameBuffers[0]);
        glBindVertexArray(bindings.vertexArrays[1]);
        glUseProgram(0);
        glViewport(0, 0, 1, 1);
    }
    BOOST_TEST(integerState(GL_DRAW_FRAMEBUFFER_BINDING) == bindings.frameBuffers[0]);
    BOOST_TEST(integerState(GL_READ_FRAMEBUFFER_BINDING) == bindings.frameBuffers[1]);
    BOOST_TEST(integerState(GL_CURRENT_PROGRAM) == shader->id());
    BOOST_TEST(integerState(GL_VERTEX_ARRAY_BINDING) == bindings.vertexArrays[0]);
    BOOST_TEST(integerState(GL_ACTIVE_TEXTURE) == GL_TEXTURE3);
    glActiveTexture(GL_TEXTURE0);
    BOOST_TEST(integerState(GL_TEXTURE_BINDING_2D) == bindings.textures[0]);
    GLint viewport[4] = {};
    glGetIntegerv(GL_VIEWPORT, viewport);
    const GLint expected[] = {3, 5, 17, 19};
    BOOST_CHECK_EQUAL_COLLECTIONS(viewport, viewport + 4, expected, expected + 4);
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}

/**
 * Restores enabled render flags and their configured settings after leaving scope.
 */
BOOST_AUTO_TEST_CASE(ScopedRenderState_restoresSettings)
{
    ScopedRenderState restore;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glDepthFunc(GL_GREATER);
    glDepthMask(GL_TRUE);
    glClearDepth(0.25);
    glCullFace(GL_FRONT);
    {
        ScopedRenderState state;
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glDisable(GL_FRAMEBUFFER_SRGB);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        glClearDepth(0.75);
        glCullFace(GL_BACK);
    }
    BOOST_TEST(glIsEnabled(GL_DEPTH_TEST) == GL_TRUE);
    BOOST_TEST(glIsEnabled(GL_BLEND) == GL_TRUE);
    BOOST_TEST(glIsEnabled(GL_CULL_FACE) == GL_TRUE);
    BOOST_TEST(glIsEnabled(GL_FRAMEBUFFER_SRGB) == GL_TRUE);
    BOOST_TEST(integerState(GL_DEPTH_FUNC) == GL_GREATER);
    BOOST_TEST(integerState(GL_CULL_FACE_MODE) == GL_FRONT);
    GLboolean depthWrite = GL_FALSE;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthWrite);
    BOOST_TEST(depthWrite == GL_TRUE);
    GLdouble clearDepth = 0.0;
    glGetDoublev(GL_DEPTH_CLEAR_VALUE, &clearDepth);
    BOOST_TEST(clearDepth == 0.25);
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}

/**
 * Nested scopes restore the immediately enclosing state, then the original state.
 */
BOOST_AUTO_TEST_CASE(ScopedRenderState_nestedScopes)
{
    ScopedRenderState restore;
    glDepthFunc(GL_LESS);
    {
        ScopedRenderState outer;
        glDepthFunc(GL_GREATER);
        {
            ScopedRenderState inner;
            glDepthFunc(GL_ALWAYS);
        }
        BOOST_TEST(integerState(GL_DEPTH_FUNC) == GL_GREATER);
    }
    BOOST_TEST(integerState(GL_DEPTH_FUNC) == GL_LESS);
}

/**
 * State restoration also runs when an exception leaves the protected scope.
 */
BOOST_AUTO_TEST_CASE(ScopedRenderState_exceptionUnwinding)
{
    ScopedRenderState restore;
    glDepthFunc(GL_GEQUAL);
    glActiveTexture(GL_TEXTURE3);
    auto fail = []() {
        ScopedRenderState state;
        glDepthFunc(GL_NEVER);
        glActiveTexture(GL_TEXTURE2);
        throw std::runtime_error("Leave render scope");
    };
    BOOST_CHECK_THROW(fail(), std::runtime_error);
    BOOST_TEST(integerState(GL_DEPTH_FUNC) == GL_GEQUAL);
    BOOST_TEST(integerState(GL_ACTIVE_TEXTURE) == GL_TEXTURE3);
}
