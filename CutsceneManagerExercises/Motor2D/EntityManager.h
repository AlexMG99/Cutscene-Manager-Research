#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Module.h"
#include "p2Point.h"

class Entity;

class EntityManager : public Module
{
public:
	std::list<Entity*> entities;
	EntityManager();
	~EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PreUpdate();
	bool PostUpdate();

	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	Entity* CreateEntity(fPoint position, std::string path);
	bool DeleteEntity(Entity* entity);

	pugi::xml_document	animations_doc;
private:
	pugi::xml_node entity_configs;

	int id_count = 0;
	bool draw_path = false;
};

#endif //_ENTITYMANAGER_H_