#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"
#include "shapes/sphere.h"
#include "cameras/ortographic.h"
#include "cameras/perspective.h"

void transformationsExercise()
{
    /* *************** */
    /* TRANSFORMATIONS */
    /* *************** */
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";

    std::cout << separatorStar << "Construct and visualize transforms" << separatorStar << std::endl;

    // Identity Matrix
    Matrix4x4 idMat; // By default, a matrix4x4 is initialized with the identity matrix
    std::cout << "The content of matrix idMat is: \n" << std::endl;
    std::cout << idMat << separator << std::endl;

    // Translation Matrix
    double deltaX, deltaY, deltaZ;
    deltaX = 1; deltaY = -2; deltaZ = 3;
    Vector3D delta(deltaX, deltaY, deltaZ);
    Matrix4x4 translationMat = Matrix4x4::translate(delta);
    std::cout << "The content of matrix translationMat is: \n" << std::endl;
    std::cout << translationMat << separator << std::endl;

    // Scale Matrix
    //(...)
	double Sx, Sy, Sz;
	Sx = 2; Sy = 1; Sz = -1;
	Vector3D scaling(Sx, Sy, Sz);
	Matrix4x4 scaleMatrix = Matrix4x4::scale(scaling);
    std::cout << "The content of matrix scaleMatrix is: \n" << std::endl;
    std::cout << scaleMatrix << separator << std::endl;

    // Rotate around X Matrix
    double angleInDegrees = 60;
    double thetaInRadians = Utils::degreesToRadians(angleInDegrees);
	Vector3D x_axis(1,0,0);
	Matrix4x4 rotateXMatrix = Matrix4x4::rotate(thetaInRadians,x_axis);
    std::cout << "The content of matrix rotateXMatrix is: \n" << std::endl;
    std::cout << rotateXMatrix << separator << std::endl;

    // Rotate around an arbitrary axis Matrix
	angleInDegrees = 30;
	thetaInRadians = Utils::degreesToRadians(angleInDegrees);
	Vector3D arbitrary_axis(1, 1, 1);
	Matrix4x4 rotateArtitraryAxisMatrix = Matrix4x4::rotate(thetaInRadians, arbitrary_axis);
    std::cout << "The content of matrix rotateArtitraryAxisMatrix is: \n" << std::endl;
    std::cout << rotateArtitraryAxisMatrix << separator << std::endl;

    // Transposed and Inversion
    std::cout << separatorStar << "Inverting and Transposing a Matrix" << separatorStar << std::endl;
    Matrix4x4 translationMatTransposed;
    translationMat.transpose(translationMatTransposed);
    std::cout << "The transpose of matrix \n\n" << translationMat << "\n is \n\n"
              << translationMatTransposed << separator << std::endl;

    Matrix4x4 inverseTranslationMat;
	translationMat.inverse(inverseTranslationMat);
    std::cout << "The inverse of matrix \n\n" << translationMat << "\n is \n\n" << inverseTranslationMat << std::endl;
    std::cout << "And their multiplication should thus give the identity matrix:\n\n";
	
	Matrix4x4 comprovationIdentityMatrix;
	comprovationIdentityMatrix = translationMat*inverseTranslationMat;
	std::cout << comprovationIdentityMatrix << "\n\n" << std::endl;


    // Combine here some transforms, and visualize the result
    std::cout << separatorStar << "Combine transforms and visualize the result" << separatorStar << std::endl;

    Matrix4x4 scaleTranslate = scaleMatrix*translationMat; 
    std::cout << "The content of matrix scaleTranslate is: \n" << std::endl;
    std::cout << scaleTranslate << separator << std::endl;

    Matrix4x4 translateScale = translationMat*scaleMatrix;
    std::cout << "The content of matrix translateScale is: \n" << std::endl;
    std::cout << translateScale << separator << std::endl;
}

void normalTransformExercise()
{
    std::string separator = "\n----------------------------------------------\n";

    // NOTE: points, vectors and normals are represented, in this framework, using
    //  the Vector3D class. It is up to the programmer to take care of whether the
    //  Vector3D is representing a point, a vector or a normal. For example, when
    //  applying a transform using a Matrix4x4, the programmer must choose which
    //  "applyTransform" method of the namespace Matrix4x4 should be used.

    Vector3D v(1, 1, 0);
    Vector3D n(-1.0/sqrt(2), 1.0/sqrt(2), 0);
    Matrix4x4 S = Matrix4x4::scale(Vector3D(2, 1, 1));

    std::cout << "Vector v = " << v << "\n" << std::endl;
    std::cout << "Normal n = " << n << "\n" << std::endl;
    std::cout << "Scale Matrix: \n" << std::endl;
    std::cout << S << std::endl;

	///Vector v transformat
    Vector3D vTransformed = S.transformVector(v);
    std::cout << "Vector v\' = " << vTransformed << "\n" << std::endl;

	///Vector n transformat malament
	Vector3D nWrongTransformed = S.transformVector(n);
	std::cout << "Vector n\' = " << nWrongTransformed << "\n" << std::endl;
	
	///Comprovem que el dotProduct no es 0 
	double dotOperation;
	dotOperation = dot(nWrongTransformed, vTransformed);
	std::cout << "dot( n\', v\') = " << dotOperation << "\n" << std::endl;

	///Calculem n transformada de manera correcte (n'=(St)^-1 * n)
	Vector3D nTransformed;
	Matrix4x4 sTransposed, sInversed;
	S.transpose(sTransposed);
	sTransposed.inverse(sInversed);
	nTransformed = sInversed.transformVector(n);

	std::cout << "Vector n\' = " << nTransformed << "\n" << std::endl;
	
	dotOperation = dot(nTransformed, vTransformed);
	std::cout << "dot( n\', v\') = " << dotOperation << "\n" << std::endl;

}

std::vector<double> NDC(int col, int lin, size_t resX, size_t resY) {
	double x, y;
	x = (col + 0.5) / resX;
	y = (lin + 0.5) / resY;

	std::vector<double> pndc;
	pndc.push_back(x);
	pndc.push_back(y);

	return pndc;
}

void paintingAnImageExercise()
{
    // Define the film (i.e., image) resolution
    size_t resX, resY;
    resX = 512;
    resY = 512;
    Film film(resX, resY);
	std::vector<double> pndc;


    for(unsigned int col = 0; col < resX; col++)
        {
            for(unsigned int row = 0; row < resY; row++)
            {
				pndc = NDC(col, row, resX, resY);
				Vector3D color(pndc.at(0), pndc.at(1), 0);
                film.setPixelValue(col, row, color);
            }
        }

    // Save the final result to file
    film.save();
}

void filteringAnImageExercise()
{
    // Create two instances of the film class with the same resolution
    int resX, resY;
    resX = 512;
    resY = 512;
    Film f1(resX, resY);
    Film f2(resX, resY);

    // Create the original image
    //  Draw a circle centered at centerX, centerY (in pixels, image space)
    //   and with ray r (also in pixels)
    int centerX = resX / 2;
    int centerY = resY / 2;
    int r = std::min(centerX, centerY)/2;
    for(int lin=0; lin<resX; lin++)
    {
        for(int col=0; col<resY; col++)
        {
            // Use the equation of the sphere to determine the pixel color
            if( (lin-centerX)*(lin-centerX) + (col-centerY)*(col-centerY) < r*r )
                f1.setPixelValue(col, lin, Vector3D(1, 1, 0));
        }
    }

    // Filter-related variables
    // Declare here your filter-related variables
	int size, halfSize;
	size = 9;
	halfSize = ceil(size / 2)-1;
	int iterations = 100;
	Film *aux1,*aux2, *aux3;
	aux1 = &f1;
	aux2 = &f2;
	Vector3D filter = Vector3D();
	int avg;
	avg= 0;
    // Implement here your image filtering algorithm
	for (int i = 0; i < iterations; i++) {
		for (int lin = 0; lin < resX; lin++)
		{
			for (int col = 0; col < resY; col++)
			{

				if (lin > halfSize && col > halfSize && lin<resX-halfSize && col<resY-halfSize){
					for (int x = lin - halfSize; x <= lin + halfSize; x++) {
						for (int y = col - halfSize; y <= col + halfSize; y++) {
							filter +=aux1->getPixelValue(x,y);
							avg += 1;
						}
					}
					filter /= avg;
					aux2->setPixelValue(lin, col, filter);
					//aux->setPixelValue(lin, col, filter);
					filter -= filter;
					avg = 0;
				}
				else {
					aux2->setPixelValue(lin, col, Vector3D(0,0,0));
				}
				
				//Vector3D pos= f1.getPixelValue(lin, col);
			}
		}
		aux3 = aux1;
		aux1 = aux2;
		aux2 = aux3;
		//f1.clearData();
		//aux.clearData();
	}
	aux1->save();

    // DO NOT FORGET TO SAVE YOUR IMAGE!
    //(...)
}

void completeSphereClassExercise()
{
    // Make your intersection tests here
    // (....)
}

void eqSolverExercise()
{
    EqSolver solver;
    rootValues roots;

    double A, B, C;

    // (...)

	bool hasRoots = true;
    //bool hasRoots = solver.rootQuadEq(A, B, C, roots);

    if(!hasRoots)
    {
        std::cout << "Equation has no real roots!" << std::endl;
    }
    else
    {
        // SHOW THE SOLUTIONS OF THE EQUATION
        // (...)
    }
}

void raytrace()
{
    // Define the film (i.e., image) resolution
    size_t resX, resY;
    resX = 512;
    resY = 512;
    Film film(resX, resY);

    /* ******************* */
    /* Orthographic Camera */
    /* ******************* */
    Matrix4x4 cameraToWorld; // By default, this gives an ID transform
                             // meaning that the camera space = world space
    OrtographicCamera camOrtho(cameraToWorld, film);

    /* ******************* */
    /* Perspective Camera */
    /* ******************* */
    double fovRadians = Utils::degreesToRadians(60);
    PerspectiveCamera camPersp(cameraToWorld, fovRadians, film);

    // Save the final result to file
    film.save();
}

int main()
{
    std::string separator = "\n----------------------------------------------\n";

    std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

    // ASSIGNMENT 1
    //transformationsExercise();
    //normalTransformExercise();
    //paintingAnImageExercise();
    filteringAnImageExercise();

    // ASSIGNMENT 2
    //eqSolverExercise();
    //completeSphereClassExercise();
    //raytrace();

    std::cout << "\n\n" << std::endl;
    return 0;
}
