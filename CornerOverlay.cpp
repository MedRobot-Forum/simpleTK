#include "corneroverlay.h"

vtkOpenGLTextActor* CornerOverlay::getTextActor()
{
	initTextActor();
	return m_textActor;
}

//-----------------------------------------------------------------------------
std::string CornerOverlay::getOverlayFromInfo()
{
	std::string values;
	for (const auto& value : m_overlaysInfo)
	{
		values.append(value.second);
	}
	return values;
}
