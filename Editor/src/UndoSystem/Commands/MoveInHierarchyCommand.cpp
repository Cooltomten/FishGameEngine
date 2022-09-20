#include "MoveInHierarchyCommand.h"
#include "ComponentSystem/Entity.h"
#include "Windows/HierarchyUtils.h"

MoveInHierarchyCommand::MoveInHierarchyCommand(std::vector<std::shared_ptr<Comp::Entity>>& someEntities, uint32_t indexToRemoveAt, uint32_t indexToInsertAt,
	std::shared_ptr<Comp::Entity> aParentEntity, int aChildIndex)
	:myEntitiesListRef(someEntities)
{
	myIndexToRemoveAt = indexToRemoveAt;
	myIndexToInsertAt = indexToInsertAt ;


	myParent = aParentEntity;
	myChildIndex = aChildIndex;

	myPreviousParent = nullptr;
	myPreviousChildIndex = -1;

	myInsertIsBiggerThanRemoveIndexFlag = myIndexToInsertAt > myIndexToRemoveAt;
}


void MoveInHierarchyCommand::Execute()
{
	auto it = myEntitiesListRef.begin() + myIndexToRemoveAt;
	auto entity = *it;
	auto children = entity->GetChildrenRecursive();
	myEntitiesListRef.erase(it);
	//remove all chidren from scene
	for (int i = 0; i < children.size(); i++)
	{
		myEntitiesListRef.erase(std::find(myEntitiesListRef.begin(), myEntitiesListRef.end(), children[i]));
	}
	if (myInsertIsBiggerThanRemoveIndexFlag)
	{
		myIndexToInsertAt--; // remove the entity itself index
		myIndexToInsertAt -= entity->GetRecursiveChildrenCount(); // remove all of the children index
	}
	myEntitiesListRef.insert(myEntitiesListRef.begin() + myIndexToInsertAt, entity);
	myEntitiesListRef.insert(myEntitiesListRef.begin() + myIndexToInsertAt + 1, children.begin(), children.end());
	
	myPreviousParent = entity->GetParent();
	//loop through all children and find the index of the entity
	if (myPreviousParent)
	{
		auto children = myPreviousParent->GetChildren();
		for (int i = 0; i < children.size(); i++)
		{
			if (myPreviousParent->GetChild(i) == entity)
			{
				myPreviousChildIndex = i;
				break;
			}
		}
	}
	entity->SetParent(myParent, myChildIndex);
}

void MoveInHierarchyCommand::Undo()
{
	auto it = myEntitiesListRef.begin() + myIndexToInsertAt;
	auto entity = *it;
	auto children = entity->GetChildrenRecursive();
	if (myInsertIsBiggerThanRemoveIndexFlag)
	{
		myIndexToInsertAt++; // add the entity itself index
		myIndexToInsertAt += entity->GetRecursiveChildrenCount(); // remove all of the children index
	}
	myEntitiesListRef.erase(it);
	//remove all chidren from scene
	for (int i = 0; i < children.size(); i++)
	{
		myEntitiesListRef.erase(std::find(myEntitiesListRef.begin(), myEntitiesListRef.end(), children[i]));
	}
	
	myEntitiesListRef.insert(myEntitiesListRef.begin() + myIndexToRemoveAt, entity);
	myEntitiesListRef.insert(myEntitiesListRef.begin() + myIndexToRemoveAt + 1, children.begin(), children.end());

	entity->SetParent(myPreviousParent, myPreviousChildIndex);
}