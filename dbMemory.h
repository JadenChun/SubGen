#pragma once
#include <vector>
#include <string>

class dbMemory {
private:
	static dbMemory* instance;

public:
	static dbMemory* getInstance() {
		if (!instance)
			instance = new dbMemory();
		return instance;
	}

	int new_id;
	std::vector<std::vector<std::string>> queryResult;
	void clearData();
};