#include "ParametricSurface.hpp"

class Cone : public ParametricSurface
{
public:
    Cone(float height, float radius) : _height(height), _radius(radius)
    {
        ParametricInterval interval = { ivec2(20, 20), vec2(TwoPi, 1) };
        setInterval(interval);
    }
	
    vec3 evaluate(const vec2& domain) const
    {
        float u = domain.x, v = domain.y;
        float x = _radius * (1 - v) * cos(u);
        float y = _height * (v - 0.5f);
        float z = _radius * (1 - v) * -sin(u);
        return vec3(x, y, z);
    }
	
private:
    float _height;
    float _radius;
};

class Sphere : public ParametricSurface
{
public:
    Sphere(float radius) : _radius(radius)
    {
        ParametricInterval interval = { ivec2(20, 20), vec2(Pi, TwoPi) };
        setInterval(interval);
    }
	
    vec3 evaluate(const vec2& domain) const
    {
        float u = domain.x, v = domain.y;
        float x = _radius * sin(u) * cos(v);
        float y = _radius * cos(u);
        float z = _radius * -sin(u) * sin(v);
        return vec3(x, y, z);
    }
	
private:
    float _radius;
};

class Torus : public ParametricSurface
{
public:
    Torus(float majorRadius, float minorRadius) :
		_majorRadius(majorRadius),
		_minorRadius(minorRadius)
    {
        ParametricInterval interval = { ivec2(20, 20), vec2(TwoPi, TwoPi) };
        setInterval(interval);
    }
	
    vec3 evaluate(const vec2& domain) const
    {
        const float major = _majorRadius;
        const float minor = _minorRadius;
        float u = domain.x, v = domain.y;
        float x = (major + minor * cos(v)) * cos(u);
        float y = (major + minor * cos(v)) * sin(u);
        float z = minor * sin(v);
        return vec3(x, y, z);
    }
	
private:
    float _majorRadius;
    float _minorRadius;
};

class TrefoilKnot : public ParametricSurface
{
public:
    TrefoilKnot(float scale) : _scale(scale)
    {
        ParametricInterval interval = { ivec2(60, 15), vec2(TwoPi, TwoPi) };
        setInterval(interval);
    }
	
    vec3 evaluate(const vec2& domain) const
    {
        const float a = 0.5f;
        const float b = 0.3f;
        const float c = 0.5f;
        const float d = 0.1f;
        float u = (TwoPi - domain.x) * 2;
        float v = domain.y;
        
        float r = a + b * cos(1.5f * u);
        float x = r * cos(u);
        float y = r * sin(u);
        float z = c * sin(1.5f * u);
        
        vec3 dv;
        dv.x = -1.5f * b * sin(1.5f * u) * cos(u) -
		(a + b * cos(1.5f * u)) * sin(u);
        dv.y = -1.5f * b * sin(1.5f * u) * sin(u) +
		(a + b * cos(1.5f * u)) * cos(u);
        dv.z = 1.5f * c * cos(1.5f * u);
        
        vec3 q = dv.normalized();
        vec3 qvn = vec3(q.y, -q.x, 0).normalized();
        vec3 ww = q.cross(qvn);
        
        vec3 range;
        range.x = x + d * (qvn.x * cos(v) + ww.x * sin(v));
        range.y = y + d * (qvn.y * cos(v) + ww.y * sin(v));
        range.z = z + d * ww.z * sin(v);
        return range * _scale;
    }
private:
    float _scale;
};

class MobiusStrip : public ParametricSurface
{
public:
    MobiusStrip(float scale) : _scale(scale)
    {
        ParametricInterval interval = { ivec2(40, 20), vec2(TwoPi, TwoPi) };
        setInterval(interval);
    }
	
    vec3 evaluate(const vec2& domain) const
    {
        float u = domain.x;
        float t = domain.y;
        float major = 1.25;
        float a = 0.125f;
        float b = 0.5f;
        float phi = u / 2;
        
        // General equation for an ellipse where phi is the angle
        // between the major axis and the X axis.
        float x = a * cos(t) * cos(phi) - b * sin(t) * sin(phi);
        float y = a * cos(t) * sin(phi) + b * sin(t) * cos(phi);
		
        // Sweep the ellipse along a circle, like a torus.
        vec3 range;
        range.x = (major + x) * cos(u);
        range.y = (major + x) * sin(u);
        range.z = y;
        return range * _scale;
    }
	
private:
    float _scale;
};

class KleinBottle : public ParametricSurface
{
public:
    KleinBottle(float scale) : _scale(scale)
    {
        ParametricInterval interval = { ivec2(20, 20), vec2(TwoPi, TwoPi) };
        setInterval(interval);
    }
	
    vec3 evaluate(const vec2& domain) const
    {
        float v = 1 - domain.x;
        float u = domain.y;
        
        float x0 = 3 * cos(u) * (1 + sin(u)) +
		(2 * (1 - cos(u) / 2)) * cos(u) * cos(v);
        
        float y0  = 8 * sin(u) + (2 * (1 - cos(u) / 2)) * sin(u) * cos(v);
        
        float x1 = 3 * cos(u) * (1 + sin(u)) +
		(2 * (1 - cos(u) / 2)) * cos(v + Pi);
        
        float y1 = 8 * sin(u);
        
        vec3 range;
        range.x = u < Pi ? x0 : x1;
        range.y = u < Pi ? -y0 : -y1;
        range.z = (-2 * (1 - cos(u) / 2)) * sin(v);
        return range * _scale;
    }
	
    bool InvertNormal(const vec2& domain) const
    {
        return domain.y > 3 * Pi / 2;
    }
	
private:
    float _scale;
};

class Quad : public ParametricSurface
{
public:
    Quad(float width, float height) : _size(width, height)
    {
        ParametricInterval interval = { ivec2(2, 2), vec2(1, 1) };
        setInterval(interval);
    }
	
    vec3 evaluate(const vec2& domain) const
    {
        float x = (domain.x - 0.5) * _size.x;
        float y = (domain.y - 0.5) * _size.y;
        return vec3(x, -y, 0);
    }
	
private:
    vec2 _size;
};
