#pragma once
#include "UndoSystem/UndoCommand.h"
#include <stack>
#include <queue>
#include <memory>

class UndoHandler
{
public:
	UndoHandler();
	~UndoHandler();

	void ExecuteAndAdd(std::shared_ptr<UndoCommand> aCommand);
	void AddUndo(std::shared_ptr<UndoCommand> aCommand);
	void Undo();
	void Redo();
	void ClearUndo();

	void SetMaxCommandCount( uint32_t aCount);
	void SetMaxRedoCount(uint32_t aCount);

private:
	std::stack<std::shared_ptr<UndoCommand>> myCommandStack;
	std::stack<std::shared_ptr<UndoCommand>> myRedoStack;

	int myMaxCommandCount;
	int myMaxRedoCount;
};

