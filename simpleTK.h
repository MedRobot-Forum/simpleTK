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

#include "vtkSmartPointer.h"
#include "vtkStringArray.h"
#include "vtkIntArray.h"
#include "TagDialog.h"
#include "utility.h"
#include "MPRMaker.h"
#include "vtkDICOMDirectory.h"

class simpleTK : public QMainWindow
{
    Q_OBJECT

public:
    simpleTK(QWidget *parent = nullptr);
    ~simpleTK();


public slots:
	void openFolder();
	void openDicomTag();
	void init();
	void constructMPR(double *center);
	void constructMPR();
	void GetVector1(vtkPlaneSource* planeSource, double v1[3]);
	void GetVector2(vtkPlaneSource* planeSource, double v2[3]);
private:
    Ui::simpleTKClass ui;
	TagDialog *tagDialog{};
	std::unique_ptr<MPRMaker> m_mprMaker = {};
	std::string m_path = {};
private:

	//vtkSmartPointer<vtkDICOMImageReader> mReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	vtkSmartPointer<vtkDICOMReader> mReader = vtkSmartPointer<vtkDICOMReader>::New();
	vtkSmartPointer<vtkImageViewer2> mImageViewer[3];
	vtkSmartPointer<vtkRenderer> mImageViewerRenderer[3];
	vtkSmartPointer<vtkRenderWindowInteractor> mImageViewerWindowInteractor[3];
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> mImageViewerRenderWindow[3];
	vtkSmartPointer<vtkTextActor> mViewImage2D[3];
	vtkSmartPointer<vtkNamedColors> mpColors;
	vtkSmartPointer<vtkPlaneSource> mpSlicePlanes[3];
	vtkSmartPointer<vtkActor> mpSlicePlaneActors[3];
	//vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindows[3] = {};
	vtkSmartPointer<vtkImageReslice> reslice[3];
};
