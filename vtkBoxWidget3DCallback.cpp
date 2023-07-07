#include <vtkAbstractVolumeMapper.h>
#include <vtkBoxRepresentation.h>
#include <vtkBoxWidget2.h>
#include <vtkObjectFactory.h>
#include "vtkboxwidget3dcallback.h"

vtkStandardNewMacro(vtkBoxWidget3DCallback);

void vtkBoxWidget3DCallback::Execute(vtkObject* caller, [[maybe_unused]] unsigned long eventId, [[maybe_unused]] void* callData)
{
	auto* const boxWidget =
		vtkBoxWidget2::SafeDownCast(caller);
	auto* const  boxRepresentation =
		vtkBoxRepresentation::SafeDownCast(boxWidget->GetRepresentation());
	boxRepresentation->SetInsideOut(1);
	boxRepresentation->GetPlanes(m_planes);
	m_volume->GetMapper()->SetClippingPlanes(m_planes);
}
