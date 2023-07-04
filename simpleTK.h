#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_simpleTK.h"
#include <QFileDialog>
#include <VtkInclude.h>
//#include "vtkDICOMDirectory.h"
//#include "vtkDICOMItem.h"
//#include "vtkDICOMMetaData.h"
//#include "vtkDICOMReader.h"
//#include "vtkDICOMDictionary.h"
#include "TagDialog.h"
#include "vtkSmartPointer.h"
#include "vtkStringArray.h"
#include "vtkIntArray.h"

#include "utility.h"
//#include "MPRMaker.h"
#include "vtkDICOMDirectory.h"
#include <vtkImageCast.h>
#include "vtkwidgetmpr.h"
#include <vtkResliceCursorCallback.h>
class simpleTK : public QMainWindow
{
    Q_OBJECT

public:
    simpleTK(QWidget *parent = nullptr);
    ~simpleTK();


public slots:
	void openFolder();
	void openDicomTag();
	void showCursorReslize(bool flag);
	void init();
	//void constructMPR(double *center);
	void constructMPR();
	void constructMPR2(vtkSmartPointer<vtkImageData> imageData);
	void constructMPR3(vtkSmartPointer<vtkImageData> imageData);
	//void GetVector1(vtkPlaneSource* planeSource, double v1[3]);
	//void GetVector2(vtkPlaneSource* planeSource, double v2[3]);

	void volumeRender(vtkSmartPointer<vtkImageData> imageData);
	void UpdateRenderModel(int value);
private:

private:
    Ui::simpleTKClass ui;
	//std::unique_ptr<MPRMaker> m_mprMaker = {};
	std::unique_ptr<vtkWidgetMPR> m_widgetMPR = {};
	std::string m_path = {};
	TagDialog* tagDialog = nullptr;

private:

	vtkSmartPointer<vtkImageViewer2> imageViewer[3];
	vtkSmartPointer<vtkRenderer> renderer[3];
	vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor[3];
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> irenderWindow[3];

	//3d
	vtkSmartPointer<vtkRenderer> volumeRenderer;


	///////////////////////////////////////////////////////
	//vtkSmartPointer<vtkDICOMImageReader> mReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	vtkSmartPointer<vtkDICOMReader> mReader = vtkSmartPointer<vtkDICOMReader>::New();
	//vtkSmartPointer<vtkImageViewer2> mImageViewer[3];
	//vtkSmartPointer<vtkRenderer> renderer[4];
	//vtkSmartPointer<vtkRenderWindowInteractor> mImageViewerWindowInteractor[3];
	//vtkSmartPointer<vtkGenericOpenGLRenderWindow> mImageViewerRenderWindow[3];
	vtkSmartPointer<vtkTextActor> mViewImage2D[3];
	vtkSmartPointer<vtkNamedColors> mpColors;
	vtkSmartPointer<vtkPlaneSource> mpSlicePlanes[3];
	vtkSmartPointer<vtkActor> mpSlicePlaneActors[3];
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindows[4] = {};
	vtkSmartPointer<vtkImageReslice> reslice[3];
	//vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor[4];

	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper;
	vtkSmartPointer<vtkVolume> volume;
	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();



	vtkSmartPointer<vtkResliceImageViewer> riw[3];
	vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];
	vtkSmartPointer<vtkDistanceWidget> DistanceWidget[3];
	vtkSmartPointer<vtkResliceImageViewerMeasurements> ResliceMeasurements;



	







};
