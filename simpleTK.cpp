#include "simpleTK.h"

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
	m_mprMaker = std::make_unique<MPRMaker>();
	for (int i = 0; i < 3; i++)
	{
		//mImageViewer[i] = vtkSmartPointer<vtkImageViewer2>::New();
		//mImageViewerRenderer[i] = vtkSmartPointer<vtkRenderer>::New();
		//mImageViewerWindowInteractor[i] = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		mRenderWindows[i] = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

		//mImageViewer[i]->SetRenderWindow(mRenderWindows[i]);
		//mImageViewer[i]->SetRenderer(mImageViewerRenderer[i]);
		//mImageViewer[i]->SetupInteractor(mImageViewerWindowInteractor[i]);
	}

	ui.imageViewerWidget0->setRenderWindow(mRenderWindows[0]);
	ui.imageViewerWidget1->setRenderWindow(mRenderWindows[1]);
	ui.imageViewerWidget2->setRenderWindow(mRenderWindows[2]);
	//ui.imageViewerWidget3->SetRenderWindow(mImageViewer[3]->GetRenderWindow());

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
void simpleTK::openFolder()
{

	//QString filePath = QFileDialog::getExistingDirectory(this, QStringLiteral("Open directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	//if (filePath.isEmpty())
		//return;

	// QString filePath = "D:/workspace/simpleTK/res/SLC";
	// mReader->SetDirectoryName(filePath.toStdString().c_str());
	// mReader->Update();
	// double* center = mReader->GetOutput()->GetCenter();

	m_path = "D:/workspace/simpleTK/res/SLC";
	vtkSmartPointer<vtkDICOMDirectory> dicomDir = vtkSmartPointer<vtkDICOMDirectory>::New();
	dicomDir->SetDirectoryName(m_path.c_str());
	dicomDir->RequirePixelDataOn();
	dicomDir->Update();

	if (dicomDir->GetNumberOfSeries() > 0) {
		mReader->SetFileNames(dicomDir->GetFileNamesForSeries(0));
		mReader->Update();

	}
	//double* center = mReader->GetOutput()->GetCenter();
	// mViewImage2D[0]->SetInput("Axial");
	// mViewImage2D[0]->GetTextProperty()->SetFontSize(20);
	// mViewImage2D[0]->GetTextProperty()->SetColor(1, 0, 0);
	// mViewImage2D[0]->SetDisplayPosition(0 ,0);
	// mImageViewerRenderer[0]->AddActor(mViewImage2D[0]);
	// mViewImage2D[1]->SetInput("Sagittal");
	// mViewImage2D[1]->GetTextProperty()->SetFontSize(20);
	// mViewImage2D[1]->GetTextProperty()->SetColor(0, 0, 1);
	// mViewImage2D[1]->SetDisplayPosition(0, 0);
	// mImageViewerRenderer[1]->AddActor(mViewImage2D[1]);
	// mViewImage2D[2]->SetInput("Coronal");
	// mViewImage2D[2]->GetTextProperty()->SetFontSize(20);
	// mViewImage2D[2]->GetTextProperty()->SetColor(0, 1, 0);
	// mViewImage2D[2]->SetDisplayPosition(0, 0);
	// mImageViewerRenderer[2]->AddActor(mViewImage2D[2]);

	constructMPR();
}
void simpleTK::constructMPR()
{
	if (!m_mprMaker)
	{
		return;
	}
	m_mprMaker->SetRenderWindows(
		ui.imageViewerWidget1->renderWindow(),
		ui.imageViewerWidget2->renderWindow(),
		ui.imageViewerWidget0->renderWindow());

	m_mprMaker->createMPR(mReader);

}

// void simpleTK::constructMPR(double *center)
// {
// 	double spacing[3];//¼ä¸ô
// 	mReader->GetOutput()->GetSpacing(spacing);
// 	double origin[3];//Ô­µã
// 	mReader->GetOutput()->GetOrigin(origin);
// 	int extent[6];//·¶Î§
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