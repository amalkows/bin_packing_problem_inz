#include "Element.h"

Element::Element()
{

}

Element::Element(int indexIn, int valueIn)
{
	index = indexIn;
	value = valueIn;
}

int Element::getIndex()
{
	return index;
}
int Element::getValue()
{
	return value;
}
void Element::setIndex(int indexIn)
{
	index = indexIn;
}
void Element::setValue(int valueIn)
{
	value = valueIn;
}