
#include "SceneHandler.hpp"

#include "Scene_Menu.hpp"

SceneHandler::SceneHandler()
{
    _addRequest = false;
    _replace = false;
    _popRequest = false;
    _exitRequest = false;
}

SceneHandler::~SceneHandler() {}

void SceneHandler::init()
{
    applySceneRequests();
}

void SceneHandler::addScene(std::unique_ptr<Scene> newScene, bool replace)
{
    _addRequest = true;
    _replace = replace;
    _newScene = std::move(newScene);
}

void SceneHandler::popScene()
{
    _popRequest = true;
}

void SceneHandler::applySceneRequests()
{
    if(_addRequest)
    {
        if(!_scenes.empty())
        {
            if(_replace)
            {
                _scenes.pop();
            }
            else
            {
                _scenes.top()->pause();
            }
        }

        _scenes.push(std::move(_newScene));

        _scenes.top()->init();

        _addRequest = false;
    }

    if(_popRequest)
    {
        if(!_scenes.empty())
        {
            _scenes.pop();

            if(!_scenes.empty())
            {
                _scenes.top()->resume();
            }
        }

        if(_scenes.empty()) _exitRequest = true;

        _popRequest = false;
    }
}

Scene& SceneHandler::activeScene()
{
    return *_scenes.top();
}

bool SceneHandler::exitRequest() const
{
    return _exitRequest;
}