#pragma once
class Component
{
protected:
	struct Dimention
	{
		int xDim = 0;
		int yDim = 0;
	};

public:
	virtual Dimention getDim() = 0;
};

