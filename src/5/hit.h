#ifndef _HIT_H
#define _HIT_H

#include <limits>

#include "vectors.h"
#include "ray.h"

class Material;

// ====================================================================
// ====================================================================

class Hit {
  
public:

  // CONSTRUCTOR & DESTRUCTOR
  Hit() { material = NULL; t = std::numeric_limits<float>::max(); }
  Hit(const Hit &h) { 
    t = h.t; 
    material = h.material; 
    normal = h.normal; 
    intersectionPoint = h.intersectionPoint;
    objectType = h.objectType;
  }
  ~Hit() {}

  enum ObjectType {
      Sphere, Plane, Triangle, Grid
  };

  // ACCESSORS
  float getT() const { return t; }
  Material* getMaterial() const { return material; }
  Vec3f getNormal() const { return normal; }
  Vec3f getIntersectionPoint() const { return intersectionPoint; }
  ObjectType getObjectType() const { return objectType; }
  
  // MODIFIER
  void set(float _t, Material *m, Vec3f n, const Ray &ray, ObjectType objType) {
    t = _t; material = m; normal = n; 
    intersectionPoint = ray.pointAtParameter(t);
    objectType = objType;
  }

private: 

  // REPRESENTATION
  float t;
  Material *material;
  Vec3f normal;
  Vec3f intersectionPoint;
  ObjectType objectType;

};

inline ostream &operator<<(ostream &os, const Hit &h) {
  os << "Hit <" <<h.getT()<<", "<<h.getNormal()<<">";
  return os;
}
// ====================================================================
// ====================================================================

#endif
