#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "Interfaces.hpp"
#include "Matrix.hpp"
#include <iostream>

#define STRINGIFY(A)  #A

#include "../Shaders/Simple.vert"
#include "../Shaders/Simple.frag"

struct Drawable
{
    GLuint vertexBuffer;
    GLuint indexBuffer;
    int indexCount;
};

class RenderingEngine : public IRenderingEngine
{
public:
    RenderingEngine();
	
    void initialize(const vector<ISurface*>& surfaces);
    void render(const vector<Visual>& visuals) const;
	
private:
    GLuint buildShader(const char* source, GLenum shaderType) const;
    GLuint buildProgram(const char* vShader, const char* fShader) const;
	
    vector<Drawable> _drawables;
	GLuint _renderBuffer;
    
    GLint _projectionUniform;
    GLint _modelviewUniform;
    mat4 _translation;

    GLuint _positionSlot;
    GLuint _colorSlot;
};

IRenderingEngine* IRenderingEngine::createRenderingEngine()
{
    return new RenderingEngine();
}

RenderingEngine::RenderingEngine()
{
    glGenRenderbuffers(1, &_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
}

void RenderingEngine::initialize(const vector<ISurface*>& surfaces)
{
    vector<ISurface*>::const_iterator surface;
	
    for (surface = surfaces.begin(); surface != surfaces.end(); ++surface) {
        // Create the VBO for the vertices.
        vector<float> vertices;
        (*surface)->generateVertices(vertices);
		
        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
        
        // Create a new VBO for the indices if needed.
        int indexCount = (*surface)->getLineIndexCount();
        GLuint indexBuffer;

        if (!_drawables.empty() && indexCount == _drawables[0].indexCount) {
            indexBuffer = _drawables[0].indexBuffer;
        }
		else {
            vector<GLushort> indices(indexCount);
            (*surface)->generateLineIndices(indices);
			
            glGenBuffers(1, &indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
        }
        
        Drawable drawable = { vertexBuffer, indexBuffer, indexCount};
        _drawables.push_back(drawable);
    }
    
    // Create the framebuffer object.
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
    
    // Create the GLSL program.
    GLuint simpleProgram = buildProgram(SimpleVertexShader, SimpleFragmentShader);
    glUseProgram(simpleProgram);
    _positionSlot = glGetAttribLocation(simpleProgram, "Position");
    _colorSlot = glGetAttribLocation(simpleProgram, "SourceColor");
    glEnableVertexAttribArray(_positionSlot);

    // Set up some matrices.
    _translation = mat4::translate(0, 0, -7);
    _projectionUniform = glGetUniformLocation(simpleProgram, "Projection");
    _modelviewUniform = glGetUniformLocation(simpleProgram, "Modelview");
}

void RenderingEngine::render(const vector<Visual>& visuals) const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
        
    vector<Visual>::const_iterator visual = visuals.begin();
    for (int visualIndex = 0; visual != visuals.end(); ++visual, ++visualIndex) {
        // Set the viewport transform.
        ivec2 size = visual->viewportSize;
        ivec2 lowerLeft = visual->lowerLeft;
        glViewport(lowerLeft.x, lowerLeft.y, size.x, size.y);

        // Set the model-view transform.
        mat4 rotation = visual->orientation.toMatrix();
        mat4 modelview = rotation * _translation;
        glUniformMatrix4fv(_modelviewUniform, 1, 0, modelview.pointer());

        // Set the projection transform.
        float h = 4.0f * size.y / size.x;
        mat4 projectionMatrix = mat4::frustum(-2, 2, -h / 2, h / 2, 5, 10);
        glUniformMatrix4fv(_projectionUniform, 1, 0, projectionMatrix.pointer());
        
        // Set the color.
        vec3 color = visual->color;
        glVertexAttrib4f(_colorSlot, color.x, color.y, color.z, 1);
        
        // Draw the wireframe.
        const Drawable& drawable = _drawables[visualIndex];
        glBindBuffer(GL_ARRAY_BUFFER, drawable.vertexBuffer);
        glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.indexBuffer);
        glDrawElements(GL_LINES, drawable.indexCount, GL_UNSIGNED_SHORT, 0);
    }
}

GLuint RenderingEngine::buildShader(const char* source, GLenum shaderType) const
{
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &source, 0);
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        std::cout << messages;
        exit(1);
    }
    
    return shaderHandle;
}

GLuint RenderingEngine::buildProgram(const char* vertexShaderSource, const char* fragmentShaderSource) const
{
    GLuint vertexShader = buildShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = buildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        std::cout << messages;
        exit(1);
    }
    
    return programHandle;
}