#include "EditorWindow.h"
#include <string>

uint32_t EditorWindow::ourNextId = 0;
EditorWindow::EditorWindow(std::string aName) 
	: myId(ourNextId++)
{
	myName = aName;
	myOpenFlag = true;
}

//const char* EditorWindow::AddIdentifier(char* aName)
//{
//	return (std::string(aName) + std::to_string(myId)).c_str();
//}
