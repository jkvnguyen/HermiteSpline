
#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include "shared/opengl.h"

class Hermite : public BaseSystem
{

public:
	Hermite(const std::string& name);
	virtual void getState(double *p);
	virtual void setState(double  *p);
	void reset(double time);

	void display(GLenum mode = GL_RENDER);

	void readModel(char *fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char **argv);
	
	void computeCR(); //Catmull-Rom
	void load(char *fname);
	void exportFile(char *fname);
	
	void add(float px, float py, float pz, float mx, float my, float mz);
	void setPoint(int index, float px, float py, float pz);
	void setTangent(int index, float px, float py, float pz);
	double getArcLength(float t);

protected:

	float m_sx;
	float m_sy;
	float m_sz;

	Vector m_pos;

	Vector points[40];
	Vector tangents[40];
	int numOfPoints;
	double arcLengths[400];

	Vector p;

	GLMmodel m_model;

};
