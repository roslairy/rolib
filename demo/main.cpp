#include <rograph.h>
#include <roaudio.h>
#include <roinput.h>
#pragma comment(lib, "rolib.lib")

#include <stdio.h>

int main() {
    roaudio::init();
    unsigned ao = roaudio::createAudioObj();
    unsigned ogg = roaudio::loadOgg(".\\tt.ogg");
    roaudio::setOggAudioObj(ao, ogg);
    roaudio::setLoopAudioObj(ao, true);
    roaudio::setVolumeAudioObj(ao, 1.0f);
    roaudio::play(ao);

    rograph::init("Test frame", 800, 600);
    unsigned po = rograph::createPaintObj();
    unsigned tex = rograph::loadTexture(".\\test.JPG");
    rograph::setTexture(po, tex);
    rograph::setSize(po, 300, 300);
    rograph::setAnchor(po, 0.5, 0.5);
    rograph::setPosition(po, 400, 300);
    unsigned po1 = rograph::createPaintObj();
    unsigned tex1 = rograph::loadTexture(".\\test1.jpg");
    rograph::setTexture(po1, tex1);
    rograph::setSize(po1, 800, 600);
    float degree = 0;
    int x, y;
    int cnt = 0;
    while (!rograph::isFrameClosed()) {
        if (roinput::getKeyPress(roinput::KEY_Q)) break;
        if (cnt == 120) roaudio::pause(ao);
        else if (cnt == 240) roaudio::play(ao);
        else if (cnt == 360) roaudio::stop(ao);
        else if (cnt == 480) roaudio::play(ao);
        cnt++;
        roinput::getCursorPos(&x, &y);
        printf("x:%d\ty:%d\n", x, y);
        rograph::setRotate(po, degree);
        degree += 0.3f;
        rograph::clear();
        rograph::paint(po1);
        rograph::paint(po);
        rograph::updateFrame();
    }
    rograph::release();
    roaudio::release();
}