#pragma once

class UndoCommand
{
public:
	UndoCommand() = default;
	virtual ~UndoCommand() = default;

	virtual void Undo() = 0;
	virtual void Execute() = 0;
};