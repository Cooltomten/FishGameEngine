#pragma once

#include "Engine/Event/Event.h"
#include <sstream>


class EntityPropertyUpdatedEvent : public FGE::Event
{
public:
	EntityPropertyUpdatedEvent() {}
	~EntityPropertyUpdatedEvent() {}

	EVENT_CLASS_TYPE(EntityPropertyUpdated)
};
