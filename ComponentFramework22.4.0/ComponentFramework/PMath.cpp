#include "PMath.h"
#include "VMath.h"
using namespace MATH;

Plane PMath::normalize(const Plane &p) {
	float mag = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	return Plane(p.x / mag, p.y / mag, p.z / mag, 
		p.d / mag);
}

float PMath::distance(const Vec3 &v, const Plane &p) { //https://mathinsight.org/distance_point_plane#:~:text=The%20shortest%20distance%20from%20a,as%20a%20gray%20line%20segment
	Vec3 n = p;
	float mag = VMath::mag(n);
	return (p.x*v.x + p.y*v.y + p.z*v.z + p.d) / mag;
}

/// Reflect an  incident vector off the normal of a plane 
Vec3 PMath::reflect(const Vec3 &v, const Plane &p) {
	float lamda = 2.0f * VMath::dot(p, v);
	return v + lamda * p;
}
