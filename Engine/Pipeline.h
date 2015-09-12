#pragma once
class Pipeline
{
public:
	Pipeline();
	virtual ~Pipeline();

	virtual void init() = 0;
	virtual void drawScene() = 0;
};

