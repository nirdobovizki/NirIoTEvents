// 
// 
// 

#include "Element.h"
#include "LoopManager.h"

Element::Element() : _enabled(true)
{
	LoopManager.AddCallback([this]() { if (_enabled) Loop(); });
}

void Element::Loop()
{
}

void Element::Enable()
{
	_enabled = true;
}

void Element::Disable()
{
	_enabled = false;
}
