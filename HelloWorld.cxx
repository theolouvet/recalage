#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageRegionIterator.h"
#include <iostream>
#include <itkImageFileWriter.h>
#include "itkRecursiveSeparableImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegistrationMethod.h"

#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkResampleImageFilter.h"

int main(){
	// Creer une image 3D avec un type de pixel unsigned short
/*	
    	std::cout << "ITK Hello World !" << std::endl;
	

	
	
	typedef itk::Image< unsigned char, 2 > InputImageType;
	

	typedef itk::ImageFileReader<InputImageType > ReaderType;
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName( "data/lena.jpg" );
	reader->Update();
	InputImageType::Pointer image = reader->GetOutput();
	image->Print(std::cout);
	InputImageType::IndexType index;
	index[0]=10;
	index[1]=10;
	image->GetPixel(index);

	std::cout<<(float)image->GetPixel(index)<< std::endl;

	InputImageType::RegionType region;
  	InputImageType::RegionType::SizeType  size;
//	ImageType::RegionType::IndexType index2;
 	index[0] = 10;
	index[1] = 10;
	size[0] = 1;
	size[1] = 1;
	region.SetSize(size);
	region.SetIndex(index);
	
	typedef itk::ImageRegionIterator<InputImageType> ImageIterator;
	ImageIterator it(image, region);

	std::cout<<(float)it.Get()<<std::endl;

	//Image filtrage
	
	typedef itk::Image<unsigned char, 2> OutputImageType; 
	typedef itk::RecursiveGaussianImageFilter<InputImageType, OutputImageType> FilterType;
	FilterType::Pointer ft = FilterType::New();
	
	const double sigma = 20;// std::stod(argv[3]);
	ft->SetSigma(sigma);
	ft->SetInput(image);
	ft->Update();
	
	

	
 
	typedef itk::ImageFileWriter<OutputImageType> WriterType;
	
	
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName("imagesortie.jpg");
	writer->SetInput(ft->GetOutput());

	try{
		writer->Update();
		writer->SetFileName("imagesortie.bmp");
		writer->Update();
	}
	catch (itk::ExceptionObject & error)
	{
		std::cerr << "Error: " << error << std::endl;
		
	}

	//recalage
*/

        typedef itk::Image< unsigned char, 2 > InputImageType;
	

	typedef itk::ImageFileReader<InputImageType > ReaderType;

	ReaderType::Pointer reader2 = ReaderType::New();
	reader2->SetFileName( "data/BrainProtonDensitySliceShifted13x17y.png" );
	reader2->Update();
	InputImageType::Pointer image_rec = reader2->GetOutput();

        ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName( "data/BrainProtonDensitySliceBorder20.png" );
	reader->Update();
	InputImageType::Pointer image_rec2 = reader->GetOutput();


	

	
	typedef itk::TranslationTransform<double, 2> TransformType;
	TransformType::Pointer transform = TransformType::New(); 	
	
	typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
	OptimizerType::Pointer optimizer = OptimizerType::New();
 	optimizer->SetMaximumStepLength(4.00);
  	optimizer->SetMinimumStepLength(0.01);
  	optimizer->SetNumberOfIterations(200);
	optimizer->MaximizeOff();

        typedef itk::MeanSquaresImageToImageMetric<InputImageType, InputImageType> MetricType;
	MetricType::Pointer metric = MetricType::New();

	typedef itk::ImageRegistrationMethod<InputImageType,InputImageType> RegistrationType;
	RegistrationType::Pointer registration = RegistrationType::New();

	typedef itk::LinearInterpolateImageFunction<InputImageType, double> interpolatorType;
	interpolatorType::Pointer interpolator = interpolatorType::New();

	registration->SetOptimizer(optimizer);
	registration->SetTransform(transform);
	registration->SetInterpolator( interpolator );
	registration->SetMetric( metric );

	registration->SetFixedImage( image_rec );
	registration->SetMovingImage( image_rec2 );
	registration->SetFixedImageRegion(image_rec->GetBufferedRegion() );
	typedef RegistrationType::ParametersType ParametersType;
	ParametersType initialParameters( transform->GetNumberOfParameters() );
	initialParameters[0] = 0.0;// Initial offset in mm along X
	initialParameters[1] = 0.0;// Initial offset in mm along Y
	
	registration->SetInitialTransformParameters( initialParameters );
	try
	{
		registration->StartRegistration();
	}
	catch( itk::ExceptionObject & err )
	{
		std::cout << "ExceptionObject caught !" << std::endl;
		std::cout << err << std::endl;
	}

	ParametersType finalParameters = registration->GetLastTransformParameters();
	TransformType::Pointer finalTransform = TransformType::New();

	std::cout << finalParameters << std::endl;

	finalTransform->SetParameters( finalParameters );


        typedef itk::ResampleImageFilter<InputImageType,InputImageType> ResampleFilterType;
	ResampleFilterType::Pointer resample = ResampleFilterType::New();
	resample->SetTransform( finalTransform );
	resample->SetInput( image_rec2 );
	resample->SetSize( image_rec->GetLargestPossibleRegion().GetSize() );
	resample->SetOutputOrigin( image_rec->GetOrigin() );
	resample->SetOutputSpacing( image_rec->GetSpacing() );
	resample->SetDefaultPixelValue( 0 );



	typedef itk::ImageFileWriter<InputImageType> WriterType;
	
	
	WriterType::Pointer writer = WriterType::New();
	try{
		writer->SetInput(resample->GetOutput());
		writer->SetFileName("resample.jpg");
		writer->Update();
		writer->SetInput(image_rec);
		writer->SetFileName("reg1.jpg");
		writer->Update();
		writer->SetInput(image_rec2);
		writer->SetFileName("reg2.jpg");
		writer->Update();
	}
	catch (itk::ExceptionObject & error)
	{
		std::cerr << "Error: " << error << std::endl;
		
	}
	
	
	return 0;
}































