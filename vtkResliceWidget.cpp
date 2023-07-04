#include "vtkreslicewidget.h"
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkWidgetRepresentation.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageResliceMapper.h>
#include "utility.h"
#include "vtkreslicecallback.h"
#include "vtkreslicewidgetrepresentation.h"


vtkStandardNewMacro(vtkResliceWidget);

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::SetEnabled(const int enabling)
{
	if (enabling) //----------------
	{
		vtkDebugMacro(<< "Enabling widget");
		if (Enabled)
		{
			return;
		}
		if (!m_windows[0] ||
			!m_windows[1] ||
			!m_windows[2])
		{
			vtkErrorMacro(<< "No render windows");
			return;
		}
		CreateDefaultRepresentation();
		for (int i = 0; i < 3; ++i)
		{
			m_cursorWidget[i]->setPlane(i);
			m_cursorWidget[i]->SetInteractor(m_windows[i]->GetInteractor());
			m_cursorWidget[i]->SetEnabled(1);
			m_cbk[i]->setRenderWindow(m_windows[i]);
			m_windows[i]->Render();
		}
		Enabled = 1;
	}
	else
	{
		for (auto& widget : m_cursorWidget)
		{
			if (widget)
			{
				widget->RemoveAllObservers();
				widget = nullptr;
			}
		}
		refreshWindows(-1);
		Enabled = 0;
	}
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::CreateDefaultRepresentation()
{
	for (auto i = 0; i < 3; ++i)
	{
		if (!m_cursorWidget[i])
		{
			m_cursorWidget[i] =
				vtkSmartPointer<vtkReslicePlaneCursorWidget>::New();
			m_cbk[i] = vtkSmartPointer<vtkResliceCallback>::New();
			m_cbk[i]->setHandleNumber(i);
			m_cbk[i]->setWidget(this);
			m_cursorWidget[i]->AddObserver(cursorRotate, m_cbk[i]);
			m_cursorWidget[i]->AddObserver(cursorMove, m_cbk[i]);
			m_cursorWidget[i]->AddObserver(qualityLow, m_cbk[i]);
			m_cursorWidget[i]->AddObserver(qualityHigh, m_cbk[i]);
			m_cursorWidget[i]->AddObserver(vtkCommand::LeftButtonReleaseEvent, m_cbk[i]);
			m_cursorWidget[i]->AddObserver(cursorFinishMovement, m_cbk[i]);
			m_windows[i]->GetInteractor()->GetInteractorStyle()->AddObserver(
				imageChanged, m_cbk[i]);
		}
	}
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::setImageReslicers(
	const vtkSmartPointer<vtkImageResliceToColors>& m_firstReslice,
	const vtkSmartPointer<vtkImageResliceToColors>& m_secondReslice,
	const vtkSmartPointer<vtkImageResliceToColors>& m_thirdReslice)
{
	m_imageReslice[0] = m_firstReslice;
	m_imageReslice[1] = m_secondReslice;
	m_imageReslice[2] = m_thirdReslice;
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::setRenderWindows(
	vtkSmartPointer<vtkRenderWindow>* t_windows)
{
	m_windows[0] = t_windows[0];
	m_windows[1] = t_windows[1];
	m_windows[2] = t_windows[2];
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::refreshWindows(const int t_windowNumber)
{
	for (auto i = 0; i < 3; ++i)
	{
		if (t_windowNumber == i)
		{
			continue;
		}
		if (t_windowNumber != -1)
		{
			if (m_isCameraCentered)
			{
				resetCamera(m_windows[i]);
			}
		}
		m_windows[i]->Render();
	}
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::setVisible(const bool t_visible)
{
	for (auto& m_cursorWidget : m_cursorWidget)
	{
		if (m_cursorWidget)
		{
			m_cursorWidget->GetRepresentation()->
				SetVisibility(static_cast<int>(t_visible));
		}
	}
	refreshWindows(-1);
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::setCameraCentered(const int t_centered)
{
	m_isCameraCentered = t_centered;
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::centerImageActors(const int t_excludedCursor)
{
	for (auto i = 0; i < 3; ++i)
	{
		if (i == t_excludedCursor)
		{
			continue;
		}
		vtkPropCollection* actorCollection = m_windows[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
		actorCollection->InitTraversal();
		for (vtkIdType j = 0; j < actorCollection->GetNumberOfItems(); j++)
		{
			vtkProp* nextProp = actorCollection->GetNextProp();
			if (nextProp->IsA("vtkImageSlice"))
			{
				auto* const actor = reinterpret_cast<vtkImageActor*>(nextProp);
				auto* const actorPosition = actor->GetPosition();
				auto* const cursorPosition = m_cursorWidget[i]->getCursorCenterPosition();
				double newPosition[3] = {
					cursorPosition[0] + actorPosition[0] - actor->GetCenter()[0],
					cursorPosition[1] + actorPosition[1] - actor->GetCenter()[1],
					0.1
				};
				m_cursorWidget[i]->setCursorPosition(newPosition);
				actor->SetPosition(actorPosition[0] - actor->GetCenter()[0],
					actorPosition[1] - actor->GetCenter()[1],
					actorPosition[2] - actor->GetCenter()[2]);
			}
		}
	}
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::resetCamera(vtkRenderWindow* t_window)
{
	vtkRenderer* currentWindowRenderer = t_window->GetRenderers()->GetFirstRenderer();
	currentWindowRenderer->SetBackground(0, 0, 0);
	currentWindowRenderer->SetBackground2(0, 0, 0);
	vtkCamera* camera = currentWindowRenderer->GetActiveCamera();
	double* actorCenter = getImageActorCenterPosition(t_window);
	if (actorCenter)
	{
		double* currentFocalPoint = camera->GetFocalPoint();
		double* currentCameraPosition = camera->GetPosition();
		camera->SetFocalPoint(actorCenter[0], actorCenter[1], currentFocalPoint[2]);
		camera->SetPosition(actorCenter[0], actorCenter[1], currentCameraPosition[2]);
	}
}

/*------------------------------------------------------------------------------------*/
double* vtkResliceWidget::getImageActorCenterPosition(vtkRenderWindow* t_window)
{
	vtkPropCollection* actorCollection =
		t_window->GetRenderers()->GetFirstRenderer()->GetViewProps();
	actorCollection->InitTraversal();

	for (vtkIdType j = 0; j < actorCollection->GetNumberOfItems(); j++)
	{
		vtkProp* nextProp = actorCollection->GetNextProp();
		if (nextProp->IsA("vtkImageSlice"))
		{
			return reinterpret_cast<vtkImageActor*>(nextProp)->GetCenter();
		}
	}
	return nullptr;
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::setHighQuality(const int t_highQuality, const int t_plane)
{
	for (auto i = 0; i < 3; ++i)
	{
		if (i == t_plane)
		{
			continue;
		}
		vtkPropCollection* actorCollection =
			m_windows[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
		actorCollection->InitTraversal();
		for (vtkIdType j = 0; j < actorCollection->GetNumberOfItems(); j++)
		{
			vtkProp* nextProp = actorCollection->GetNextProp();
			if (nextProp->IsA("vtkImageSlice"))
			{
				if (t_highQuality == 0)
				{
					setQualityToLow(i, reinterpret_cast<vtkImageActor*>(nextProp));
				}
				else
				{
					setQualityToHigh(i, reinterpret_cast<vtkImageActor*>(nextProp));
				}
				break;
			}
		}
	}
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::resetResliceCursor()
{
	for (auto const& cursor : m_cursorWidget)
	{
		double pos[2] = { 0, 0 };
		cursor->setCursorCenterPosition(pos);
		auto* const rep =
			dynamic_cast<vtkResliceWidgetRepresentation*>(
				cursor->GetRepresentation());
		if (rep)
		{
			rep->getResliceActor()->reset();
		}
	}
	for (auto const& window : m_windows)
	{
		resetCamera(window);
		window->Render();
	}
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::setQualityToHigh(
	const int t_windowNumber, vtkImageActor* t_actor)
{
	reinterpret_cast<vtkImageResliceMapper*>(
		t_actor->GetMapper())->SetResampleToScreenPixels(1);
	m_imageReslice[t_windowNumber]->SetOutputExtentToDefault();
	m_imageReslice[t_windowNumber]->SetOutputSpacingToDefault();
	m_imageReslice[t_windowNumber]->SetInterpolationModeToCubic();
	m_imageReslice[t_windowNumber]->Update();
}

/*------------------------------------------------------------------------------------*/
void vtkResliceWidget::setQualityToLow(
	const int t_windowNumber, vtkImageActor* t_actor)
{
	reinterpret_cast<vtkImageResliceMapper*>(
		t_actor->GetMapper())->SetResampleToScreenPixels(0);
	m_imageReslice[t_windowNumber]->SetInterpolationModeToLinear();
	m_imageReslice[t_windowNumber]->SetOutputSpacing(2, 2, 2);
	m_imageReslice[t_windowNumber]->Update();
}
