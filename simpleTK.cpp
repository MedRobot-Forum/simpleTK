#include "simpleTK.h"
//------------------------------------------------------------------------------

simpleTK::simpleTK(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->setStyleSheet("QWidget{background: black;}");
	init();
	
}

simpleTK::~simpleTK()
{}

void simpleTK::init()
{
	connect(ui.actionFolder, &QAction::triggered, this, &simpleTK::openFolder);
	connect(ui.actionTag, &QAction::triggered, this, &simpleTK::openDicomTag);
	connect(ui.actionreslize, &QAction::triggered, this, &simpleTK::showCursorReslize);
	ui.actionreslize->setCheckable(true);


	m_widgetMPR = std::make_unique<vtkWidgetMPR>();

	for (int i = 0; i < 3; i++)
	{
		irenderWindow[i] = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

	}
	ui.imageViewerWidget0->setRenderWindow(irenderWindow[0]);
	ui.imageViewerWidget1->setRenderWindow(irenderWindow[1]);
	ui.imageViewerWidget2->setRenderWindow(irenderWindow[2]);
    

	//3d
		volumeRenderer = vtkSmartPointer<vtkRenderer>::New();
		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
		ui.imageViewerWidget3->setRenderWindow(renderWindow);
		ui.imageViewerWidget3->renderWindow()->AddRenderer(volumeRenderer);


	UpdateRenderModel(0);
	for (int i = 0; i < 3; i++)
	{
		mpSlicePlanes[i] = vtkSmartPointer<vtkPlaneSource>::New();
		mpSlicePlaneActors[i] = vtkSmartPointer<vtkActor>::New();
		mViewImage2D[i] = vtkSmartPointer<vtkTextActor>::New();

	}
	mpColors = vtkSmartPointer<vtkNamedColors>::New();

}


void simpleTK::openDicomTag()
{
	QString filePath = "D:/workspace/simpleTK/res/SLC";
	tagDialog = new TagDialog(nullptr, filePath);
	tagDialog->show();	
}

void simpleTK::showCursorReslize(bool flag)
{

	m_widgetMPR->setShowCursor(flag);
}

void simpleTK::openFolder()
{

// 	QString filePath = QFileDialog::getExistingDirectory(this, QStringLiteral("Open directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
// 	if (filePath.isEmpty())
// 		return;
// 
// 	mReader->SetDirectoryName(filePath.toStdString().c_str());
// 	mReader->Update();
// 	double* center = mReader->GetOutput()->GetCenter();

	m_path = "D:/workspace/project/simpleTK/res/SLC";
	vtkSmartPointer<vtkDICOMDirectory> dicomDir = vtkSmartPointer<vtkDICOMDirectory>::New();
	dicomDir->SetDirectoryName(m_path.c_str());
	dicomDir->RequirePixelDataOn();
	dicomDir->Update();

	if (dicomDir->GetNumberOfSeries() > 0) {
		mReader->SetFileNames(dicomDir->GetFileNamesForSeries(0));
		mReader->Update();

	}	



	//constructMPR3(mReader->GetOutput());
	//volumeRender(mReader->GetOutput());

	constructMPR();
	volumeRender(mReader->GetOutput());
}
void simpleTK::constructMPR()
{
	if (!m_widgetMPR)
	{
		return;
	}
	
	m_widgetMPR->setRenderWindowsMPR(
		ui.imageViewerWidget1->renderWindow(),
		ui.imageViewerWidget2->renderWindow(),
		ui.imageViewerWidget0->renderWindow());
	m_widgetMPR->render(mReader);
}

void simpleTK::volumeRender(vtkSmartPointer<vtkImageData> imageData) {

	int dim[3];
	imageData->GetDimensions(dim);
	if (dim[0] < 2 || dim[1] < 2 || dim[2] < 2)
	{
	 	cout << "Error loading data!" << endl;
	 	exit(EXIT_FAILURE);
	}

	volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	volumeMapper->SetInputData(imageData);
	volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance() / 6);	//设置光线采样距离

	volume = vtkSmartPointer<vtkVolume>::New(); 
	
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	double* center = mReader->GetOutput()->GetCenter();

	vtkSmartPointer<vtkCamera> camera = volumeRenderer->GetActiveCamera();
	camera->SetViewUp(0, 0, -1);
	camera->SetFocalPoint(center[0], center[1], center[2]);
	camera->SetPosition(center[0], center[1] + 300, center[2]);
	camera->ComputeViewPlaneNormal();
	camera->ParallelProjectionOn();
	camera->OrthogonalizeViewUp();
	camera->SetParallelScale(85);
	volumeRenderer->SetActiveCamera(camera);
	volumeRenderer->AddVolume(volume);

	volumeRenderer->GetRenderWindow()->Render();


	/*vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName("D:\\workspace\\simpleTK\\marker3.stl");
	reader->Update();

	vtkSmartPointer<vtkPolyData> marker = reader->GetOutput();
	mpMarkerWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(marker);
	vtkSmartPointer<vtkActor> markerActor = vtkSmartPointer<vtkActor>::New();
	markerActor->SetMapper(mapper);

	mpMarkerWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	mpMarkerWidget->SetOrientationMarker(markerActor);
	mpMarkerWidget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	mpMarkerWidget->SetInteractor(mImageViewerRenderWindow[3]->GetInteractor());
	mpMarkerWidget->SetViewport(0.8, 0.0, 1, 0.2);
	mpMarkerWidget->SetEnabled(1);
	mpMarkerWidget->InteractiveOff();*/


}

void simpleTK::UpdateRenderModel(int value)
{
	if (value == 0)
	{
		volumeProperty->SetInterpolationTypeToLinear();
		volumeProperty->ShadeOn();  //打开或者关闭阴影测试
		volumeProperty->SetAmbient(0.3);
		volumeProperty->SetDiffuse(0.8);  //漫反射
		vtkSmartPointer<vtkPiecewiseFunction> opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
		opacityFun->AddPoint(-1100, 0);
		opacityFun->AddPoint(1043, 0);
		opacityFun->AddPoint(1459, 1);
		opacityFun->AddPoint(3072, 1);
		opacityFun->AddPoint(3899, 1);
		vtkSmartPointer<vtkColorTransferFunction> colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
		colorFun->AddRGBPoint(-1100, 0, 0, 0);
		colorFun->AddRGBPoint(129, 0.3, 0, 0);
		colorFun->AddRGBPoint(1059, 0.8, 0.07, 0);
		colorFun->AddRGBPoint(1459, 1, 0.9, 0.6);
		colorFun->AddRGBPoint(1863, 1, 1, 1);
		colorFun->AddRGBPoint(3899, 1, 1, 1);
		volumeProperty->SetScalarOpacity(opacityFun);
		volumeProperty->SetColor(colorFun);
	}
	else if (value == 1)
	{
		volumeProperty->SetInterpolationTypeToLinear();
		//volumeProperty->ShadeOn();  //打开或者关闭阴影测试
		volumeProperty->SetAmbient(1);
		volumeProperty->SetDiffuse(0);  //漫反射
		vtkSmartPointer<vtkPiecewiseFunction> opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
		opacityFun->AddPoint(-1100, 0);
		opacityFun->AddPoint(1043, 0);
		opacityFun->AddPoint(1853, 0.28);
		opacityFun->AddPoint(3072, 0.5);
		opacityFun->AddPoint(3899, 1);
		vtkSmartPointer<vtkColorTransferFunction> colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
		colorFun->AddRGBPoint(-1100, 0, 0, 0);
		colorFun->AddRGBPoint(129, 0.7, 0, 0);
		colorFun->AddRGBPoint(954, 0.9, 0.3, 0);
		colorFun->AddRGBPoint(1978, 1, 0.5, 0);
		colorFun->AddRGBPoint(3001, 1, 1, 1);
		colorFun->AddRGBPoint(3120, 1, 1, 1);
		colorFun->AddRGBPoint(3899, 1, 1, 1);
		volumeProperty->SetScalarOpacity(opacityFun);
		volumeProperty->SetColor(colorFun);
	}
	else if (value == 2)
	{
		volumeProperty->SetInterpolationTypeToLinear();
		//volumeProperty->ShadeOn();  //打开或者关闭阴影测试
		volumeProperty->SetAmbient(0.6);
		volumeProperty->SetDiffuse(0.6);  //漫反射
		vtkSmartPointer<vtkPiecewiseFunction> opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
		opacityFun->AddPoint(-1100, 0);
		opacityFun->AddPoint(-385, 0);
		opacityFun->AddPoint(205, 0.2);
		opacityFun->AddPoint(922, 0.2);
		opacityFun->AddPoint(1638, 0.3);
		opacityFun->AddPoint(3072, 0.5);
		opacityFun->AddPoint(3899, 0.5);
		vtkSmartPointer<vtkColorTransferFunction> colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
		colorFun->AddRGBPoint(-1100, 0, 0, 0);
		colorFun->AddRGBPoint(-385, 0.15, 0.15, 0.15);
		colorFun->AddRGBPoint(1638, 1, 1, 1);
		colorFun->AddRGBPoint(3899, 1, 1, 1);
		volumeProperty->SetScalarOpacity(opacityFun);
		volumeProperty->SetColor(colorFun);
	}
	volumeRenderer->GetRenderWindow()->Render();

}

void simpleTK::constructMPR2(vtkSmartPointer<vtkImageData> imageData) {

	for (int i = 0; i < 3; i++)
	{	
		// make them all share the same reslice cursor object.
		vtkResliceCursorLineRepresentation* rep = vtkResliceCursorLineRepresentation::SafeDownCast(
			riw[i]->GetResliceCursorWidget()->GetRepresentation());
		riw[i]->SetResliceCursor(riw[0]->GetResliceCursor());
		
		rep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);

		riw[i]->SetInputData(mReader->GetOutput());
		riw[i]->SetSliceOrientation(i);
		riw[i]->SetResliceModeToOblique();
	}
	vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance(0.005);

	vtkSmartPointer<vtkProperty> ipwProp = vtkSmartPointer<vtkProperty>::New();
	ipwProp->SetOpacity(0.01);

	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	//vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	//ui.imageViewerWidget3->setRenderWindow(renderWindow);
	//ui.imageViewerWidget3->renderWindow()->AddRenderer(ren);
	vtkRenderWindowInteractor* iren = ui.imageViewerWidget3->interactor();

	for (int i = 0; i < 3; i++)
	{
		planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
		planeWidget[i]->SetInteractor(iren);
		planeWidget[i]->SetPicker(picker);
		planeWidget[i]->RestrictPlaneToVolumeOn();
		double color[3] = { 0, 0, 0 };
		color[i] = 1;
		planeWidget[i]->GetPlaneProperty()->SetColor(color);

		//color[0] /= 4.0;
		//color[1] /= 4.0;
		//color[2] /= 4.0;
		//riw[i]->GetRenderer()->SetBackground(color);

		planeWidget[i]->SetTexturePlaneProperty(ipwProp);
		planeWidget[i]->TextureInterpolateOff();
		planeWidget[i]->SetResliceInterpolateToLinear();
		int imageDims[3];
		imageData->GetDimensions(imageDims);
		planeWidget[i]->SetInputData(imageData);
		planeWidget[i]->SetPlaneOrientation(i);
		planeWidget[i]->SetSliceIndex(imageDims[i] / 2);
		planeWidget[i]->DisplayTextOn();
		planeWidget[i]->SetDefaultRenderer(ren);
		planeWidget[i]->SetWindowLevel(4095, 1024);
		planeWidget[i]->On();
		planeWidget[i]->InteractionOn();
	}

	vtkSmartPointer<vtkResliceCursorCallback> cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();

	for (int i = 0; i < 3; i++)
	{
		cbk->IPW[i] = planeWidget[i];
		cbk->RCW[i] = riw[i]->GetResliceCursorWidget();
		riw[i]->GetResliceCursorWidget()->AddObserver(
			vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);
		riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::WindowLevelEvent, cbk);
		riw[i]->GetResliceCursorWidget()->AddObserver(
			vtkResliceCursorWidget::ResliceThicknessChangedEvent, cbk);
		riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::ResetCursorEvent, cbk);
		riw[i]->GetInteractorStyle()->AddObserver(vtkCommand::WindowLevelEvent, cbk);
		riw[i]->AddObserver(vtkResliceImageViewer::SliceChangedEvent, cbk);

		// Make them all share the same color map.
		riw[i]->SetLookupTable(riw[0]->GetLookupTable());
		planeWidget[i]->GetColorMap()->SetLookupTable(riw[0]->GetLookupTable());
		// planeWidget[i]->GetColorMap()->SetInput(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap()->GetInput());
		planeWidget[i]->SetColorMap(
			riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap());
	}


	ui.imageViewerWidget0->show();
	ui.imageViewerWidget1->show();
	ui.imageViewerWidget2->show();

	riw[0]->GetRenderer()->ResetCamera();
	riw[1]->GetRenderer()->ResetCamera();
	riw[2]->GetRenderer()->ResetCamera();

	riw[0]->GetRenderWindow()->Render();
	riw[1]->GetRenderWindow()->Render();
	riw[2]->GetRenderWindow()->Render();
}

void simpleTK::constructMPR3(vtkSmartPointer<vtkImageData> imageData) {

	vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance(0.005);

	vtkSmartPointer<vtkProperty> ipwProp = vtkSmartPointer<vtkProperty>::New();
	ipwProp->SetOpacity(0.01);

	vtkRenderWindowInteractor* iren = ui.imageViewerWidget3->interactor();


	for (int i = 0; i < 3; i++)
	{
		planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
		planeWidget[i]->SetInteractor(iren);
		planeWidget[i]->SetPicker(picker);
		planeWidget[i]->RestrictPlaneToVolumeOn();
		double color[3] = { 0, 0, 0 };
		color[i] = 1;
		planeWidget[i]->GetPlaneProperty()->SetColor(color);

		//color[0] /= 4.0;
		//color[1] /= 4.0;
		//color[2] /= 4.0;
		//riw[i]->GetRenderer()->SetBackground(color);

		planeWidget[i]->SetTexturePlaneProperty(ipwProp);
		planeWidget[i]->TextureInterpolateOff();
		planeWidget[i]->SetResliceInterpolateToLinear();
		int imageDims[3];
		imageData->GetDimensions(imageDims);
		planeWidget[i]->SetInputData(imageData);
		planeWidget[i]->SetPlaneOrientation(i);
		planeWidget[i]->SetSliceIndex(imageDims[i] / 2);
		planeWidget[i]->DisplayTextOn();
		planeWidget[i]->SetDefaultRenderer(volumeRenderer);
		planeWidget[i]->SetWindowLevel(4095, 1024);
		planeWidget[i]->On();
		planeWidget[i]->InteractionOn();
	}

	planeWidget[1]->SetLookupTable(planeWidget[0]->GetLookupTable());
	planeWidget[2]->SetLookupTable(planeWidget[0]->GetLookupTable());

	// 当坐标系发生改变时即调用vtkResliceCursor来进行图像切分并进行更新到vtkRenderer对象中
	vtkSmartPointer<vtkResliceCursorCallback> cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();
	vtkSmartPointer< vtkResliceCursor > resliceCursor = vtkSmartPointer< vtkResliceCursor >::New();
	resliceCursor->SetCenter(imageData->GetCenter());
	resliceCursor->SetThickMode(0);
	resliceCursor->SetThickness(10, 10, 10);
	resliceCursor->SetImage(imageData);

	vtkSmartPointer< vtkResliceCursorWidget > resliceCursorWidget[3];
	vtkSmartPointer< vtkResliceCursorLineRepresentation > resliceCursorRep[3];




	double viewUp[3][3] = { { 0, 0, -1 }, { 0, 0, 1 }, { 0, 1, 0 } };
	for (int i = 0; i < 3; i++)
	{
		resliceCursorWidget[i] = vtkSmartPointer< vtkResliceCursorWidget >::New();
		resliceCursorWidget[i]->SetInteractor(iren);

		resliceCursorRep[i] = vtkSmartPointer< vtkResliceCursorLineRepresentation >::New();
		resliceCursorWidget[i]->SetRepresentation(resliceCursorRep[i]);
		resliceCursorRep[i]->GetResliceCursorActor()->GetCursorAlgorithm()->SetResliceCursor(resliceCursor);
		resliceCursorRep[i]->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);

		const double minVal = imageData->GetScalarRange()[0];
		if (vtkImageReslice *reslice = vtkImageReslice::SafeDownCast(resliceCursorRep[i]->GetReslice()))
		{
			reslice->SetBackgroundColor(minVal, minVal, minVal, minVal);
		}

		resliceCursorWidget[i]->SetDefaultRenderer(imageViewer[i]->GetRenderer());
		resliceCursorWidget[i]->SetEnabled(1);

		imageViewer[i]->GetRenderer()->GetActiveCamera()->SetFocalPoint(0, 0, 0);
		double camPos[3] = { 0, 0, 0 };
		camPos[i] = 1;
		imageViewer[i]->GetRenderer()->GetActiveCamera()->SetPosition(camPos);
		imageViewer[i]->GetRenderer()->GetActiveCamera()->ParallelProjectionOn();
		imageViewer[i]->GetRenderer()->GetActiveCamera()->SetViewUp(viewUp[i][0], viewUp[i][1], viewUp[i][2]);
		imageViewer[i]->GetRenderer()->ResetCamera();
		cbk->IPW[i] = planeWidget[i];
		cbk->RCW[i] = resliceCursorWidget[i];
		resliceCursorWidget[i]->AddObserver(vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);
		double range[2];
		imageData->GetScalarRange(range);
		resliceCursorRep[i]->SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);
		planeWidget[i]->SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);
		resliceCursorRep[i]->SetLookupTable(resliceCursorRep[0]->GetLookupTable());
		planeWidget[i]->GetColorMap()->SetLookupTable(resliceCursorRep[0]->GetLookupTable());



		//_rendererAxial->GetRenderWindow()->GetInteractor()->SetInteractorStyle(styleAxial);
		imageViewer[i]->GetRenderer()->ResetCamera();
		imageViewer[i]->GetRenderer()->GetRenderWindow()->Render();
		//imageViewer[i]->GetRenderer()->Render();
	}


}


// void simpleTK::constructMPR(double *center)
// {
// 	double spacing[3];//间隔
// 	mReader->GetOutput()->GetSpacing(spacing);
// 	double origin[3];//原点
// 	mReader->GetOutput()->GetOrigin(origin);
// 	int extent[6];//范围
// 	mReader->GetOutput()->GetExtent(extent);
// 	std::cout << "spacings:" << spacing[0] << " " << spacing[1] << " " << spacing[2] << std::endl;
// 	std::cout << "origin:" << origin[0] << " " << origin[1] << " " << origin[2] << std::endl;
// 	std::cout << "extent:" << extent[0] << " " << extent[1] << " " << extent[2] << " " << extent[3] << " " << extent[4] << " " << extent[5] << std::endl;
// 	//double center[3];
// 	/*center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
// 	center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
// 	center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);*/
// 	double xmin = origin[0] + spacing[0] * (extent[0] - 0.5);
// 	double xmax = origin[0] + spacing[0] * (extent[1] + 0.5);
// 	double ymin = origin[1] + spacing[1] * (extent[2] - 0.5);
// 	double ymax = origin[1] + spacing[1] * (extent[3] + 0.5);
// 	double zmin = origin[2] + spacing[2] * (extent[4] - 0.5);
// 	double zmax = origin[2] + spacing[2] * (extent[5] + 0.5);
// 	double xcenter = center[0];
// 	double ycenter = center[1];
// 	double zcenter = center[2];
// 	std::cout << "xmin:" << xmin << "  xmax:" << xmax << std::endl
// 		<< "ymin:" << ymin << "  ymax:" << ymax << std::endl
// 		<< "zmin:" << zmin << "  zmax:" << zmax << std::endl;
// 	// axial
// 	mpSlicePlanes[0]->SetOrigin(xmin, ymin, zcenter);
// 	mpSlicePlanes[0]->SetPoint1(xmax, ymin, zcenter);
// 	mpSlicePlanes[0]->SetPoint2(xmin, ymax, zcenter);
// 
// 	// coronal
// 	mpSlicePlanes[1]->SetOrigin(xmin, ycenter, zmax);
// 	mpSlicePlanes[1]->SetPoint1(xmax, ycenter, zmax);
// 	mpSlicePlanes[1]->SetPoint2(xmin, ycenter, zmin);
// 
// 	// sagital
// 	mpSlicePlanes[2]->SetOrigin(xcenter, ymin, zmax);
// 	mpSlicePlanes[2]->SetPoint1(xcenter, ymax, zmax);
// 	mpSlicePlanes[2]->SetPoint2(xcenter, ymin, zmin);
// 
// 	for (int i = 0; i < 3; i++)
// 	{
// 		reslice[i] = vtkSmartPointer<vtkImageReslice>::New();
// 		reslice[i]->SetInputConnection(mReader->GetOutputPort());
// 		reslice[i]->SetOutputDimensionality(2);
// 		reslice[i]->SetInterpolationModeToNearestNeighbor();
// 
// 		double planeAxis1[3];
// 		double planeAxis2[3];
// 		GetVector1(mpSlicePlanes[i], planeAxis1);
// 		GetVector2(mpSlicePlanes[i], planeAxis2);
// 
// 		std::cout << "planeAxis1:" << planeAxis1[0] << "," << planeAxis1[1] << "," << planeAxis1[2] << std::endl;
// 		std::cout << "planeAxis2:" << planeAxis2[0] << "," << planeAxis2[1] << "," << planeAxis2[2] << std::endl;
// 
// 		double planeSizeX = vtkMath::Normalize(planeAxis1);
// 		double planeSizeY = vtkMath::Normalize(planeAxis2);
// 
// 		std::cout << "planeSizeX:" << planeSizeX << " planeSizeY:" << planeSizeY << std::endl;
// 
// 		double normal[3];
// 		mpSlicePlanes[i]->GetNormal(normal);
// 		std::cout << "normal:" << normal[0] << "," << normal[1] << "," << normal[2] << std::endl;
// 
// 		vtkSmartPointer<vtkMatrix4x4> resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();
// 		resliceAxes->Identity();
// 		for (int i = 0; i < 3; i++)
// 		{
// 			resliceAxes->SetElement(0, i, planeAxis1[i]);
// 			resliceAxes->SetElement(1, i, planeAxis2[i]);
// 			resliceAxes->SetElement(2, i, normal[i]);
// 		}
// 
// 		double planeOrigin[4];
// 		mpSlicePlanes[i]->GetOrigin(planeOrigin);
// 		planeOrigin[3] = 1.0;
// 
// 		std::cout << "planeOrigin:" << planeOrigin[0] << "," << planeOrigin[1] << "," << planeOrigin[2] << std::endl;
// 
// 		resliceAxes->Transpose();
// 		resliceAxes->SetElement(0, 3, planeOrigin[0]);
// 		resliceAxes->SetElement(1, 3, planeOrigin[1]);
// 		resliceAxes->SetElement(2, 3, planeOrigin[2]);
// 
// 		reslice[i]->SetResliceAxes(resliceAxes);
// 
// 		double spacingX = fabs(planeAxis1[0] * spacing[0]) + fabs(planeAxis1[1] * spacing[1]) + fabs(planeAxis1[2] * spacing[2]);
// 		double spacingY = fabs(planeAxis2[0] * spacing[0]) + fabs(planeAxis2[1] * spacing[1]) + fabs(planeAxis2[2] * spacing[2]);
// 
// 		// make sure we're working with valid values
// 		double realExtentX = (spacingX == 0) ? VTK_INT_MAX : planeSizeX / spacingX;
// 
// 		int extentX;
// 		// Sanity check the input data:
// 		// * if realExtentX is too large, extentX will wrap
// 		// * if spacingX is 0, things will blow up.
// 		if (realExtentX > (VTK_INT_MAX >> 1))
// 		{
// 			//vtkErrorMacro(<< "Invalid X extent: " << realExtentX);
// 			extentX = 0;
// 		}
// 		else
// 		{
// 			extentX = 1;
// 			while (extentX < realExtentX)
// 			{
// 				extentX = extentX << 1;
// 			}
// 		}
// 
// 		// make sure extentY doesn't wrap during padding
// 		double realExtentY = (spacingY == 0) ? VTK_INT_MAX : planeSizeY / spacingY;
// 
// 		int extentY;
// 		if (realExtentY > (VTK_INT_MAX >> 1))
// 		{
// 			//vtkErrorMacro(<< "Invalid Y extent: " << realExtentY);
// 			extentY = 0;
// 		}
// 		else
// 		{
// 			extentY = 1;
// 			while (extentY < realExtentY)
// 			{
// 				extentY = extentY << 1;
// 			}
// 		}
// 
// 		double outputSpacingX = (extentX == 0) ? 1.0 : planeSizeX / extentX;
// 		double outputSpacingY = (extentY == 0) ? 1.0 : planeSizeY / extentY;
// 		reslice[i]->SetOutputSpacing(outputSpacingX, outputSpacingY, 1);
// 		reslice[i]->SetOutputOrigin(0.5 * outputSpacingX, 0.5 * outputSpacingY, 0);
// 		reslice[i]->SetOutputExtent(0, extentX - 1, 0, extentY - 1, 0, 0);
// 		reslice[i]->Update();
// 
// 		vtkSmartPointer<vtkImageData> imagedata = reslice[i]->GetOutput();
// 
// 		mImageViewer[i]->SetInputData(imagedata);
// 		mImageViewer[i]->SetColorLevel(1024);
// 		mImageViewer[i]->SetColorWindow(4095);
// 		mImageViewer[i]->GetRenderer()->ResetCamera();
// 		mImageViewer[i]->Render();
// 	}
// 	for (int i = 0; i < 3; i++)
// 	{
// 		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
// 		mapper->SetInputConnection(mpSlicePlanes[i]->GetOutputPort());
// 
// 		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
// 		actor->SetMapper(mapper);
// 
// 		vtkSmartPointer<vtkProperty> property = actor->GetProperty();
// 		property->SetRepresentationToWireframe();
// 		property->SetLighting(0);
// 		mpSlicePlaneActors[i] = actor;
// 	}
// 
// 	mpSlicePlaneActors[0]->GetProperty()->SetColor(mpColors->GetColor3d("Green").GetData());
// 	mpSlicePlaneActors[1]->GetProperty()->SetColor(mpColors->GetColor3d("Blue").GetData());
// 	mpSlicePlaneActors[2]->GetProperty()->SetColor(mpColors->GetColor3d("Red").GetData());
// 
// 
// 	mImageViewerRenderer[0]->GetActiveCamera()->Zoom(3.5);
// 	mRenderWindows[0]->Render();
// 	mImageViewerRenderer[1]->GetActiveCamera()->Zoom(3.5);
// 	mRenderWindows[1]->Render();
// 	mImageViewerRenderer[2]->GetActiveCamera()->Zoom(3.5);
// 	mRenderWindows[2]->Render();
// }

//  void simpleTK::GetVector1(vtkPlaneSource* planeSource, double v1[3])
// {
// 	double* p1 = planeSource->GetPoint1();
// 	double* o = planeSource->GetOrigin();
// 	v1[0] = p1[0] - o[0];
// 	v1[1] = p1[1] - o[1];
// 	v1[2] = p1[2] - o[2];
// }
// void simpleTK::GetVector2(vtkPlaneSource* planeSource, double v2[3])
// {
// 	double* p2 = planeSource->GetPoint2();
// 	double* o = planeSource->GetOrigin();
// 	v2[0] = p2[0] - o[0];
// 	v2[1] = p2[1] - o[1];
// 	v2[2] = p2[2] - o[2];
// }