#include "UndoHandler.h"

UndoHandler::UndoHandler()
{
}

UndoHandler::~UndoHandler()
{
}
void UndoHandler::ExecuteAndAdd(std::shared_ptr<UndoCommand> aCommand)
{
	aCommand->Execute();
	AddUndo(aCommand);
}

void UndoHandler::AddUndo(std::shared_ptr<UndoCommand> aCommand)
{
	myCommandStack.push(aCommand);

	while (!myRedoStack.empty())
	{
		myRedoStack.pop();
	}
}

void UndoHandler::Undo()
{
	if (!myCommandStack.empty())
	{
		myCommandStack.top()->Undo();
		myRedoStack.push(myCommandStack.top());
		myCommandStack.pop();
	}
}

void UndoHandler::Redo()
{
	if (!myRedoStack.empty())
	{
		myRedoStack.top()->Execute();
		myCommandStack.push(myRedoStack.top());
		myRedoStack.pop();
	}
}

void UndoHandler::ClearUndo()
{
	while (!myCommandStack.empty())
	{
		myCommandStack.pop();
	}
}

void UndoHandler::SetMaxCommandCount(uint32_t aCount)
{
	myMaxCommandCount = aCount;
}

void UndoHandler::SetMaxRedoCount(uint32_t aCount)
{
	myMaxRedoCount = aCount;
}

