#include "EditorWindow.h"
#include <string>

uint32_t EditorWindow::ourNextId = 0;
EditorWindow::EditorWindow(std::string aName) 
	: myId(ourNextId++)
{
	myName = aName;
	myOpenFlag = true;

	myUndoHandler = std::make_unique<UndoHandler>();
}
