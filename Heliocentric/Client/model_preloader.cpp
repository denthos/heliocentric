#include "model_preloader.h"
#include "model.h"

#define ROCKET_MODEL "Models/Federation Interceptor HN48/Federation Interceptor HN48 flying.obj"
#define BEAR_MODEL "Models/bear.obj"

void ModelPreloader::preload() {
	Model::getInstance(ROCKET_MODEL);
	Model::getInstance(BEAR_MODEL);
}