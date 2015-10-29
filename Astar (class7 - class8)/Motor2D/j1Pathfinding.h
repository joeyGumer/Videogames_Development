#ifndef _J1PATHFINDING_H_
#define _J1PATHFINDING_H_

#include "j1Module.h"
#include "p2List.h"

struct PathNode
{
	PathNode(PathNode* node)
	{
		parent = node;
	}
	//parent node
	PathNode*	parent;
	//position coordinates
	int			x;
	int			y;
	
	//important data of the node:
	//g: cost to arrive to this node (distance run form init)
	//h: cost to arrive to the goal
	//f: g + h, so, the number of steps to do if going trough this node
	float		g;
	float		h;
	float		f;
};


class j1Pathfinding : public j1Module
{
public:
	// Constructor
	j1Pathfinding();

	// Destructor
	virtual ~j1Pathfinding();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);
	//Have to think how to do it with the update functions
	// Called before all Updates
	/*bool PreUpdate();*/
	// Called before quitting
	bool CleanUp();

	// A* pathfinding algorithm
	bool AStar(PathNode* start, PathNode* goal);

public:
	p2List<PathNode*> open_nodes;
	p2List<PathNode*> close_nodes;
};

#endif _J1PATHFINDING_H_