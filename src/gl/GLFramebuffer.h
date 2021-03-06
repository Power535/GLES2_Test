#pragma once

#include "ContextBase.h"
#include "GLTexture2D.h"

class GLFramebuffer : public IBindableObject, public IChangeableObject, public IGLObject
{
	GLuint glId;

	struct Attachment {
		GLenum point;
		std::shared_ptr<GLTexture2D> texture;
	};
	std::vector<Attachment> attachments;
	std::vector<GLenum> discards;

#ifdef USE_OPENGL_ES
    static GLuint quadShader;
    static GLint quadVertexAttribute;
    static GLint quadTexCoordAttribute;
    static GLint quadTexSamplerUniform;
    static const char * vertexCode;
    static const char * fragmentCode;
    static float quadVertices[];
    static float quadTexCoords[];
#endif

public:
	GLFramebuffer(std::shared_ptr<ContextBase> & context);

	bool attach(const GLenum attachmentPoint, std::shared_ptr<GLTexture2D> & texture);
	//bool attach(const GLenum attachmentPoint, std::shared_ptr<GLTextureDepth> & texture);
	const Attachment & getAttachment(const GLenum attachmentPoint) const;

	GLsizei getWidth() const;
	GLsizei getHeight() const;
	GLuint getId() const;

	bool blitTo(std::shared_ptr<GLFramebuffer> & to, const GLbitfield mask = GL_COLOR_BUFFER_BIT, const GLenum filter = GL_LINEAR);
	bool blitToScreen(const int width, const int height, const GLbitfield mask = GL_COLOR_BUFFER_BIT, const GLenum filter = GL_LINEAR);
    bool blitFromScreen(const int width, const int height, const GLbitfield mask = GL_COLOR_BUFFER_BIT, const GLenum filter = GL_LINEAR);

	bool discard() const;

	bool bind(std::shared_ptr<ParameterBase> parameter = nullptr) override;
	bool unbind(std::shared_ptr<ParameterBase> parameter = nullptr) override;

	~GLFramebuffer();
};

//------------------------------------------------------------------------------------------------------

class GLFrameBufferException : public GLException
{
public:
	GLFrameBufferException(const char * errorString) throw();
	GLFrameBufferException(const std::string & errorString) throw();
};
