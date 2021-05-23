#include <iostream>
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <OgreFrameListener.h>

class MyTestApp : public OgreBites::ApplicationContext, public OgreBites::InputListener {
public:
    MyTestApp();
    void setup();
    bool keyPressed(const OgreBites::KeyboardEvent& evt);
    bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
    bool frameRenderingQueued(const Ogre::FrameEvent& fe);
private:
    float x, y, z;
    Ogre::SceneNode* camNode;
};


int main(int argc, char *argv[]) {
    std::cout << "MyTestApp app" << std::endl;
    // Instanciamos un objeto MyTestApp
    // El mismo es un objeto que extiende `OgreBites::ApplicationContext`, porque será el motor de rendering
    // y también OgreBites::InputListener, porque podrá atender entradas del usuario (teclado y mouse)
    MyTestApp app;
    std::cout << "init" << std::endl;
    // Inicializa el motor, llama a this->setup() y luego inicializa el loop de eventos
    app.initApp();
    std::cout << "startRendering" << std::endl;
    // Inicia el loop de eventos y rendering
    app.getRoot()->startRendering();
    std::cout << "closeApp" << std::endl;
    // Una vez que finalizó la aplicación, libera recursos
    // (Sí, es más RAII moverlo al destructor)
    app.closeApp();
    std::cout << "exit" << std::endl;
    return 0;
}




MyTestApp::MyTestApp() : OgreBites::ApplicationContext("OgreTutorialApp"), x(0), y(0), z(10) {
}


// Cada vez que se renderiza un cuadro, atiendo eventos
bool MyTestApp::frameRenderingQueued(const Ogre::FrameEvent& fe) {
    this->pollEvents();
    return true;
}

// Atiendo eventos de mouse
bool MyTestApp::mouseMoved(const OgreBites::MouseMotionEvent& evt) {
    return true;
}

// Atiendo eventos de teclado
bool MyTestApp::keyPressed(const OgreBites::KeyboardEvent& evt) {
    switch(evt.keysym.sym) {
        case OgreBites::SDLK_ESCAPE:
            getRoot()->queueEndRendering();
        break;
        case 'q':
            this->x -= 0.1;
        break;
        case 'w':
            this->x += 0.1;
        break;
        case 'a':
            this->y -= 0.1;
        break;
        case 's':
            this->y += 0.1;
        break;
        case 'z':
            this->z -= 0.1;
        break;
        case 'x':
            this->z += 0.1;
        break;
    }
    this->camNode->setPosition(this->x, this->y, this->z);
    return true;
}

//! [setup]
void MyTestApp::setup(void) {
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();
    
    // register for input events
    addInputListener(this);

    // get a pointer to the already created root
    Ogre::Root* root = getRoot();
    root->addFrameListener(this);
    Ogre::SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // without light we would just get a black screen    
    Ogre::Light* light = scnMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 10);
    lightNode->attachObject(light);

    // also need to tell where we are
    this->camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(this->x, this->y, this->z);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    // create the camera
    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(1); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);

    // and tell it to render into the main window
    getRenderWindow()->addViewport(cam);

    // finally something to render
    Ogre::Entity* ent = scnMgr->createEntity("Hello.mesh");
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(ent);
    std::cout << "Setup ready" << std::endl;
}
