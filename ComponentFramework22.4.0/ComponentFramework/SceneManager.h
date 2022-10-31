#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

//imGui
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <string>
class SceneManager  {
public:
	
	SceneManager();
	~SceneManager();
	void Run();
	bool Initialize(std::string name_, int width_, int height_);
	void HandleEvents();
	
	
private:
	enum class SCENE_NUMBER {
		SCENE0 = 0,
		SCENE1,
		SCENE1V2,
		SCENE2,
		SCENE3,
		SCENE4,
		SCENE5,
		SCENE6
	};

	class Scene* currentScene;
	//class Timer* timer;
	class Window* window;

	/*unsigned int fps;
	bool isRunning;
	bool fullScreen;*/
	void BuildNewScene(SCENE_NUMBER scene_);
};


#endif // SCENEMANAGER_H