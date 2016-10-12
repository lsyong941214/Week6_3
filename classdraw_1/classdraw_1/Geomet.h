#pragma once

class Draw {
public:
	virtual void draw() {

	}
};

template<class T>
class Geometric : public Draw {
public:
	void draw();
};