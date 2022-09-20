#pragma once
#include "UndoSystem/UndoCommand.h"

#include <memory>
#include <vector>

namespace Comp
{
	class Entity;
}
	

class MoveInHierarchyCommand : public UndoCommand
{

public:
	MoveInHierarchyCommand(std::vector<std::shared_ptr<Comp::Entity>>& someEntities, uint32_t indexToRemoveAt, uint32_t indexToInsertAt,
		std::shared_ptr<Comp::Entity> aParentEntity, int aChildIndex);
	// Inherited via UndoCommand
	virtual void Undo() override;
	virtual void Execute() override;

private:
	std::vector<std::shared_ptr<Comp::Entity>>& myEntitiesListRef;
	uint32_t myIndexToRemoveAt;
	uint32_t myIndexToInsertAt;
	
	std::shared_ptr<Comp::Entity> myParent;
	int myChildIndex;
	
	std::shared_ptr<Comp::Entity> myPreviousParent;
	int myPreviousChildIndex;
	bool myInsertIsBiggerThanRemoveIndexFlag = false;
	
	
	
	
};