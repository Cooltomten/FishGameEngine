#pragma once
#include <cstdint>
#include <string>

class EditorWindow
{
public:
	EditorWindow(std::string aName);
	virtual ~EditorWindow() = default;

	virtual void Update(float aTimeStep) = 0;
	virtual void Render() = 0;
	
	inline bool IsOpen() { return myOpenFlag; }
	inline void SetOpen(bool aFlag) { myOpenFlag = aFlag; }
	
	
	//const char* AddIdentifier(char* aName);

protected:
	const uint32_t myId;
	bool myOpenFlag;
private:
	static uint32_t ourNextId;
	std::string myName;
};

