//
//  ParametricSurface.cpp
//  OpenGL_ES2
//
//  Created by khkim on 8/8/12.
//
//

#include "ParametricSurface.hpp"

void ParametricSurface::setInterval(const ParametricInterval &interval)
{
	_upperBound = interval.upperBound;
	_divisions = interval.divisions;
	_slices = _divisions - ivec2(1, 1);
}

int ParametricSurface::getVertexCount() const
{
	return _divisions.x * _divisions.y;
}

int ParametricSurface::getLineIndexCount() const
{
	return 4 * _slices.x * _slices.y;
}

vec2 ParametricSurface::computeDomain(float i, float j) const
{
	return vec2(i * _upperBound.x / _slices.x, j * _upperBound.y / _slices.y);
}

void ParametricSurface::generateVertices(vector<float> &vertices) const
{
	vertices.resize(getVertexCount() * 3);

	vector<float>::iterator index = vertices.begin();
	for (int j = 0; j < _divisions.y; j++)
		for (int i = 0; i < _divisions.x; i++) {
			vec2 domain = computeDomain(i, j);
			vec3 range = evaluate(domain);
			
			*index++ = range.x;
			*index++ = range.y;
			*index++ = range.z;
		}
}

void ParametricSurface::generateLineIndices(vector<unsigned short> &indices) const
{
	indices.resize(getLineIndexCount());
	
	vector<unsigned short>::iterator index = indices.begin();
	for (int j = 0, vertex = 0; j < _slices.y; j++) {
		for (int i = 0; i < _slices.x; i++) {
			int next = (i + 1) % _divisions.x;
	
			*index++ = vertex + i;
			*index++ = vertex + next;
			*index++ = vertex + i;
			*index++ = vertex + i + _divisions.x;
		}
		
		vertex += _divisions.x;
	}
}