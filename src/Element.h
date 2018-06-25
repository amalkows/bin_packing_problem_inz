#ifndef ELEMENT_H
#define ELEMENT_H
class Element
{
public:
	Element();
	Element(int index, int value);
	int getIndex();
	int getValue();
	void setIndex(int index);
	void setValue(int value);
private:
	int index;
	int value;
};
#endif
