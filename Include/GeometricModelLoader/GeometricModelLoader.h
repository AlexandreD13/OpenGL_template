#ifndef GEOEMTRICAL_Node_LOADER_
#define GEOEMTRICAL_Node_LOADER_

#include <string>
using namespace std;
class GeometricModel;

class GeometricModelLoader{
	public:
		GeometricModelLoader()= default;
		~GeometricModelLoader()= default;
		virtual bool loadModel(string name,GeometricModel *model) = 0;
};

#endif