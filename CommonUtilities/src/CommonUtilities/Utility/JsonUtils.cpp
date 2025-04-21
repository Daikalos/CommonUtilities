#include <CommonUtilities/Utility/JsonUtils.hpp>

using namespace CommonUtilities;

void CommonUtilities::to_json(nlohmann::json& aJSON, const Mat4f& aMatrix)
{
	aJSON["Matrix"] = aMatrix.GetValues();
}
void CommonUtilities::from_json(const nlohmann::json& aJSON, Mat4f& aMatrix)
{
	cu::Mat4f nlohmann_json_default_obj;
	aMatrix = aJSON.value("Matrix", nlohmann_json_default_obj.GetValues());
}

void CommonUtilities::to_json(nlohmann::json& aJSON, const AABBf& aAABB)
{
	aJSON["Min"] = aAABB.GetMin();
	aJSON["Max"] = aAABB.GetMax();
}
void CommonUtilities::from_json(const nlohmann::json& aJSON, AABBf& aAABB)
{
	aAABB = AABBf::InitWithMinAndMax(
		aJSON.value("Min", cu::Vector3f::Zero),
		aJSON.value("Max", cu::Vector3f::Zero));
}

void CommonUtilities::to_json(nlohmann::json& aJSON, const Spheref& aSphere)
{
	aJSON["Center"] = aSphere.GetCenter();
	aJSON["Radius"] = aSphere.GetRadius();
}
void CommonUtilities::from_json(const nlohmann::json& aJSON, Spheref& aSphere)
{
	aSphere = Spheref::InitWithCenterAndRadius(
		aJSON.value("Center", cu::Vector3f::Zero),
		aJSON.value("Radius", 0.0f));
}

void CommonUtilities::to_json(nlohmann::json& aJSON, const Rayf& aRay)
{
	aJSON["Origin"]		= aRay.GetOrigin();
	aJSON["Direction"]	= aRay.GetDirection();
}
void CommonUtilities::from_json(const nlohmann::json& aJSON, Rayf& aRay)
{
	aRay = Rayf::InitWithOriginAndDirection(
		aJSON.value("Origin", cu::Vector3f::Zero), 
		aJSON.value("Direction", cu::Vector3f::Right));
}

void CommonUtilities::to_json(nlohmann::json& aJSON, const Capsulef& aCapsule)
{
	aJSON["Base"]	= aCapsule.GetBase();
	aJSON["Tip"]	= aCapsule.GetTip();
	aJSON["Radius"]	= aCapsule.GetRadius();
}
void CommonUtilities::from_json(const nlohmann::json& aJSON, Capsulef& aCapsule)
{
	aCapsule = Capsulef(
		aJSON.value("Base", cu::Vector3f::Zero),
		aJSON.value("Tip", cu::Vector3f::Right),
		aJSON.value("Radius", 0.0f));
}

void CommonUtilities::to_json(nlohmann::json& aJSON, const Planef& aPlane)
{
	aJSON["Origin"]	= aPlane.GetOrigin();
	aJSON["Normal"]	= aPlane.GetNormal();
}
void CommonUtilities::from_json(const nlohmann::json& aJSON, Planef& aPlane)
{
	aPlane = Planef::InitWithPointAndNormal(
		aJSON.value("Origin", cu::Vector3f::Zero),
		aJSON.value("Normal", cu::Vector3f::Up));
}

void CommonUtilities::to_json(nlohmann::json& aJSON, const Frustumf& aFrustum)
{
	aJSON["FrustumPoints"] = aFrustum.GetPoints();
}
void CommonUtilities::from_json(const nlohmann::json& aJSON, Frustumf& aFrustum)
{
	aFrustum = cu::Frustumf(aJSON.value("FrustumPoints", Frustumf::FrustumPoints{}));
}