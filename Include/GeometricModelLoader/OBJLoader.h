#include "GeometricModelLoader.h"

using namespace std;

class GeometricModel;

class OBJLoader : public GeometricModelLoader {
	public:
		OBJLoader();
		~OBJLoader();
		bool loadModel(string name,GeometricModel *model) override;

	private:
		static void setupForTextureCoordinates(GeometricModel* model);
		static void computeNormals(GeometricModel* model);
		static void computeTangents(GeometricModel* model);
};