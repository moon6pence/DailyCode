//
//  ParametricSurface.hpp
//  OpenGL_ES2
//
//  Created by khkim on 8/8/12.
//
//

#pragma once

#include "Interfaces.hpp"

struct ParametricInterval
{
	ivec2 divisions;
	vec2 upperBound;
};

class ParametricSurface : public ISurface
{
public:
	int getVertexCount() const;
	int getLineIndexCount() const;
	void generateVertices(vector<float>& vertices) const;
	void generateLineIndices(vector<unsigned short>& indices) const;
	
protected:
	void setInterval(const ParametricInterval& interval);
	virtual vec3 evaluate(const vec2& domain) const = 0;
	
private:
	vec2 computeDomain(float i, float j) const;
	
	vec2 _upperBound;
	ivec2 _slices;
	ivec2 _divisions;
};