#pragma once

#include <vtkMatrix4x4.h>

class vtkImageResliceToColors;


	class vtkResliceRotator
	{
	public:
		vtkResliceRotator() = default;
		~vtkResliceRotator() = default;
		static void rotatePlane(vtkMatrix4x4* t_sourceMatrix,
			vtkMatrix4x4* t_destinationMatrix, double t_angle);
	};

