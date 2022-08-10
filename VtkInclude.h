#pragma once
#include "vtkSmartVolumeMapper.h"
#include "vtkBoundedPlanePointPlacer.h"
#include "vtkCellPicker.h"
#include "vtkCommand.h"
#include "vtkDICOMImageReader.h"
#include "vtkDistanceRepresentation.h"
#include "vtkDistanceRepresentation2D.h"
#include "vtkDistanceWidget.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include "vtkHandleRepresentation.h"
#include "vtkImageData.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkImageSlabReslice.h"
#include "vtkInteractorStyleImage.h"
#include "vtkLookupTable.h"
#include "vtkPlane.h"
#include "vtkPlaneSource.h"
#include "vtkPointHandleRepresentation2D.h"
#include "vtkPointHandleRepresentation3D.h"
#include "vtkProperty.h"
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include "vtkRenderWindowInteractor.h"
#include "vtkResliceImageViewer.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkResliceCursorThickLineRepresentation.h"
#include "vtkResliceCursorWidget.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkResliceCursor.h"
#include "vtkResliceImageViewerMeasurements.h"
#include <vtkImagePlaneWidget.h>
#include <vtkResliceImageViewer.h>
#include <vtkImageMathematics.h>
#include <vtkTextActor.h>
#include "vtkVolumePicker.h"
#include "vtkPicker.h"
#include "vtkCellPicker.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkAngleWidget.h>
#include <vtkProperty2D.h>
#include <vtkLeaderActor2D.h>
#include <vtkAngleRepresentation2D.h>
#include <vtkBiDimensionalWidget.h>
#include <vtkBiDimensionalRepresentation2D.h>

#include <vtkPlaneCollection.h>
#include <vtkPlanes.h>
#include <vtkBoxWidget2.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vtkVolume.h>
#include <vtkBoxRepresentation.h>
#include "vtkImageReslice.h"
#include "vtkNamedColors.h"

#include <vtkDistanceRepresentation.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation3D.h>
#include <vtkContourWidget.h>
#include <vtkProperty.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkImageActor.h>
#include <vtkLineSource.h>
#include <vtkPolyLine.h>
#include <vtkBoxWidget2.h>
#include <vtkPoints.h>
//VTL include header


#include <vtkColorTransferFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkBorderWidget.h>
#include <vtkSmartPointer.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkRenderer.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkView.h>
#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkPolyDataNormals.h>
#include <vtkTextProperty.h>
#include <vtkCubeAxesActor2D.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkProperty2D.h>
#include <vtkPropAssembly.h>
#include <vtkSphereSource.h>

//filter
#include <vtkOutlineFilter.h>
#include <vtkTubeFilter.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkTransformPolyDataFilter.h>

#include <vtkPlaneSource.h>
#include <vtkFloatArray.h>
#include <vtkRectilinearGrid.h>
#include <vtkRectilinearGridGeometryFilter.h>

#include <vtkBoundingBox.h>
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkLightActor.h>
#include <vtkLightCollection.h>

//for vtk line storing and showing
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkTriangle.h>
#include <vtkPolyLine.h>

//for screenshot
#include <vtkPNGWriter.h>
#include <vtkWindowToImageFilter.h>

//for volume calculation
#include <vtkMassProperties.h>

//for image
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkActor2D.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSplineWidget2.h>
#include <vtkSplineFilter.h>
#include <vtkParametricSpline.h>
#include <vtkSplineRepresentation.h>
#include <vtkParametricFunctionSource.h>
#include <vtkProbeFilter.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCellPicker.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkRendererCollection.h>
#include <vtkWindowLevelLookupTable.h>
#include <vtkDataSetMapper.h>
#include <vtkXYPlotActor.h>
#include <vtkPointData.h>
#include <vtkImagePointIterator.h>
#include <vtkMetaImageReader.h>
#include <vtkMetaImageWriter.h>
#include <vtkInformation.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageActor.h>
#include <vtkLineSource.h>
#include <vtkPolyLine.h>
