#pragma once

#include "UndoSystem/UndoHandler.h"

#include <cstdint>
#include <string>
#include <memory>

namespace FGE
{
	class Event;
}
class UndoHandler;
class EditorWindow
{
public:
	EditorWindow(std::string aName);
	virtual ~EditorWindow() = default;

	virtual void Update(float aTimeStep) = 0;
	virtual void Render() = 0;
	virtual void UpdateImGui() = 0;
	virtual void OnEvent(FGE::Event& aEvent) {};
	
	inline bool IsOpen() { return myOpenFlag; }
	inline void SetOpen(bool aFlag) { myOpenFlag = aFlag; }

	inline const std::string& GetName() { return myName; }
protected:
	const uint32_t myId;
	bool myOpenFlag;

	std::unique_ptr<UndoHandler> myUndoHandler;
private:
	static uint32_t ourNextId;
	std::string myName;
};

