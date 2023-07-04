#include "vtkwidgetmprcallback.h"
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>
#include "vtkwidgetmprinteractorstyle.h"

vtkStandardNewMacro(vtkWidgetMPRCallback);

//-----------------------------------------------------------------------------
void vtkWidgetMPRCallback::Execute(vtkObject* caller, const unsigned long eventId, void* callData)
{
	if (eventId == ModifiedEvent)
	{
		if (auto* const interactorStyle = dynamic_cast<vtkWidgetMPRInteractorStyle*>(
			vtkRenderWindow::SafeDownCast(caller)
			->GetInteractor()->GetInteractorStyle()); interactorStyle)
		{
			interactorStyle->rescaleAxisActor();
		}
	}
}