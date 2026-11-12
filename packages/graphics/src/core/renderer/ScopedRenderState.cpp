#include <graphics/core/renderer/ScopedRenderState.h>
#include <glad/glad.h>

ScopedRenderState::ScopedRenderState()
{
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFrameBuffer);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFrameBuffer);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
    glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
    glActiveTexture(GL_TEXTURE0);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &cubeTexture);
    glGetIntegerv(GL_DEPTH_FUNC, &depthFunction);
    glGetIntegerv(GL_CULL_FACE_MODE, &cullFace);
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthWrite);
    glGetDoublev(GL_DEPTH_CLEAR_VALUE, &clearDepth);
    depth = glIsEnabled(GL_DEPTH_TEST);
    blend = glIsEnabled(GL_BLEND);
    cull = glIsEnabled(GL_CULL_FACE);
    srgb = glIsEnabled(GL_FRAMEBUFFER_SRGB);
}

ScopedRenderState::~ScopedRenderState()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFrameBuffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, readFrameBuffer);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    glUseProgram(program);
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
    glActiveTexture(activeTexture);
    glDepthFunc(depthFunction);
    glDepthMask(depthWrite);
    glClearDepth(clearDepth);
    glCullFace(cullFace);
    depth ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    cull ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    srgb ? glEnable(GL_FRAMEBUFFER_SRGB) : glDisable(GL_FRAMEBUFFER_SRGB);
}
