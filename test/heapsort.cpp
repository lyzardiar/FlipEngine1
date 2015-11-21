#include <stdio.h>
#include <iostream>

struct vec2 
{
	int x;
	int y;
};
class A
{
public:
	vec2 pos;
	A() { pos.x = 2; pos.y = 2; }

	vec2& getposition()
	{
		return pos;
	}
};

int main()
{
	A a;
	vec2& position = a.getposition();
	position.x = 3;

	printf("%d", a.pos.x);
	
	system("pause");
}