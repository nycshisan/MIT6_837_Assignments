#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "ray.h"
#include "hit.h"
#include "vectors.h"

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material() = default;
  explicit Material(const Vec3f &d_color) { _diffuseColor = d_color; }
  virtual ~Material() = default;

  // ACCESSORS
  virtual Vec3f getDiffuseColor(const Vec3f &wsCoord) const { return _diffuseColor; }
  virtual Vec3f getReflectiveColor(const Vec3f &wsCoord) const = 0;
  virtual Vec3f getTransparentColor(const Vec3f &wsCoord) const = 0;
  virtual float getIndexOfRefraction(const Vec3f &wsCoord) const = 0;

  virtual void glSetMaterial() const = 0;
  virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const = 0;

protected:

  // REPRESENTATION
  Vec3f _diffuseColor;
  
};

// ====================================================================
// ====================================================================

class PhongMaterial : public Material {
    Vec3f _specularColor, _reflectiveColor, _transparentColor;
    float _exponent = 0.f, _indexOfRefraction = 1.f;

    friend class Grid;
    PhongMaterial() = default;

public:
    PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent,
                                 const Vec3f &reflectiveColor, const Vec3f &transparentColor, float indexOfRefraction);

    Vec3f getReflectiveColor(const Vec3f &wsCoord) const override { return _reflectiveColor; }
    Vec3f getTransparentColor(const Vec3f &wsCoord) const override { return _transparentColor; }
    float getIndexOfRefraction(const Vec3f &wsCoord) const override { return _indexOfRefraction; }

    void glSetMaterial() const override;
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;
};


class ProceduralMaterial : public Material {
    std::shared_ptr<Matrix> _mat;

protected:
    explicit ProceduralMaterial(Matrix *m) : _mat(m) {};

    Vec3f _getTextureSpaceCoord(const Vec3f &wsCoord) const;
};


class Checkerboard : public ProceduralMaterial {
    Material *_m[2];

    Material* _getMaterial(const Vec3f &wsCoord) const;

public:
    Checkerboard(Matrix *m, Material *mat1, Material *mat2);

    Vec3f getDiffuseColor(const Vec3f &wsCoord) const override { return _getMaterial(wsCoord)->getDiffuseColor(wsCoord); }
    Vec3f getReflectiveColor(const Vec3f &wsCoord) const override { return _getMaterial(wsCoord)->getReflectiveColor(wsCoord); }
    Vec3f getTransparentColor(const Vec3f &wsCoord) const override { return _getMaterial(wsCoord)->getTransparentColor(wsCoord); }
    float getIndexOfRefraction(const Vec3f &wsCoord) const override { return _getMaterial(wsCoord)->getIndexOfRefraction(wsCoord); }

    void glSetMaterial() const override { _m[0]->glSetMaterial(); }
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;
};


class Noise : public ProceduralMaterial {
protected:
    Material *_m1, *_m2;
    int _octaves;

    float _rangeLength = 0.f, _rangeOffset = 0.f;

protected:
    virtual float _noise(const Vec3f &tsCoord) const;
    float _clampedNoise(const Vec3f &tsCoord) const;
    template <class T>
    T _lerp(const T &a, const T &b, float t) const { return a + t * (b - a); }

public:
    Noise(Matrix *m, Material *mat1, Material *mat2, int octaves);


    Vec3f getDiffuseColor(const Vec3f &wsCoord) const override {
      auto tsCoord = _getTextureSpaceCoord(wsCoord);
      return _lerp(_m1->getDiffuseColor(wsCoord), _m2->getDiffuseColor(wsCoord), _clampedNoise(tsCoord));
    }
    Vec3f getReflectiveColor(const Vec3f &wsCoord) const override {
      auto tsCoord = _getTextureSpaceCoord(wsCoord);
      return _lerp(_m1->getReflectiveColor(wsCoord), _m2->getReflectiveColor(wsCoord), _clampedNoise(tsCoord));
    }
    Vec3f getTransparentColor(const Vec3f &wsCoord) const override {
      auto tsCoord = _getTextureSpaceCoord(wsCoord);
      return _lerp(_m1->getTransparentColor(wsCoord), _m2->getTransparentColor(wsCoord), _clampedNoise(tsCoord));
    }
    float getIndexOfRefraction(const Vec3f &wsCoord) const override {
      auto tsCoord = _getTextureSpaceCoord(wsCoord);
      return _lerp(_m1->getIndexOfRefraction(wsCoord), _m2->getIndexOfRefraction(wsCoord), _clampedNoise(tsCoord));
    }

    void glSetMaterial() const override { _m1->glSetMaterial(); }
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;
};

class Marble : public Noise {
    float _freq, _ampl;

    float _noise(const Vec3f &tsCoord) const override;

public:
    Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude);
};

class Wood : public Noise {
    float _freq, _ampl;

    float _noise(const Vec3f &tsCoord) const override;

public:
    Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude);
};

#endif
