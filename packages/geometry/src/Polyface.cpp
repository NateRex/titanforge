#include <geometry/Polyface.h>
#include <common/exceptions/OutOfBoundsException.h>

// ------------------------------------------------------------------------------------------------------------------
// Polyface::Iterator
// ------------------------------------------------------------------------------------------------------------------

Polyface::Iterator::Iterator(const std::vector<Vector3>& positions, const std::vector<unsigned int>& verts,
	const std::vector<unsigned int>::const_iterator& itr) : _pos(&positions), _verts(&verts), _itr(itr)
{
	if (*_itr == -1)
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
	if (_itr == _verts->end())
	{
		throw OutOfBoundsException("Polyface has no next facet");
	}

	std::vector<Vector3> r;
	std::vector<unsigned int>::const_iterator itr = _itr;
	while (*itr != -1 && itr != _verts->end())
	{
		r.push_back(_pos->at(*itr));
	}

	return r;
}

void Polyface::Iterator::toNextFacet()
{
	// Move iterator to the next -1 value, indicating the end of the facet
	while (*_itr != -1 && _itr != _verts->end())
	{
		_itr++;
	}

	// Move iterator past the end marker(s) to the actual first value of the facet
	while (*_itr == -1 && _itr != _verts->end())
	{
		_itr++;
	}
}

// ------------------------------------------------------------------------------------------------------------------
// Polyface
// ------------------------------------------------------------------------------------------------------------------

Polyface::Polyface(const std::vector<Vector3>& positions, const std::vector<unsigned int> vertices)
	: _positions(positions), _vertices(vertices)
{

}

unsigned int Polyface::getNumVertices() const
{
	unsigned int count = 0;
	for (auto& v : _vertices)
	{
		if (v != -1)
		{
			count++;
		}
	}
	return count;
}

unsigned int Polyface::getNumFacets() const
{
	unsigned int count = 0;
	for (Polyface::Iterator itr = begin(); itr != end(); ++itr)
	{
		count++;
	}
	return count;
}

Polyface::Iterator Polyface::begin() const
{
	return Iterator(_positions, _vertices, _vertices.begin());
}

Polyface::Iterator Polyface::end() const
{
	return Iterator(_positions, _vertices, _vertices.end());
}