#ifndef _ROGEN_ROGRAPH_H_
#define _ROGEN_ROGRAPH_H_

namespace rograph {

// initialize opengl
void init(const char *title, int width, int height);

// release all resources
void release();

// load a texture from buffer, return TBO
unsigned loadTexture(const char *path);
unsigned loadTexture(unsigned char *buffer, int size);

void unloadTexture(unsigned texture);

// create a paint obj
unsigned createPaintObj();

// release a paint obj
void destroyPaintObj(unsigned paintObj);

// change attr of paint obj
void setSize(unsigned paintObj, unsigned width, unsigned height);
void setAnchor(unsigned paintObj, float xAnchor, float yAnchor);
void setPosition(unsigned paintObj, int x, int y);
void setRotate(unsigned paintObj, float degree);
void setTexture(unsigned paintObj, unsigned texture);
void setOpacity(unsigned paintObj, float opacity);

// clear the pain area
void clear();

// paint a paint obj
void paint(unsigned paintObj);

// update frame to show all drawed picture
void updateFrame();

// check if frame has been closed by user
bool isFrameClosed();

} // namespace rograph

#endif
