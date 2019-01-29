#ifndef SCENE_HANDLER_HPP
#define SCENE_HANDLER_HPP

#include <memory>
#include <stack>

#include "Scene.hpp"

class Scene;

class SceneHandler : public sf::NonCopyable
{
public:

    SceneHandler();
    ~SceneHandler();

    void init();

    void addScene(std::unique_ptr<Scene> newScene, bool replace = true);
    void popScene();

    void applySceneRequests();

    Scene& activeScene();

    bool exitRequest() const;

private:

    std::stack< std::unique_ptr<Scene> > _scenes;

    bool _addRequest;
    bool _replace;
    std::unique_ptr<Scene> _newScene;
    bool _popRequest;

    bool _exitRequest;
};

#endif