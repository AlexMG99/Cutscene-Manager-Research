#include "j1App.h"
#include "p2Log.h"
#include "Render.h"
#include "Fonts.h"
#include "GUI.h"
#include "EntityManager.h"
#include "CutsceneManager.h"
#include "CutsceneMoveCamera.h"
#include "CutsceneMoveEntity.h"
#include "CutsceneModifyText.h"
#include "CutsceneModifyImage.h"
#include "CutsceneEntity.h"
#include "CutsceneMap.h"
#include "CutsceneText.h"
#include "CutsceneImage.h"

CutsceneManager::CutsceneManager()
{
	name = "cutscene_manager";
}

bool CutsceneManager::Awake(pugi::xml_node &config)
{
	LOG("Starting Cutscene Manager:");
	

	return true;
}

bool CutsceneManager::Update(float dt)
{
	//TODO8: Uncomment this code, which basically starts the Cutscene and manage it
	// It is the main function that allows everything works
	//ExecuteCutscene(dt);

	return true;
}

bool CutsceneManager::CleanUp()
{
	App->entity_manager->CleanUp();
	App->gui->CleanUp();
	App->fonts->CleanUp();
	return true;
}


void CutsceneManager::PlayCutscene(std::string path)
{
	if (!is_executing) 
	{
		LoadCutscene(path);
		is_executing = true;
	}
}

bool CutsceneManager::LoadCutscene(std::string path)
{

	bool ret = false;

	// Here we load the xml file
	pugi::xml_parse_result result = cutscene_file.load_file(path.c_str());

	if (result == NULL)
		LOG("Could not load map xml file cutscene.xml. Pugi error: %s", result.description());
	else
	{
		ret = true;

		CutsceneAction* cutscene_action = nullptr;

		// The node is pointing to the first action node of the XML.
		for (pugi::xml_node cutscene_action_node = cutscene_file.first_child().child("actions").child("cutscene"); cutscene_action_node; cutscene_action_node = cutscene_action_node.next_sibling())
		{
			std::string action = cutscene_action_node.attribute("action").as_string();

			uint start = cutscene_action_node.child("time").attribute("start").as_uint();
			uint duration = cutscene_action_node.child("time").attribute("duration").as_uint();

			if (action == "move_camera")
			{
				cutscene_action = new CutsceneMoveCamera(start, duration, cutscene_action_node.child("time").attribute("speed_x").as_int(), cutscene_action_node.child("time").attribute("speed_y").as_int());
			}
			else if (action == "modify_text")
			{
				cutscene_action = new CutsceneModifyText(start, duration,
					cutscene_action_node.attribute("name").as_string(),
					cutscene_action_node.child("time").attribute("type").as_string(),
					cutscene_action_node.child("time").attribute("text").as_string());
			}
			else if (action == "modify_image")
			{
				cutscene_action = new CutsceneModifyImage(start, duration,
					cutscene_action_node.attribute("name").as_string(),
					cutscene_action_node.child("time").attribute("type").as_string());
			}
			// TODO6: Create a new CutsceneMoveEntity that will be load from the xml
			// Check CutsceneMoveEntity for more information

			actions.push_back(cutscene_action);
		}

		CutsceneElement* cutscene_element = nullptr;

		// The node is pointing to the first element node of the XML.
		for (pugi::xml_node cutscene_element_node = cutscene_file.first_child().child("elements").child("element"); cutscene_element_node; cutscene_element_node = cutscene_element_node.next_sibling())
		{
			std::string type = cutscene_element_node.attribute("type").as_string();
			std::string name = cutscene_element_node.attribute("name").as_string();

			if (type == "map")
			{
				cutscene_element = new CutsceneMap(cutscene_element_node.attribute("path").as_string());
			}
			else if (type == "text")
			{
				cutscene_element = new CutsceneText(cutscene_element_node.attribute("pos_x").as_int(),
					cutscene_element_node.attribute("pos_y").as_int(),
					cutscene_element_node.attribute("txt").as_string(),
					cutscene_element_node.attribute("size").as_uint(),
					cutscene_element_node.attribute("active").as_bool(true)
				);
			}
			//TODO4: Create a new CutsceneEntity element which will be load from the xml.
			// Check CutsceneEntity constructor for more information.

			//TODO5: Create a new CutsceneImage element which will be load from the xml.
			// Check CutsceneImage constructor for more information.

			cutscene_element->active = cutscene_element_node.attribute("active").as_bool(true);


			elements.insert(std::pair <std::string, CutsceneElement*>(name, cutscene_element));
		}
	}

	return ret;
}

void CutsceneManager::ExecuteCutscene(float dt)
{

	if (is_executing)
	{
		// The cutscene_timer starts, and item points to the first item of the action list
		if (start) {
			cutscene_timer.Start();

			item = actions.begin();
			start = false;
		}

		// Checks if the actions has finished and goes to the next action in the case that is finished.
		if ((cutscene_timer.ReadMs() - (*item)->start_time) > (*item)->duration_time)
		{
			++item;
		}

		// This calls all the functions 
		if (item != actions.end())
		{
			if (cutscene_timer.ReadMs() > (*item)->start_time)
				(*item)->Execute(dt);
		}
		else
		{
			is_executing = false;
			start = true;
			ClearCutscene();
		}
	}
}

void CutsceneManager::ClearCutscene()
{
	for (std::vector<CutsceneAction*>::iterator it = actions.begin(); it != actions.end(); ++it)
	{
		delete (*it);
	}

	actions.clear();

	for (std::map<std::string, CutsceneElement*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		(*it).second->~CutsceneElement();
		delete (*it).second;
	}

	elements.clear();

	App->entity_manager->CleanUp();

	App->render->camera = { 0, -200 };
}

double CutsceneManager::GetTimer()
{
	return cutscene_timer.ReadMs();
}
