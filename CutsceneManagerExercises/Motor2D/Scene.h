#ifndef _SCENE_H_
#define _SCENE_H_

#include "PugiXml\src\pugixml.hpp"
#include "UIElement.h"

//Interface for all Scenes in Project
enum GUI_Event;

class Scene
{
public:

	Scene() {};

	// Destructor
	~Scene() {};

	// Called before the first frame
	virtual bool Start() { return true; };

	// Called before all Updates
	virtual bool PreUpdate() { return true; };

	// Called each loop iteration
	virtual bool Update(float dt) { return true; };

	// Called before all Updates
	virtual bool PostUpdate() { return true; };

	// Called before quitting
	virtual bool CleanUp() { return true; };

	virtual bool Load(pugi::xml_node&) { return true; };

	virtual bool Save(pugi::xml_node&) const { return true; };

	virtual bool GUIEvent(UIElement*, GUI_Event) { return true; };
};

#endif // _SCENE_H_
