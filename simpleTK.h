#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_simpleTK.h"
#include <QFileDialog>
#include <VtkInclude.h>
#include "vtkDICOMDirectory.h"
#include "vtkDICOMItem.h"
#include "vtkDICOMMetaData.h"
#include "vtkDICOMDictionary.h"

#include "vtkSmartPointer.h"
#include "vtkStringArray.h"
#include "vtkIntArray.h"

class simpleTK : public QMainWindow
{
    Q_OBJECT

public:
    simpleTK(QWidget *parent = nullptr);
    ~simpleTK();


public slots:
	void openFile();
	void init();
	void constructMPR(double *center);
	void GetVector1(vtkPlaneSource* planeSource, double v1[3]);
	void GetVector2(vtkPlaneSource* planeSource, double v2[3]);
private:
    Ui::simpleTKClass ui;

private:

	vtkSmartPointer<vtkDICOMImageReader> mReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	vtkSmartPointer<vtkImageViewer2> mImageViewer[4];
	vtkSmartPointer<vtkRenderer> mImageViewerRenderer[4];
	vtkSmartPointer<vtkRenderWindowInteractor> mImageViewerWindowInteractor[4];
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> mImageViewerRenderWindow[4];
	vtkSmartPointer<vtkTextActor> mViewImage2D[3];
	vtkSmartPointer<vtkNamedColors> mpColors;
	vtkSmartPointer<vtkPlaneSource> mpSlicePlanes[3];
	vtkSmartPointer<vtkActor> mpSlicePlaneActors[3];

	vtkSmartPointer<vtkImageReslice> reslice[3];
};
