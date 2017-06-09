#include "buildable.h"

Buildable::Buildable(BuildType buildType, int productionCost) : buildType(buildType), productionCost(productionCost) {

}

Buildable::BuildType Buildable::getBuildType() {
	return buildType;
}