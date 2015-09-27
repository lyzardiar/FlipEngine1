#ifndef KNIGHT_MODEL_H
#define KNIGHT_MODEL_H

struct ModelVertex
{
	float position[3];
	float normal[3];
	float color[4];
	float uv[3];
};

class KnightModel
{
public:

    static const unsigned int numVertices;
    static const ModelVertex vertices[];

    static const unsigned int numIndices;
    static const unsigned short indices[];
};

#endif 
