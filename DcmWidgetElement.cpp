#include "DcmWidgetElement.h"

DcmWidgetElement::DcmWidgetElement(const QString &itemTag, const QString &itemVR, const QString &itemVM, const QString &itemLength, const QString &itemDescription, const QString &itemValue)
{
	this->itemTag = itemTag;
	this->itemVR = itemVR;
	this->itemVM = itemVM;
	this->itemLength = itemLength;
	this->itemDescription = itemDescription;
	this->itemValue = itemValue;
}

//========================================================================================================================
QString DcmWidgetElement::getItemTag() const
{
	return this->itemTag;
}

//========================================================================================================================
QString DcmWidgetElement::getItemVR() const
{
	return this->itemVR;
}

//========================================================================================================================
QString DcmWidgetElement::getItemVM() const
{
	return this->itemVM;
}

//========================================================================================================================
QString DcmWidgetElement::getItemLength() const
{
	return this->itemLength;
}

//========================================================================================================================
QString DcmWidgetElement::getItemDescription() const
{
	return this->itemDescription;
}

//========================================================================================================================
QString DcmWidgetElement::getItemValue() const
{
	return this->itemValue;
}

//========================================================================================================================
int DcmWidgetElement::getTableIndex() const
{
	return this->tableIndex;
}

//========================================================================================================================
std::string DcmWidgetElement::toString() const
{
	return std::string(this->itemTag.toStdString() + " " + this->itemVM.toStdString() + " " + this->itemVR.toStdString() + " "
		+ this->itemLength.toStdString() + " " + this->itemDescription.toStdString() + " " + this->itemValue.toStdString());
}

//========================================================================================================================
int DcmWidgetElement::getDepth() const
{
	return this->depth;
}

//========================================================================================================================
void DcmWidgetElement::calculateTableIndex(const int & current, const std::vector<DcmWidgetElement>& elements)
{
	int count = 0;

	for (auto element : elements)
	{
		if (*this == element)
			count++;

		if (count == current)
		{
			this->setTableIndex(element.getTableIndex());
			break;
		}
	}
}

//========================================================================================================================
void DcmWidgetElement::setDepth(const int & depth)
{
	this->depth = depth;
}

//========================================================================================================================
void DcmWidgetElement::setTableIndex(const int & index)
{
	this->tableIndex = index;
}

//========================================================================================================================
void DcmWidgetElement::incrementDepth()
{
	this->depth++;
}

//========================================================================================================================
void DcmWidgetElement::setItemTag(const QString & final)
{
	this->itemTag = final;
}

//========================================================================================================================
bool DcmWidgetElement::checkIfContains(const QString& str) const
{
	return this->itemTag.toUpper().contains(str.toUpper()) || this->itemVM.toUpper().contains(str.toUpper()) ||
		this->itemVR.toUpper().contains(str.toUpper()) || this->itemLength.toUpper().contains(str.toUpper()) ||
		this->itemDescription.toUpper().contains(str.toUpper()) || this->itemValue.toUpper().contains(str.toUpper());
}

//========================================================================================================================
DcmTagKey DcmWidgetElement::extractTagKey() const
{
	QString str = this->getItemTag().toUpper();
	str.replace(" ", "");
	str.replace("(", "");
	str.replace(")", "");
	QStringList list = str.split(",");

	return DcmTagKey(hexToDecimal(list[0].toStdString().c_str()), hexToDecimal(list[1].toStdString().c_str()));
}

//========================================================================================================================
void DcmWidgetElement::calculateDepthFromTag()
{
	QString str = this->getItemTag();
	int numberOfSpaces = 0;

	for (auto& i : str)
	{
		if (i == ' ')
		{
			numberOfSpaces++;
		}
	}

	this->setDepth(numberOfSpaces / 2);
}

void DcmWidgetElement::setVR(const QString& str)
{
	this->itemVR = str;
}

void DcmWidgetElement::setValue(const QString& str)
{
	this->itemValue = str;
}

//========================================================================================================================
int DcmWidgetElement::hexToDecimal(const char * hex)
{
	const int length = strlen(hex);
	int base = 1;
	int result = 0;

	for (int i = length - 1; i >= 0; i--)
	{
		if (hex[i] >= '0' && hex[i] <= '9')
		{
			result += (hex[i] - 48) * base;
			base *= 16;
		}

		else if (hex[i] >= 'A' && hex[i] <= 'F')
		{
			result += (hex[i] - 55) * base;
			base *= 16;
		}
	}

	return result;
}

//========================================================================================================================
bool DcmWidgetElement::operator==(DcmWidgetElement & element) const
{
	return this->getItemTag().replace(" ", "") == element.getItemTag().replace(" ", "") &&
		this->getItemVM() == element.getItemVM() &&
		this->getItemVR() == element.getItemVR() &&
		this->getItemLength() == element.getItemLength() &&
		this->getItemValue() == element.getItemValue();
}

//========================================================================================================================
bool DcmWidgetElement::operator>(DcmWidgetElement & element) const
{
	return this->extractTagKey() > element.extractTagKey();
}

//========================================================================================================================
bool DcmWidgetElement::operator<(DcmWidgetElement & element) const
{
	return this->extractTagKey() < element.extractTagKey();
}

//========================================================================================================================
int DcmWidgetElement::compareTagKey(DcmWidgetElement & element) const
{
	if (this->extractTagKey() == element.extractTagKey())
	{
		return 1;
	}

	else if (this->extractTagKey() > element.extractTagKey())
	{
		return 2;
	}

	else
	{
		return 3;
	}
}
