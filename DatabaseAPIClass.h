#pragma once
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include "dbMemory.h"
#include <vector>

class DatabaseAPI {
private:
	sqlite3* DB;
	const char* dbName = "database/SubGen.db";
	int connect();

public:
	void createTable();
    void save_recognition_data(time_t timestamp, std::string language, std::string recognition);
    void read_recognition_data();
    void delete_recognition_data(std::vector<int> IDs);
	void save_translation_data(time_t timestamp, std::string fromLanguage, std::string toLanguage , std::string recognition, std::string translation);
	void read_translation_data();
	void delete_translation_data(std::vector<int> IDs);
    void read_engine_state();
    void save_engine_state(std::string key,std::string region,std::string fromLanguage,std::string toLanguage,std::string recordState, std::string engineMode, std::string displaySingleLanguage);
};

static int read_id_callback(void* notUsed, int argc, char** argv, char** azColName) {
    int new_id = 1;
    if (argv[0]) {
        new_id = std::stoi(argv[0])+1;
    }
    dbMemory *memo = dbMemory::getInstance();
    memo->new_id = new_id;
    return 0;
}

static int query_callback(void* notUsed, int argc, char** argv, char** azColName) {
    int i;
    std::vector<std::string> row;
    for (i = 0; i < argc; i++) {
        row.push_back(argv[i]);
    }
    dbMemory* memo = dbMemory::getInstance();
    memo->queryResult.push_back(row);
    return 0;
}