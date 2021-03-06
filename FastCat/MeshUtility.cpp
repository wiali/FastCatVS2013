#include "MeshUtility.h"
#include "CommonInclude.h"

#include <maya/MGlobal.h>


int Vertex::numTrackableOutgoingEdges()
{
	Edge *cur = edge;
	int count = 0;

	do
	{
		++count;
		cur = cur->vNext();
	} while (cur != edge);

	return count;
}


void Vertex::getOneRingIndices(int firstVertexOffset, std::vector<int> &indices)
{
	Edge *cur = edge;
	Edge *start = edge;

	if (valence < 0) // boundary
	{
		while (cur->dual)
		{
			cur = cur->vNext();
		}

		start = cur;
	}

	do
	{
		assert(cur->leftFace && cur->leftFace->valence == 4);
		indices.push_back(firstVertexOffset + cur->dest->idx);
		indices.push_back(firstVertexOffset + cur->fNext()->dest->idx);

		cur = cur->vNext();
	} while (cur != start);

	if (valence < 0)
	{
		indices.push_back(firstVertexOffset + cur->vPrev()->fPrev()->origin->idx);
	}
}


int Vertex::getOneRingMeta(std::vector<int> &neighbours, int *numCreases, float *averageSharpness, std::vector<int> &creases)
{
	if (valence < -2) // boundary
	{
		neighbours.clear();
		*numCreases = 0;
		*averageSharpness = 0.f;
		findBoundaryIndices(creases);
		return valence;
	}

	if (valence == -2) // corner
	{
		neighbours.clear();
		*numCreases = 0;
		*averageSharpness = 0.f;
		creases.push_back(-1); creases.push_back(-1);
		return valence;
	}

	Edge *cur = edge;
	float totalSharpness = 0.f;
	float count = 0; // numerb of creases
	do
	{
		neighbours.push_back(cur->dest->idx);

		if (cur->leftFace->newIdx < 0)
		{
			neighbours.clear();
			*numCreases = 0;
			*averageSharpness = 0.f;
			creases.clear();
			creases.push_back(-1); creases.push_back(-1);
			return -2; // use corner rule
		}
		neighbours.push_back(cur->leftFace->newIdx);

		if (cur->sharpness > 0.f)
		{
			creases.push_back(cur->dest->idx);
			totalSharpness += cur->sharpness;
			++count;
		}

		cur = cur->vNext();
	} while (cur != edge);

	*numCreases = count;
	*averageSharpness = totalSharpness / count;

	assert(neighbours.size() == 2 * valence);
	
	if (count != 2)
	{
		creases.clear();
		creases.push_back(-1); creases.push_back(-1);
	}
	if (count < 2)
	{
		*averageSharpness = 0.f;
	}

	return valence;
}


void Vertex::findBoundaryIndices(std::vector<int> &bis)
{
	if (valence >= -2)
	{
		return; // not boundary
	}

	Edge *e = edge;
	do
	{
		if (!e->dual)
		{
			break;
		}
		e = e->vNext();
	} while (e != edge);

	assert(e->dual == NULL && e->origin == this);
	bis.push_back(e->dest->idx);
	e = e->vPrev()->fPrev();
	assert(e->dest == this);
	bis.push_back(e->origin->idx);
}


bool Vertex::isUnevaluable()
{
	if (valence < 0)
	{
		return false; // ignore boundaries
	}
	if (valence != 4)
	{
		return true; // crease with extraordinary vertex cannot be evaluated directly
	}

	Edge *cur = edge;
	int numCreases = 0, index = 0;
	int firstAt, secondAt;
	float lastSharpness = -1.f;

	do
	{
		if (cur->sharpness > 0.f)
		{
			if (lastSharpness < 0.f)
			{
				lastSharpness = cur->sharpness;
			}
			else if (fabsf(lastSharpness - cur->sharpness) > FLT_EPSILON)
			{
				return true; // different sharpness on the vertex
			}
			if (numCreases == 0)
			{
				firstAt = index;
			}
			if (numCreases == 1)
			{
				secondAt = index;
			}
			++numCreases;
		}

		++index;
		cur = cur->vNext();
	} while (cur != edge);

	if (numCreases != 2)
	{
		return true;
	}
	if (secondAt - firstAt != 2)
	{
		return true; // bent crease cannot be evaluated directly
	}

	return false;
}


int Face::numBoundaryVertices()
{
	Edge *cur = right;
	int count = 0;

	do
	{
		if (cur->origin->valence < 0)
		{
			++count;
		}
		cur = cur->fNext();
	} while (cur != right);

	return count;
}


int Face::numCreases()
{
	Edge *cur = right;
	int count = 0;

	do
	{
		if (cur->sharpness > 0.f || !cur->dual)
		{
			++count;
		}
		cur = cur->fNext();
	} while (cur != right);

	return count;
}


bool Face::hasNonregularBoundary()
{
	Edge *cur = right;

	do
	{
		if (cur->origin->valence == -2 || cur->origin->valence < -3)
		{
			return true;
		}
		cur = cur->fNext();
	} while (cur != right);

	return false;
}


bool Face::hasTriangleHead()
{
	Edge *cur = right;

	do
	{
		if (cur->isTriangleHead)
		{
			return true;
		}
		cur = cur->fNext();
	} while (cur != right);

	return false;
}


void Face::getPartialPatchInfo(int *p_numTriHeands, bool *p_connected) const
{
	Edge *cur = right;
	int &numTriHeads = *p_numTriHeands;
	bool &connected = *p_connected;
	int triHeadIdx0, triHeadIdx1, idx;

	// assert doesn't work in Maya plugin
	// COMMENT THIS OUT IN RELEASE BUILD
	if (valence != 4)
	{
		MGlobal::displayError("Transition patch is not quad");
		return;
	}

	numTriHeads = 0;
	connected = false;
	idx = 0;

	do
	{
		if (cur->isTriangleHead)
		{
			if (numTriHeads == 0)
			{
				triHeadIdx0 = idx;
			}
			if (numTriHeads == 1)
			{
				triHeadIdx1 = idx;
			}
			++numTriHeads;
		}

		++idx;
		cur = cur->fNext();
	} while (cur != right);

	if (numTriHeads == 2 &&
		(triHeadIdx1 - triHeadIdx0 == 1 || triHeadIdx1 - triHeadIdx0 == valence - 1))
	{
		connected = true;
	}
}


void Face::getVertices(std::set<Vertex *> &vs)
{
	Edge *cur = right;
	do
	{
		vs.insert(cur->origin);
		cur = cur->fNext();
	} while (cur != right);
}


void Face::getEdges(std::set<Edge *> &es)
{
	Edge *cur = right;
	do
	{
		es.insert(cur);
		cur = cur->fNext();
	} while (cur != right);
}


void Face::getVertices(std::vector<Vertex *> &vs)
{
	Edge *cur = right;
	do
	{
		vs.push_back(cur->origin);
		cur = cur->fNext();
	} while (cur != right);
}


void Face::getEdges(std::vector<Edge *> &es)
{
	Edge *cur = right;
	do
	{
		es.push_back(cur);
		cur = cur->fNext();
	} while (cur != right);
}


void Face::getOneRingNeighbourFaces(std::set<Face *> &neighbours)
{
	Edge *cur = right;
	do
	{
		Edge *e = cur->origin->edge;

		do
		{
			neighbours.insert(e->leftFace);
			if (e->rightFace)
			{
				neighbours.insert(e->rightFace);
			}

			e = e->vNext();
		} while (e != cur->origin->edge);

		cur = cur->fNext();
	} while (cur != right);
}


void Face::getOneRingIndices(int firstVertexOffset, unsigned *outIndices)
{
	assert(valence == 4);
	const int numIndices = 16;
	Edge *cur = right;
	std::vector<unsigned> indexCache;
	indexCache.reserve(numIndices);
	int reorderMap[numIndices] = { 6, 7, 2, 3, 10, 14, 11, 15, 9, 8, 13, 12, 5, 1, 4, 0 };

	do
	{
		assert(cur->origin->valence == 4);

		indexCache.push_back(cur->origin->idx + firstVertexOffset);

		if (cur->origin->valence < 0) // boundary
		{
			if (cur->sharpness < 0.f)
			{
				indexCache.push_back(indexCache.back());
				indexCache.push_back(cur->vNext()->fPrev()->origin->idx + firstVertexOffset);
				indexCache.push_back(indexCache.back());
			}
			else
			{
				indexCache.push_back(cur->vPrev()->dest->idx + firstVertexOffset);
				indexCache.push_back(cur->origin->idx + firstVertexOffset);
				indexCache.push_back(cur->vPrev()->dest->idx + firstVertexOffset);
			}
		}
		else
		{
			indexCache.push_back(cur->vPrev()->dest->idx + firstVertexOffset);
			indexCache.push_back(cur->vPrev()->vPrev()->dest->idx + firstVertexOffset);
			indexCache.push_back(cur->vPrev()->vPrev()->fNext()->dest->idx + firstVertexOffset);
		}

		cur = cur->fNext();
	} while (cur != right);

	for (int i = 0; i < numIndices; ++i)
	{
		outIndices[reorderMap[i]] = indexCache[i];
	}
}