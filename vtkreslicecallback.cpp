#include "vtkreslicecallback.h"
#include <vtkImageResliceToColors.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include "utility.h"
#include "vtkreslicerotator.h"
#include "vtkreslicetranslator.h"
#include "vtkreslicewidgetrepresentation.h"

vtkStandardNewMacro(vtkResliceCallback);

void vtkResliceCallback::Execute(
	vtkObject* caller, const unsigned long eventId, void* callData)
{
	auto* const data = static_cast<double*>(callData);
	switch (eventId)
	{
	case LeftButtonReleaseEvent:
		m_resliceWidget->refreshWindows(-1);
		break;
	case cursorRotate:
		rotateCursor(data[0]);
		m_resliceWidget->centerImageActors(m_handleNumber);
		m_resliceWidget->refreshWindows(m_handleNumber);
		break;
	case cursorMove:
		moveCursor(data);
		m_resliceWidget->centerImageActors(m_handleNumber);
		m_resliceWidget->refreshWindows(m_handleNumber);
		break;
	case cursorFinishMovement:
	{
		setCursorPositiontoDefault();
		m_resliceWidget->centerImageActors(-1);
		m_resliceWidget->refreshWindows(-1);
	}
	break;
	case qualityLow:
	case qualityHigh:
		m_resliceWidget->setHighQuality(
			eventId - qualityLow,
			*(static_cast<int*>(callData)));
		break;
	case imageChanged:
		changeMatrixCenterPosition(m_resliceWidget->getImageReslicers()[m_handleNumber]->GetResliceAxes(),
			m_resliceWidget->getImageReslicers()[(m_handleNumber + 1) % 3]->GetResliceAxes());
		changeMatrixCenterPosition(m_resliceWidget->getImageReslicers()[m_handleNumber]->GetResliceAxes(),
			m_resliceWidget->getImageReslicers()[(m_handleNumber + 2) % 3]->GetResliceAxes());
		m_resliceWidget->centerImageActors(-1);
		m_resliceWidget->refreshWindows(m_handleNumber);
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
void vtkResliceCallback::rotateCursor(const double t_angle) const
{
	vtkMatrix4x4* sourceMatrix =
		m_resliceWidget->getImageReslicers()[m_handleNumber]->GetResliceAxes();
	vtkMatrix4x4* matrix =
		m_resliceWidget->getImageReslicers()[(m_handleNumber + 1) % 3]->GetResliceAxes();;
	vtkMatrix4x4* matrix2 =
		m_resliceWidget->getImageReslicers()[(m_handleNumber + 2) % 3]->GetResliceAxes();;
	vtkResliceRotator::rotatePlane(sourceMatrix, matrix, t_angle);
	vtkResliceRotator::rotatePlane(sourceMatrix, matrix2, t_angle);
}

//-----------------------------------------------------------------------------
void vtkResliceCallback::moveCursor(double* t_position) const
{
	vtkMatrix4x4* matrix = nullptr;
	vtkMatrix4x4* matrix2 = nullptr;
	vtkMatrix4x4* sourceMatrix =
		m_resliceWidget->getImageReslicers()[m_handleNumber]->GetResliceAxes();
	switch (m_handleNumber)
	{
	case 0:
		matrix = m_resliceWidget->getImageReslicers()[1]->GetResliceAxes();
		matrix2 = m_resliceWidget->getImageReslicers()[2]->GetResliceAxes();
		break;
	case 1:
		matrix = m_resliceWidget->getImageReslicers()[0]->GetResliceAxes();
		matrix2 = m_resliceWidget->getImageReslicers()[2]->GetResliceAxes();
		break;
	case 2:
		matrix = m_resliceWidget->getImageReslicers()[0]->GetResliceAxes();
		matrix2 = m_resliceWidget->getImageReslicers()[1]->GetResliceAxes();
		break;
	default:
		break;
	}
	if (m_window)
	{
		vtkResliceTranslator::movePlaneX(
			sourceMatrix, matrix, matrix2, t_position);
	}
}

//-----------------------------------------------------------------------------
void vtkResliceCallback::setCursorPositiontoDefault() const
{
	vtkMatrix4x4* matrix2 =
		m_resliceWidget->getImageReslicers()[(m_handleNumber + 1) % 3]->GetResliceAxes();;
	vtkMatrix4x4* sourceMatrix =
		m_resliceWidget->getImageReslicers()[m_handleNumber]->GetResliceAxes();
	sourceMatrix->SetElement(0, 3, matrix2->GetElement(0, 3));
	sourceMatrix->SetElement(1, 3, matrix2->GetElement(1, 3));
	sourceMatrix->SetElement(2, 3, matrix2->GetElement(2, 3));
}

//-----------------------------------------------------------------------------
void vtkResliceCallback::changeMatrixCenterPosition(vtkMatrix4x4* t_sourceMatrix,
	vtkMatrix4x4* t_destinationMatrix)
{
	t_destinationMatrix->SetElement(0, 3, t_sourceMatrix->GetElement(0, 3));
	t_destinationMatrix->SetElement(1, 3, t_sourceMatrix->GetElement(1, 3));
	t_destinationMatrix->SetElement(2, 3, t_sourceMatrix->GetElement(2, 3));
}
