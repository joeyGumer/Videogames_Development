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


bool j1Pathfinding::CreatePath(iPoint& start, iPoint& goal)
{
	bool ret = false;

	PathNode* way;
	way = new PathNode(start, NULL);

	PathNode* finish;
	finish = new PathNode(goal, NULL);
	
	open_nodes.add(way);
	
	while (open_nodes.start)
	{
		p2List_item<PathNode*>* item = open_nodes.start;
		p2List_item<PathNode*>* node = open_nodes.start;
		PathNode* parent = NULL;
		
		//cicle to get the open node with the lowest f and the highest h
		while (item)
		{
			if (item->data->f < node->data->f)
				node->data = item->data;

			/*else if (item->data->f == node->data->f)
			{
				if (item->data->h > node->data->h)
					node->data = item->data;
			}*/
		}

		//delete the node from open list and add it to close list
		parent = node->data;
		
		open_nodes.del(node);
		

		//create each child
		for (int i = 0; i < 4; i++)
		{
			PathNode* child;
			child = new PathNode(parent->pos ,parent);
			

			switch (i)
			{
			case 0:
				child->pos.x++;
				break;
			case 1:
				child->pos.y++;
				break;
			case 2:
				child->pos.x--;
				break;
			case 3:
				child->pos.y--;
				break;
			}

			//this only works for 4 directions
			child->g = parent->g ++;

			//I use the absolute value because if negative, it would give antoher numbers
			child->h = abs(goal.x - child->pos.x) + abs(goal.y - child->pos.y);
			child->f = child->g + child->h;

			//if child is the goal, stop the child creating cicle, we find it!
			if (child->pos == goal)
			{
				//has to make something here
				finish.parent = child->parent;
				ret = true;
				break;
			}

			//TODO : change the PathNode position int x,y to a point, it will be a lot easier to make equivalences
			//Also, after make it , follow the instructions mentioned at the PDF
			//I can simplify a lot the code using diferent functions
			//Review the uses of new, i'm confused about when it's useful to use it and when not;

			//Checks if the node is alredy created at the lists
			item = open_nodes.start;
			while (item)
			{
				if (item->data->pos == child->pos)
					delete child;

				item = item->next;
			}

			item = close_nodes.start;
			while (item)
			{
				if (item->data->pos == child->pos)
					delete child;

				item = item->next;
			}

			//Add child to the open  list if don't skipped
			//maybe i can check here if the child is the goal
			if (finish.parent)
				delete child;
			
				open_nodes.add(child);
		}
			close_nodes.add(parent);
	}
	
	/*Have to do this the good way, it sttores the path nodes to the path list
	if (ret)
	{
		p2List_item<PathNode*>* item = open_nodes.start;
		while (finish->pos != way->pos)
		{
			item
			path.add(finish)

		}
		path.add
	}
	*/
	return ret;
		
}
	
