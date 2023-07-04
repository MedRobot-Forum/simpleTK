#pragma once
#include <vtkCommand.h>
#include <vtkDICOMTag.h>
class utility
{
};
constexpr auto iconTitleBar = ":/res/res/icon_small.png";
constexpr auto iconapp = ":/res/res/icon_big.png";
enum class transformationType
{
	none,
	flipHorizontal,
	flipVertical,
	rotateLeft,
	rotateRight,
	invert,
	scrollMouse,
	zoom,
	pan,
	windowLevel
};

enum class overlayKey
{
	zoom = 1001,
	series = 1003,
	window = 2625616,
	level = 2625617
};

enum vtkCustomEvents : unsigned long
{
	changeScrollValue = vtkCommand::UserEvent + 1,
	defaultCursor = changeScrollValue + 1,
	cursorMove = defaultCursor + 1,
	cursorFinishMovement = cursorMove + 1,
	cursorRotate = cursorFinishMovement + 1,
	imageChanged = cursorRotate + 1,
	qualityLow = imageChanged + 1,
	qualityHigh = qualityLow + 1,
};