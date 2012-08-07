//
//  ApplicationEngine.cpp
//  OpenGL_ES2
//
//  Created by khkim on 8/8/12.
//
//

#include "Interfaces.hpp"
#include "ParametricEquations.hpp"

using namespace std;

class ApplicationEngine : public IApplicationEngine
{
public:
	ApplicationEngine(IRenderingEngine *renderingEngine);
	~ApplicationEngine();
	
	void initialize(int width, int height);
	void render() const;
	void updateAnimation(float timeStep);

	void onMouseUp(ivec2 location);
	void onMouseDown(ivec2 location);
	void onMouseMove(ivec2 oldLocation, ivec2 newLocation);
	
private:
	vec3 mapToSphere(ivec2 touchPoint) const;
	
	IRenderingEngine *_renderEngine;

	ivec2 _screenSize;
	ivec2 _centerPoint;
	float _trackballlRadius;
	
	ivec2 _mouseStart;
	bool _spinning;
	
	Quaternion _orientation;
	Quaternion _previousOrientation;
};

IApplicationEngine *IApplicationEngine::createApplicationEngine(IRenderingEngine *renderingEngine)
{
	return new ApplicationEngine(renderingEngine);
}

ApplicationEngine::ApplicationEngine(IRenderingEngine *renderingEngine) :
	_spinning(false),
	_renderEngine(renderingEngine)
{
}

ApplicationEngine::~ApplicationEngine()
{
	if (_renderEngine) { delete _renderEngine; _renderEngine = NULL; }
}

void ApplicationEngine::initialize(int width, int height)
{
	_trackballlRadius = width / 3;
	_screenSize = ivec2(width, height);
	_centerPoint = _screenSize / 2;
	
	vector<ISurface *> surfaces;
	surfaces.push_back(new Cone(3, 1));
	surfaces.push_back(new Sphere(1.4f));
	surfaces.push_back(new Torus(1.4f, 0.3f));
	surfaces.push_back(new TrefoilKnot(1.8f));
	surfaces.push_back(new KleinBottle(0.2f));
	surfaces.push_back(new MobiusStrip(1));
	_renderEngine->initialize(surfaces);
	
	for (int i = 0; i < surfaces.size(); i++)
		delete surfaces[i];
}

void ApplicationEngine::render() const
{
	Visual visual;
	visual.color = _spinning ? vec3(1, 1, 1) : vec3(0, 1, 1);
	visual.lowerLeft = ivec2(0, 48);
	visual.viewportSize = ivec2(320, 432);
	visual.orientation = _orientation;

	vector<Visual> visuals;
	visuals.push_back(visual);
	
	_renderEngine->render(visuals);
}

void ApplicationEngine::updateAnimation(float timeStep)
{
}

void ApplicationEngine::onMouseDown(ivec2 location)
{
	_mouseStart = location;
	_previousOrientation = _orientation;
	
	_spinning = true;
}

void ApplicationEngine::onMouseMove(ivec2 oldLocation, ivec2 newLocation)
{
	if (_spinning) {
		vec3 start = mapToSphere(_mouseStart);
		vec3 end = mapToSphere(newLocation);
		
		Quaternion delta = Quaternion::createFromVectors(start, end);
		_orientation = delta.rotated(_previousOrientation);
	}
}

void ApplicationEngine::onMouseUp(ivec2 location)
{
	_spinning = false;
}

vec3 ApplicationEngine::mapToSphere(ivec2 touchPoint) const
{
	vec2 p = touchPoint - _centerPoint;
	
	p.y = -p.y;
	
	const float radius = _trackballlRadius;
	const float safeRadius = radius - 1;
	
	if (p.length() > safeRadius) {
		float theta = atan2(p.y, p.x);
		
		p.x = safeRadius * cos(theta);
		p.y = safeRadius * sin(theta);
	}
	
	float z = sqrt(radius * radius - p.lengthSquared());
	
	vec3 mapped = vec3(p.x, p.y, z);
	return mapped / radius;
}