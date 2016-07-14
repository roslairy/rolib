#ifndef _ROGEN_ROGRAPH_H_
#define _ROGEN_ROGRAPH_H_

/**
* API for Graphic
* Use these functions to create window, load textures and paint them on window.
*/
namespace rograph {

/**
* Initialize OpenGL and create a window.
* @param title title of window
* @param width width of window
* @param height height of window
*/
void init(const char *title, int width, int height);

/**
* Uninitialize OpenGL and close window.
*/
void release();

/**
* Load texture from file.
* @param path path of picture file
* @return handle of texture
*/
unsigned loadTexture(const char *path);

/**
* Load texture from memory.
* @param buffer buffer of picture
* @param size size of buffer
* @return handle of texture
*/
unsigned loadTexture(unsigned char *buffer, int size);

/**
* Unload texture.
* @param texture handle of texture
*/
void unloadTexture(unsigned texture);

/**
* Create a new paint-object for painting.
* @return handle of paint-object
*/
unsigned createPaintObj();

/**
* Destroy an existing paint-object.
* @param paintObj handle of paint-object
*/
void destroyPaintObj(unsigned paintObj);

/**
* Set size for a paint-object.
* @param paintObj handle of paint-object
* @param width width of paint-object
* @param height height of paint-object
*/
void setSize(unsigned paintObj, unsigned width, unsigned height);

/**
* Set anchor for a paint-object, anchor range is [0, 1].
* @param paintObj handle of paint-object
* @param xAnchor x-coordinate of anchor
* @param yAnchor y-coordinate of anchor
*/
void setAnchor(unsigned paintObj, float xAnchor, float yAnchor);

/**
* Set position for a paint-object.
* @param paintObj handle of paint-object
* @param x x-coordinate of paint-object
* @param y y-coordinate of paint-object
*/
void setPosition(unsigned paintObj, int x, int y);

/**
* Set rotation for a paint-object.
* @param paintObj handle of paint-object
* @param degree degree of rotation
*/
void setRotate(unsigned paintObj, float degree);

/**
* Set texture for a paint-object.
* @param paintObj handle of paint-object
* @param texture handle of texture
*/
void setTexture(unsigned paintObj, unsigned texture);

/**
* Set opacity for a paint-object.
* @param paintObj handle of paint-object
* @param opacity opacity of paint-object
*/
void setOpacity(unsigned paintObj, float opacity);

/**
* Clear the window with black pixel.
*/
void clear();

/**
* Paint a paint-object on the window.
* @param paintObj handle of paint-object
*/
void paint(unsigned paintObj);

/**
* Swap the buffer of window, so you can see what you painted.
* At the same time, it will poll window event and wait for vsync.
*/
void updateFrame();

/**
* Check if the close-button of the window has been pressed.
* @return if the window is closed
*/
bool isFrameClosed();

} // namespace rograph

#endif
