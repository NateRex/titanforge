#include <geometry/Polyface.h>
#include <geometry/Vector3.h>
#include <common/exceptions/OutOfBoundsException.h>

// ------------------------------------------------------------------------------------------------------------------
// Polyface::Iterator
// ------------------------------------------------------------------------------------------------------------------

Polyface::Iterator::Iterator(const std::vector<Vector3>& vertices, const std::vector<int>& indices,
	const std::vector<int>::const_iterator& itr) : _vertices(&vertices), _indices(&indices), _itr(itr)
{
	if (_itr != indices.end() && *_itr == -1)
	{
		// If vertex list started with a -1 (new facet marker), ensure we move to the actual start of the first facet
		toNextFacet();
	}
}

Polyface::Iterator& Polyface::Iterator::operator++()
{
	toNextFacet();
	return *this;
}

bool Polyface::Iterator::operator==(const Iterator& other) const
{
	return _itr == other._itr;
}

bool Polyface::Iterator::operator!=(const Iterator& other) const
{
	return _itr != other._itr;
}

std::vector<Vector3> Polyface::Iterator::operator*() const
{
	if (_itr == _indices->end())
	{
		throw OutOfBoundsException("Polyface has no next facet");
	}

	std::vector<Vector3> r;
	std::vector<int>::const_iterator itr = _itr;
	while (itr != _indices->end() && *itr != -1)
	{
		r.push_back(_vertices->at(*itr));
		itr++;
	}

	return r;
}

void Polyface::Iterator::toNextFacet()
{
	// Move iterator to the next -1 value, indicating the end of the facet
	while (_itr != _indices->end() && *_itr != -1)
	{
		_itr++;
	}

	// Move iterator past the end marker(s) to the actual first value of the facet
	while (_itr != _indices->end() && *_itr == -1)
	{
		_itr++;
	}
}

// ------------------------------------------------------------------------------------------------------------------
// Polyface
// ------------------------------------------------------------------------------------------------------------------

Polyface::Polyface(const Vector3* vertices, int numVerts, const int* indices, int numIndices)
	: _vertices(vertices, vertices + numVerts), _indices(indices, indices + numIndices)
{

}

Polyface::Polyface(const std::vector<Vector3>& vertices, const std::vector<int> indices)
	: _vertices(vertices), _indices(indices)
{

}

Polyface::Polyface(const Polyface& polyface)
	: _vertices(polyface._vertices), _indices(polyface._indices)
{

}

int Polyface::getNumVertices() const
{
	int count = 0;
	for (auto& v : _indices)
	{
		if (v != -1)
		{
			count++;
		}
	}
	return count;
}

int Polyface::getNumFacets() const
{
	int count = 0;
	for (Polyface::Iterator itr = begin(); itr != end(); ++itr)
	{
		count++;
	}
	return count;
}

Polyface::Iterator Polyface::begin() const
{
	return Iterator(_vertices, _indices, _indices.begin());
}

Polyface::Iterator Polyface::end() const
{
	return Iterator(_vertices, _indices, _indices.end());
}