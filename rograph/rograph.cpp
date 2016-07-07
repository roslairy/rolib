#include "rograph.h"

#include <iostream>
#include <map>

#include "glew.h"
#include "glfw3.h"
#include "SOIL.h"
#include "ProgramBuilder.h"
#include "glm\mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace rograph {

using namespace std;
using namespace glm;

// indicate a vector in 2-dimision
typedef struct _Vector {
    float x, y;
} Vector, *PVector;

// indicate a square
typedef struct _Position {
    Vector zero;
    Vector right;
    Vector vert;
    Vector top;
} Position;

// an object used to store paint data
typedef struct _PaintObj {
    Position pos;
    Position texCoord;
    mat4 backZero;
    mat4 rotate;
    mat4 translate;
    float opacity;
    int texture;
    Vector size;
    Vector anchor;
} PaintObj;

///////////////////////////////////////////
//
//	global vars
//
///////////////////////////////////////////

int viewportWidth = 0;
int viewportHeight = 0;
mat4 ort;

map<unsigned, PaintObj*> paintObjs;
vector<unsigned> textures;

GLuint backZeroLoc = 0;
GLuint rotateLoc = 0;
GLuint translateLoc = 0;
GLuint opacityLoc = 0;
GLuint texLoc = 0;
GLuint orthoLoc = 0;

GLuint vbo = 0;
GLuint vao = 0;

GLFWwindow *window = NULL;

const Position initialPos = {
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f }
};

const Position fixedTexCoord = {
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f }
};

const Vector initialSize = { 1.0f, 1.0f };
const Vector initialAnchor = { 0.0f, 0.0f };

void initOpengl(int width, int height);
void createFrame(const char *title, int width, int height);

void error_callback(int error, const char* description){
    puts(description);
}

// initialize opengl
void init(const char *title, int vWidth, int vHeight) {
    viewportWidth = vWidth;
    viewportHeight = vHeight;
    createFrame(title, vWidth, vHeight);
    glfwSetErrorCallback(error_callback);
    initOpengl(vWidth, vHeight);
}

// release all resources
void release() {
    // release paint objects
    for each (auto elem in paintObjs) {
        delete paintObjs[elem.first];
    }

    // release opengl
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(textures.size(), &textures[0]);

    // release glfw
    glfwTerminate();
}

GLuint loadTexture(const char *path) {
    unsigned tex = SOIL_load_OGL_texture
    (
        path,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (tex == 0) {
        printf("WARNING: Failed to load texture of %s", path);
        return 0;
    }
    textures.push_back(tex);
    return tex;
}

unsigned createPaintObj() {
    PaintObj *obj = new PaintObj;
    obj->pos = initialPos;
    obj->texCoord = fixedTexCoord;
    obj->backZero = mat4(1.0f);
    obj->rotate = mat4(1.0f);
    obj->translate = mat4(1.0f);
    obj->opacity = 1.0f;
    obj->texture = 0;
    obj->size = initialSize;
    obj->anchor = initialAnchor;
    unsigned idx = 0;
    if (paintObjs.size() > 0) {
        idx = paintObjs.rbegin()->first + 1;
    }
    paintObjs[idx] = obj;
    return idx;
}

void destroyPaintObj(unsigned paintObj) {
    delete paintObjs[paintObj];
    paintObjs.erase(paintObj);
}

// used to rebuild backZero and pos in PaintObj
void rebuildMatrixAfterResizeAndReanchor(PaintObj *obj);

void setSize(unsigned paintObj, unsigned width, unsigned height) {
    PaintObj* obj = paintObjs[paintObj];
    obj->size = Vector{ (float)width, (float)height };
    rebuildMatrixAfterResizeAndReanchor(obj);
}

void setAnchor(unsigned paintObj, float xAnchor, float yAnchor) {
    if (xAnchor > 1.0f || xAnchor < 0.0f) return;
    if (yAnchor > 1.0f || yAnchor < 0.0f) return;
    PaintObj* obj = paintObjs[paintObj];
    obj->anchor = Vector{ xAnchor, yAnchor };
    rebuildMatrixAfterResizeAndReanchor(obj);
}

void setPosition(unsigned paintObj, int x, int y) {
    PaintObj* obj = paintObjs[paintObj];
    obj->translate = translate(mat4(1.0f),
        vec3((float)x, (float)y, 0.0f));
}

void setRotate(unsigned paintObj, float degree) {
    PaintObj* obj = paintObjs[paintObj];
    obj->rotate = rotate(mat4(1.0f), radians(degree), vec3(0.0f, 0.0f, 1.0f));
}

void setTexture(unsigned paintObj, unsigned texture) {
    PaintObj* obj = paintObjs[paintObj];
    obj->texture = texture;
}

void setOpacity(unsigned paintObj, float opacity) {
    if (opacity > 1.0f || opacity < 0.0f) return;
    PaintObj* obj = paintObjs[paintObj];
    obj->opacity = opacity;
}

void clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void paint(unsigned paintObj) {
    PaintObj* obj = paintObjs[paintObj];
    glBindTexture(GL_TEXTURE_2D, obj->texture);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(Position), &obj->pos, GL_DYNAMIC_DRAW);
    glUniformMatrix4fv(backZeroLoc, 1, GL_FALSE, &obj->backZero[0][0]);
    glUniformMatrix4fv(rotateLoc, 1, GL_FALSE, &obj->rotate[0][0]);
    glUniformMatrix4fv(translateLoc, 1, GL_FALSE, &obj->translate[0][0]);
    glUniform1f(opacityLoc, obj->opacity);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void updateFrame() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool isFrameClosed() {
    return glfwWindowShouldClose(window);
}

void rebuildMatrixAfterResizeAndReanchor(PaintObj *obj) {
    obj->pos = {
        { 0.0f, 0.0f },
        { obj->size.x, 0.0f },
        { obj->size.x, obj->size.y },
        { 0.0f, obj->size.y }
    };
    obj->backZero = translate(mat4(1.0f),
        vec3(-obj->size.x * obj->anchor.x, -obj->size.y * obj->anchor.y, 0.0f));
}

void initOpengl(int width, int height) {
    // init glew
    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize glew!" << endl;
    }

    // init initial settings
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build shaders
    ProgramBuilder builder;
    builder.addShader(GL_VERTEX_SHADER, "./shaders/rograph.vert");
    builder.addShader(GL_FRAGMENT_SHADER, "./shaders/rograph.frag");
    GLuint program = builder.build();
    glUseProgram(program);
    glDeleteProgram(program);

    // get location of uniforms and attributes
    backZeroLoc = glGetUniformLocation(program, "backZero");
    rotateLoc = glGetUniformLocation(program, "rotate");
    translateLoc = glGetUniformLocation(program, "translate");
    opacityLoc = glGetUniformLocation(program, "opacity");
    texLoc = glGetUniformLocation(program, "tex");
    glUniform1i(texLoc, 0);

    // create fundamental ortho matrix and apply it
    orthoLoc = glGetUniformLocation(program, "ortho");
    ort = ortho(0.0f, (float)width, 0.0f, (float)height);
    glUniformMatrix4fv(orthoLoc, 1, GL_FALSE, &ort[0][0]);

    // create vbo and vao
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // indicate use of buffer data
    GLuint ivPosLoc = glGetAttribLocation(program, "ivPos");
    glVertexAttribPointer(ivPosLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(ivPosLoc);
    GLuint ivTexLoc = glGetAttribLocation(program, "ivTex");
    glVertexAttribPointer(ivTexLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(Position));
    glEnableVertexAttribArray(ivTexLoc);
}

void createFrame(const char *title, int width, int height) {
    if (!glfwInit()) {
        printf("ERROR: Failed to init glfw.");
        return;
    }
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 16);
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        printf("ERROR: Failed to create window.");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

} // namespace rogrph

namespace roinput {

bool getKeyPress(int key) {
    return GLFW_PRESS == glfwGetKey(rograph::window, key);
}

bool getKeyRelease(int key) {
    return GLFW_RELEASE == glfwGetKey(rograph::window, key);
}

bool getMousePress(int button) {
    return GLFW_PRESS == glfwGetMouseButton(rograph::window, button);
}

bool getMouseRelease(int button) {
    return GLFW_RELEASE == glfwGetMouseButton(rograph::window, button);
}

void getCursorPos(int *x, int *y) {
    double dx, dy;
    glfwGetCursorPos(rograph::window, &dx, &dy);
    *x = static_cast<int>(dx);
    *y = rograph::viewportHeight - static_cast<int>(dy);
}

} // namespace roinput