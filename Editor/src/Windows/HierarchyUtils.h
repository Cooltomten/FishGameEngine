#pragma once
#include "ComponentSystem/Entity.h"

#include <vector>

namespace 
{
	void SortChildren(std::vector<std::shared_ptr<Comp::Entity>>& someEntities)
	{
		std::vector<std::shared_ptr<Comp::Entity>> copyOfScene;


		std::vector<std::shared_ptr<Comp::Entity>> children;
		for (int i = 0; i < someEntities.size(); i++)
		{
			if (std::find(copyOfScene.begin(), copyOfScene.end(), someEntities[i]) != copyOfScene.end() || someEntities[i]->GetParent())
			{
				continue;
			}
			copyOfScene.push_back(someEntities[i]);
			children = someEntities[i]->GetChildren();
			while (children.size() > 0)
			{
				auto child = *children.begin();
				copyOfScene.push_back(child);
				int index = 1;
				for (auto c : child->GetChildren())
				{
					auto it = children.begin() + index;
					children.insert(it, c);
					index++;
				}
				children.erase(children.begin());

			}
		}
		someEntities = copyOfScene;
	}
}