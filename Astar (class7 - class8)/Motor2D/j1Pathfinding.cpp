#include "j1Pathfinding.h"
#include <math.h>


j1Pathfinding::j1Pathfinding() : j1Module()
{
	name.create("pathfinding");
}

// Destructor
j1Pathfinding::~j1Pathfinding()
{}


bool j1Pathfinding::Awake(pugi::xml_node& node)
{
	return true;
}

bool j1Pathfinding::CleanUp()
{
	return true;
}


bool j1Pathfinding::AStar(PathNode* start, PathNode* goal)
{
	bool ret = false;

	open_nodes.add(start);
	
	while (open_nodes.start)
	{
		p2List_item<PathNode*>* item = open_nodes.start;
		p2List_item<PathNode*>* node = open_nodes.start;
		PathNode* parent = NULL;
		
		while (item)
		{
			if (item->data->f < node->data->f)
				node->data = item->data;
		}

		parent = node->data;
		open_nodes.del(node);

		for (int i = 0; i < 4; i++)
		{
			PathNode* child;
			child = new PathNode(parent);

			switch (i)
			{
			case 0:
				child->x = parent->x++;
				child->y = parent->y;
				break;
			case 1:
				child->x = parent->x;
				child->y = parent->y++;
				break;
			case 2:
				child->x = parent->x--;
				child->y = parent->y;
				break;
			case 3:
				child->x = parent->x;
				child->y = parent->y--;
				break;
			}

			if (child->x == goal->x && child->y == goal->y)
			{
				ret = true;
			}

			//this only works for 4 directions
			child->g = parent->g ++;

			//I use the absolute value because if negative, it would give antoher numbers
			child->h = abs(goal->x - child->x) + abs(goal->y - child->y);
			child->f = child->g + child->h;

			//TODO : change the PathNode position int x,y to a point, it will be a lot easier to make equivalences
			//Also, after make it , follow the instructions mentioned at the PDF
			//I can simplify a lot the code using diferent functions
			//Review the uses of new, i'm confused about when it's useful to use it and when not;
		}
		
	}


	return ret;
}