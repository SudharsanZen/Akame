#pragma once

class Input
{
private:

public:
	
	
	static bool getKeyDown(int key);
	static bool getKeyUp(int key);
	static bool getKey(int key);

	static void getMouseXY(double &x,double &y);
	static int getMouseButton(int button);

};

