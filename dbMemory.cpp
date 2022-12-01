#include "dbMemory.h"

dbMemory* dbMemory::instance = 0;

void dbMemory::clearData()
{
	this->queryResult = {};
}
