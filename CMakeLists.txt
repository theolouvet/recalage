cmake_minimum_required(VERSION 2.6)
project(HelloWorld)
# Find ITK.
find_package(ITK)

if(ITK_FOUND)
	include(${ITK_USE_FILE})
else(ITK_FOUND)
	message(FATAL_ERROR
		"Cannot build without ITK. Please set ITK_DIR.")
endif(ITK_FOUND)

add_executable(HelloWorld HelloWorld.cxx )
target_link_libraries(HelloWorld ITKCommon ITKIO ITKNumerics)
