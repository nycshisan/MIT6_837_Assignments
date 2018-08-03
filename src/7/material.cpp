//
// Created by Nycshisan on 2018/7/25.
//

#include "material.h"

#include <cmath>

#include <OpenGL/gl.h>

#include "perlin_noise.h"

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

PhongMaterial::PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent,
                             const Vec3f &reflectiveColor, const Vec3f &transparentColor, float indexOfRefraction)
        : Material(diffuseColor) {
    _specularColor = specularColor;
    _reflectiveColor = reflectiveColor;
    _transparentColor = transparentColor;
    _exponent = exponent;
    _indexOfRefraction = indexOfRefraction;
}

void PhongMaterial::glSetMaterial() const {
    GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
    GLfloat specular[4] = {
            _specularColor.r(),
            _specularColor.g(),
            _specularColor.b(),
            1.0};
    GLfloat diffuse[4] = {
            _diffuseColor.r(),
            _diffuseColor.g(),
            _diffuseColor.b(),
            1.0};

    // NOTE: GL uses the Blinn Torrance version of Phong...
    float glExponent = _exponent;
    if (glExponent < 0) glExponent = 0;
    if (glExponent > 128) glExponent = 128;

#if !SPECULAR_FIX

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glExponent);

#else
    // OPTIONAL: 3 pass rendering to fix the specular highlight
    // artifact for small specular exponents (wide specular lobe)

    if (SPECULAR_FIX_WHICH_PASS == 0) {
        // First pass, draw only the specular highlights
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glExponent);
    } else if (SPECULAR_FIX_WHICH_PASS == 1) {
        // Second pass, compute normal dot light
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    } else {
        // Third pass, add ambient & diffuse terms
        assert (SPECULAR_FIX_WHICH_PASS == 2);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    }
#endif
}

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    Vec3f v = ray.getDirection();
    v.Negate();
    Vec3f h = v + dirToLight;
    h.Normalize();

    auto diffuse = std::max(0.f, dirToLight.Dot3(hit.getNormal())) * lightColor * _diffuseColor;
    auto specular = _specularColor * lightColor * powf(hit.getNormal().Dot3(h), _exponent); // ignore r^2 coefficient
    return diffuse + specular;
}

Vec3f ProceduralMaterial::_getTextureSpaceCoord(const Vec3f &wsCoord) const {
    auto tsCoord = wsCoord;
    _mat->Transform(tsCoord);
    return tsCoord;
}

Checkerboard::Checkerboard(Matrix *m, Material *mat1, Material *mat2) : ProceduralMaterial(m) {
    _m[0] = mat1; _m[1] = mat2;
}

Material* Checkerboard::_getMaterial(const Vec3f &wsCoord) const {
    auto tsp = _getTextureSpaceCoord(wsCoord);
    auto index = int(std::fabsf(std::floorf(tsp.x()) + std::floorf(tsp.y()) + std::floorf(tsp.z()))) % 2;
    return _m[index];
}

Vec3f Checkerboard::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    const auto &intersection = hit.getIntersectionPoint();
    return _getMaterial(intersection)->Shade(ray, hit, dirToLight, lightColor);
}

Noise::Noise(Matrix *m, Material *mat1, Material *mat2, int octaves) : ProceduralMaterial(m) {
    _m1 = mat1; _m2 = mat2;
    _octaves = octaves;
    _rangeLength = 1.f;
    _rangeOffset = 0.5f;
}

float Noise::_noise(const Vec3f &tsCoord) const {
    float noise = 0.0f;
    auto noiseP = tsCoord;
    float noiseScale = 1.f;
    for (int i = 0 ; i < _octaves; ++i) {
        float x, y, z;
        noiseP.Get(x, y, z);
        noise += PerlinNoise::noise(x, y, z) / noiseScale;
        noiseP *= 2.f;
        noiseScale *= 2.f;
    }
    return noise;
}

float Noise::_clampedNoise(const Vec3f &tsCoord) const {
    float n = (_rangeOffset - _noise(tsCoord)) / _rangeLength;
    return std::min(1.f, std::max(0.f, n));
}

Vec3f Noise::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    auto color1 = _m1->Shade(ray, hit, dirToLight, lightColor);
    auto color2 = _m2->Shade(ray, hit, dirToLight, lightColor);
    float noise = _clampedNoise(_getTextureSpaceCoord(hit.getIntersectionPoint()));
    return _lerp(color1, color2, noise);
}

Marble::Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) : Noise(m, mat1, mat2, octaves) {
    _freq = frequency;
    _ampl = amplitude;
    _rangeLength = 2.f;
    _rangeOffset = 1.f;
}

float Marble::_noise(const Vec3f &tsCoord) const {
    return std::sinf(_freq * tsCoord.x() + _ampl * Noise::_noise(tsCoord));
}

Wood::Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) : Noise(m, mat1, mat2, octaves) {
    _freq = frequency;
    _ampl = amplitude;
    _rangeLength = 2.f;
    _rangeOffset = 1.f;
}

float Wood::_noise(const Vec3f &tsCoord) const {
    float l = tsCoord.x() * tsCoord.x() + tsCoord.z() * tsCoord.z();
    float s = sinf(_freq * sqrtf(l) + _ampl * Noise::_noise(tsCoord) + tsCoord.y() * 0.4f);
    float sign = s < 0 ? -1.f : 1.f;
    s = fabsf(s);
    return sign * (1 - std::powf(1 - s, _freq));
}
