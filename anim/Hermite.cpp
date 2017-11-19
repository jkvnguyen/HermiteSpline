#include "Hermite.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

Hermite::Hermite(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f)
{
	setVector(m_pos, 0, 0, 0);
	numOfPoints = 0;

}	// Hermite Class

void Hermite::getState(double* p)
{

	VecCopy(p, m_pos);

}	// Hermite::getState

void Hermite::setState(double  *p)
{

	VecCopy(m_pos, p);

}	// Hermite::setState

void Hermite::reset(double time)
{

	setVector(m_pos, 0, 0, 0);
	numOfPoints = 0;

}	// Hermite::Reset


int Hermite::command(int argc, myCONST_SPEC char **argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name);
		return TCL_ERROR;
	}
	//load
	else if (strcmp(argv[0], "load") == 0)
	{
		if (argc == 2)
		{
			Hermite::load(argv[1]);
			animTcl::OutputMessage("Successfully loaded file.");
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: load <file_name>");
			return TCL_ERROR;
		}
		//export
	}
	else if (strcmp(argv[0], "export") == 0)
	{
		if (argc == 2)
		{
			Hermite::exportFile(argv[1]);
			animTcl::OutputMessage("Successfully exported file.");
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: export <file_name>");
			return TCL_ERROR;
		}
	} else if (strcmp(argv[0], "scale") == 0)
	{
		if (argc == 4)
		{
			m_sx = (float)atof(argv[1]);
			m_sy = (float)atof(argv[2]);
			m_sz = (float)atof(argv[3]);
		}
		else
		{
			animTcl::OutputMessage("Usage: scale <sx> <sy> <sz> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "pos") == 0)
	{
		if (argc == 4)
		{
			m_pos[0] = atof(argv[1]);
			m_pos[1] = atof(argv[2]);
			m_pos[2] = atof(argv[3]);
		}
		else
		{
			animTcl::OutputMessage("Usage: pos <x> <y> <z> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "flipNormals") == 0)
	{
		flipNormals();
		return TCL_OK;

	}
	else if (strcmp(argv[0], "reset") == 0)
	{
		double p[3] = { 0,0,0 };
		setState(p);
	}
	else if (strcmp(argv[0], "test") == 0)
	{
		//Hermite::getArcLength(1);
		/*
		float t = 1;

		Vector a; VecNumMul(a, points[0], 2 * t*t*t - 3 * t*t + 1);
		Vector b; VecNumMul(b, tangents[0], t*t*t - 2 * t*t + t);
		Vector c; VecNumMul(c, points[1], -2 * t*t*t + 3 * t*t);
		Vector d; VecNumMul(d, tangents[1], t*t*t - t*t);

		Vector h; VecAdd(h, a, b);
		VecAdd(h, h, c);
		VecAdd(h, h, d);

		Vector w; VecSubtract(w, points[0], h);

		for (int i = 0; i < 20; i++) {
			animTcl::OutputResult("arcLengths: %f\n", Hermite::getArcLength(i));
		}
		Vector k; VecSubtract(k, points[0], points[1]);
		*/

		//animTcl::OutputResult("length .1 is: %f", VecLength(w));
		//animTcl::OutputResult("length 1 is: %f", VecLength(k));
		
		//}
		
		//add
	} else if ((strcmp(argv[0], "add") == 0) && (strcmp(argv[1], "point") == 0)) {
		if (argc == 8) {
			Hermite::add(
				(float)atof(argv[2]),
				(float)atof(argv[3]),
				(float)atof(argv[4]),
				(float)atof(argv[5]),
				(float)atof(argv[6]),
				(float)atof(argv[7]));
			animTcl::OutputResult("Successfully added point.");
			return	TCL_OK;
		} else {
			animTcl::OutputResult("Invalid <add> point and tangent input.");
			return TCL_ERROR;
		}
		//setPoint
	}	else if ((strcmp(argv[0], "set") == 0) && (strcmp(argv[1], "point") == 0)){
		if (argc == 6) {
			Hermite::setPoint(
				(float)atof(argv[2]),
				(float)atof(argv[3]),
				(float)atof(argv[4]),
				(float)atof(argv[5]));
			animTcl::OutputResult("Successfully set point at index %.0f", (float)atof(argv[2]));
			return	TCL_OK;
		} else {
			animTcl::OutputResult("Invalid <set> point input.");
			return TCL_ERROR;
		}
		//setTangent
	}
	else if ((strcmp(argv[0], "set") == 0) && (strcmp(argv[1], "tangent") == 0)) {
		if (argc == 6) {
			Hermite::setTangent(
				(float)atof(argv[2]),
				(float)atof(argv[3]),
				(float)atof(argv[4]),
				(float)atof(argv[5]));
			animTcl::OutputResult("Successfully set tangent at index %.0f", (float)atof(argv[2]));
			return	TCL_OK;
		} else {
			animTcl::OutputResult("Invalid <set> tangent input.");
			return TCL_ERROR;
		}
	}
	else if ((strcmp(argv[0], "getArcLength") == 0)) {
		if (argc == 2) {
			double temp = Hermite::getArcLength((float)atof(argv[1]));
			animTcl::OutputResult("Arc length of parameter value, %.2f, is %f", (float)atof(argv[1]),
				temp);
			return	TCL_OK;
		}
		else {
			animTcl::OutputResult("Invalid <get> arc length input.");
			return TCL_ERROR;
		}
	}
	
	return	TCL_OK;

}	// SampleParticle::command

void Hermite::display(GLenum mode)
{
	glDisable(GL_LIGHTING);

	Vector p0;
	Vector p1;
	Vector m0;
	Vector m1;

	//when adding 1 point
	if (numOfPoints == 1) {

		glPointSize(4.0);
		glColor3f(0, 1, 0);
		glBegin(GL_POINTS);

		glVertex3f(points[numOfPoints - 1][0], points[numOfPoints - 1][1], points[numOfPoints - 1][2]);

		glEnd();

	} 

		//numOfPoints-1 because i+1 = numOfPoints -> 0-17 (18) -> i+1 -> 17+1 = 18 (19)
		for (int i = 0; i < numOfPoints - 1; i++) {		

			glPointSize(4.0);
			glColor3f(0, 1, 0);
			glBegin(GL_POINTS);

			setVector(p0, points[i][0], points[i][1], points[i][2]);
			setVector(p1, points[i + 1][0], points[i + 1][1], points[i + 1][2]);
			setVector(m0, tangents[i][0], tangents[i][1], tangents[i][2]);
			setVector(m1, tangents[i + 1][0], tangents[i + 1][1], tangents[i + 1][2]);

			glVertex3f(points[i][0], points[i][1], points[i][2]);
			//this kind of overlaps but it draws the last point
			glVertex3f(points[i+1][0], points[i+1][1], points[i+1][2]);

			glEnd(); //end Points

			//spline drawing
			glColor3f(1, 0.5, 0);
			glPointSize(2.0);

			glBegin(GL_LINE_STRIP);

			for (double t = 0.0; t <= 1.0; t = t + 0.1) {

				Vector a; VecNumMul(a, p0, 2 * t*t*t - 3 * t*t + 1);
				Vector b; VecNumMul(b, m0, t*t*t - 2 * t*t + t);
				Vector c; VecNumMul(c, p1, -2 * t*t*t + 3 * t*t);
				Vector d; VecNumMul(d, m1, t*t*t - t*t);

				Vector h; VecAdd(h, a, b);
				VecAdd(h, h, c);
				VecAdd(h, h, d);

				glVertex3dv(h);

			}
			glEnd();
			//spline drawing
	}
	
}	// Hermite::display

//Catmull-Rom initialization
void Hermite::computeCR() {

	for (int i = 0; i < numOfPoints; i++) {

		//general form
		if (i >= 1 && i <= numOfPoints-2) {
			VecSubtract(tangents[i], points[i+1], points[i-1]);
			VecNumMul(tangents[i], tangents[i], 0.5);
		}
	}

}

void Hermite::load(char *fname) {

	ifstream myFile(fname);

	char name[256];
	numOfPoints;
	float px, py, pz, mx, my, mz;

	if (myFile.is_open())
	{
		//scan the first line of file
		myFile >> name >> numOfPoints;

		for (int i = 0; i < numOfPoints; i++) {

			myFile >> px >> py >> pz >> mx >> my >> mz;

			setVector(points[i], px, py, pz);
			setVector(tangents[i], mx, my, mz);

		}
		myFile.close();

	} else {
		animTcl::OutputMessage("Unable to open file.");
	}

}

//method 'export' does not work (probably a reserved word)
void Hermite::exportFile(char *fname) {

	ofstream myFile(fname);

	if (myFile.is_open()) {
		myFile << "hermite " << numOfPoints << endl;

		for (int i = 0; i < numOfPoints; i++) {
			myFile << points[i][0] << " "
				<< points[i][1] << " "
				<< points[i][2] << " "
				<< tangents[i][0] << " "
				<< tangents[i][1] << " "
				<< tangents[i][2] << endl;
		}
	} else {
		animTcl::OutputMessage("Unable to export file.");
	}

}

void Hermite::add(float px, float py, float pz, float mx, float my, float mz) {

	if (numOfPoints < 40) {
		setVector(points[numOfPoints], px, py, pz);
		setVector(tangents[numOfPoints], mx, my, mz);
		numOfPoints++;
	}
	else {
		animTcl::OutputMessage("Control points are full!");
	}

}

void Hermite::setPoint(int index, float px, float py, float pz) {
	
	if (index >= numOfPoints || index < 0) {
		animTcl::OutputMessage("Index out of bounds.");
	} else {
		setVector(points[index], px, py, pz);
	}

}

void Hermite::setTangent(int index, float px, float py, float pz) {
	
	if (index >= numOfPoints || index < 0) {
		animTcl::OutputMessage("Index out of bounds.");
	} else {
		setVector(tangents[index], px, py, pz);
	}
	
}

//the getArcLength method is behind one update value of arc length, so click another point
//and getArcLength to get the previous value of arc length
double Hermite::getArcLength(float t) {

	arcLengths[0] = 0;
	double length = 0;
	int count = 1;

	//for loops are a bit redudent but arc lengths do not properly show when loading a script and not drawn (using display)
	for (int i = 0; i < numOfPoints - 1; i++) {

		//starts the loop at the first point
		Vector p1; setVector(p1, points[i][0], points[i][1], points[i][2]);

		for (double t = 0.0; t <= 1.0; t = t + 0.1) {

			//if count goes out of index of arcLengths
			if (count > 400) {
				return TCL_ERROR;
			}

			Vector a; VecNumMul(a, points[i], 2 * t*t*t - 3 * t*t + 1);
			Vector b; VecNumMul(b, tangents[i], t*t*t - 2 * t*t + t);
			Vector c; VecNumMul(c, points[i+1], -2 * t*t*t + 3 * t*t);
			Vector d; VecNumMul(d, tangents[i+1], t*t*t - t*t);

			Vector h; VecAdd(h, a, b);
			VecAdd(h, h, c);
			VecAdd(h, h, d);

			//Vector p2; VecAdd(p2, p1, h);
			Vector l; VecSubtract(l, p1, h);

			//sets p1 to p2
			setVector(p1, h[0], h[1], h[2]);
			
			length += VecLength(l);
			arcLengths[count++] = length;
		}
	}

	int arcLengthPoints = (numOfPoints-1) * 10; //10 is the number of times interpolated between points
	int index = (int) (arcLengthPoints * t);

	return arcLengths[index];

}