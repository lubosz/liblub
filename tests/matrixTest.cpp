#include "Matrix.h"
#include "Camera.h"
#include <iostream>
#include <QMatrix4x4>
#include <QMatrix3x3>
#include <QVector3D>

using namespace std;

Matrix myMatrix;
QMatrix4x4 myQMatrix;

void printMatrix(Matrix matrix){
	for (int i = 0; i < 16; i++){
		if (i%4 == 0 && i != 0) cout << "\n";
		cout << matrix.at(i) << " ";

	}
	cout << "\n";
}

void printMatrix(QMatrix4x4 matrix){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			cout << matrix(i,j) << " ";
		}
		cout << "\n";
	}
}

void printNormalMatrix(vector<GLfloat> matrix){
	for (int i = 0; i < 9; i++){
		if (i%3 == 0 && i != 0) cout << "\n";
		cout << matrix.at(i) << " ";
	}
	cout << "\n";
}

void printNormalMatrix(QMatrix3x3 matrix){
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			cout << matrix(i,j) << " ";
		}
		cout << "\n";
	}
}

void print(){
	printMatrix(myMatrix);
	cout << "\n";
	printMatrix(myQMatrix);
	cout << "\n";
}

int main(int argc, char *argv[])
{
	//Construct
	myMatrix = Matrix();
	myMatrix.identity();
	myQMatrix = QMatrix4x4();

	print();

	//Translation test
	myMatrix.translate({1,2,3});
	myQMatrix.translate(QVector3D(1,2,3));
	myQMatrix = myQMatrix.transposed();
	//myQMatrix;

	print();

	//Scale
	myMatrix.scale(2);
	myQMatrix.scale(2);

	print();

	//Rotation test
	myMatrix.rotate(10, X_AXIS);
	myQMatrix.rotate(10, QVector3D(1,0,0));

	print();

	//Normal Matrix test
	printNormalMatrix(myMatrix.getNormalMatrix());
	cout << "\n";
	printNormalMatrix(myQMatrix.normalMatrix());
	cout << "\n";

	/*
	//Reset
	myMatrix.identity();
	myQMatrix.setToIdentity();
	print();
*/
	//Projection Matrix
	QMatrix4x4 projectionMatrix = QMatrix4x4();
	float aspect = 1920.0f/1200.0f;
	Camera::Instance().setParams(70.0, 0.1, 100.0);
	Camera::Instance().setAspect(aspect);
/*
	projectionMatrix.perspective ( 70.0, aspect, 0.1, 100.0 );
	qreal blubb = projectionMatrix(2,3);
	projectionMatrix(2,3) = projectionMatrix(3,2);
	projectionMatrix(3,2) = blubb;
*/
	myMatrix.multiply(Camera::Instance().getProjectionmatrix2());

	myQMatrix *= Camera::Instance().getProjectionmatrix();
	//myQMatrix *= projectionMatrix;

	//myQMatrix.ortho ( -1, 1, -1, 1, .1, 100.0 );
	//myQMatrix.lookAt({0,0,2},{0,0,0},{0,1,0});

	print();

}
