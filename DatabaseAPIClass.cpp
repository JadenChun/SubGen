#include "stdafx.h"
#include "DatabaseAPIClass.h"
#include <iostream>

int DatabaseAPI::connect() {
    int exit = 0;
    exit = sqlite3_open(dbName, &DB);
    if (exit) {
        qDebug() << "Error open DB " << sqlite3_errmsg(DB);
    }
    return exit;
}

void DatabaseAPI::createTable()
{
    if (this->connect())return;
    std::string create_translation_table_sql = "CREATE TABLE IF NOT EXISTS translation_record (" \
        "id INTEGER PRIMARY KEY NOT NULL," \
        "timestamp INTEGER NOT NULL," \
        "from_language TEXT NOT NULL," \
        "to_language TEXT NOT NULL," \
        "recognition_result TEXT NOT NULL," \
        "translation_result TEXT NOT NULL);";
    sqlite3_exec(DB, create_translation_table_sql.c_str(), 0, 0, 0);

    std::string create_recognition_table_sql = "CREATE TABLE IF NOT EXISTS recognition_record (" \
        "id INTEGER PRIMARY KEY NOT NULL," \
        "timestamp INTEGER NOT NULL," \
        "language TEXT NOT NULL," \
        "recognition_result TEXT NOT NULL);";
    sqlite3_exec(DB, create_recognition_table_sql.c_str(), 0, 0, 0);
    std::string create_engine_state_table_sql = "CREATE TABLE IF NOT EXISTS speech_enigne_state ("\
        "id INTEGER PRIMARY KEY NOT NULL,"
        "key TEXT NOT NULL,"\
        "region TEXT NOT NULL,"\
        "fromLanguage TEXT NOT NULL,"\
        "toLanguage TEXT,"\
        "recordState INTEGER NOT NULL,"\
        "engineMode INTEGER NOT NULL,"\
        "displaySingleLanguage INTEGER);";
    sqlite3_exec(DB, create_engine_state_table_sql.c_str(), 0, 0, 0);
    sqlite3_close(DB);
}

void DatabaseAPI::save_recognition_data(time_t timestamp,std::string language, std::string recognition)
{
    if (this->connect())return;
    std::string max_id_query = "SELECT max(id) FROM recognition_record";
    char* messaggeError;
    int exit = 0;
    dbMemory* queryData = dbMemory::getInstance();
    queryData->clearData();
    exit = sqlite3_exec(DB, max_id_query.c_str(), read_id_callback, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        qDebug() << "Error select Table :" << messaggeError;
    }
    std::string insert_translation_sql = "INSERT INTO recognition_record VALUES("\
        + std::to_string(queryData->new_id) + ","\
        + std::to_string(timestamp) + ","\
        + "'" + language + "',"\
        + "'" + recognition + "');";
    exit = sqlite3_exec(DB, insert_translation_sql.c_str(), 0, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        qDebug() << "failled! " << messaggeError;
    }
    sqlite3_close(DB);
}

void DatabaseAPI::read_recognition_data()
{
    if (this->connect())return;
    std::string query = "SELECT * FROM recognition_record";
    char* messaggeError;
    int exit = 0;
    dbMemory* queryData = dbMemory::getInstance();
    queryData->clearData();
    sqlite3_exec(DB, query.c_str(), query_callback, 0, &messaggeError);
    sqlite3_close(DB);
}

void DatabaseAPI::delete_recognition_data(std::vector<int> IDs)
{
    if (this->connect())return;
    for (int i = 0; i < IDs.size(); i++) {
        std::string delete_sql = "DELETE from recognition_record where ID=" + std::to_string(IDs.at(i)) + ";";
        sqlite3_exec(DB, delete_sql.c_str(), 0, 0, 0);
    }
    sqlite3_close(DB);
}

void DatabaseAPI::save_translation_data(time_t timestamp, std::string fromLanguage, std::string toLanguage, std::string recognition, std::string translation)
{
    if (this->connect())return;
    std::string max_id_query = "SELECT max(id) FROM translation_record";
    char* messaggeError;
    int exit = 0;
    dbMemory *queryData = dbMemory::getInstance();
    queryData->clearData();
    exit = sqlite3_exec(DB, max_id_query.c_str(), read_id_callback, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        qDebug()<< "Error select Table :"<< messaggeError;
    }
    std::string insert_translation_sql= "INSERT INTO translation_record VALUES("\
        + std::to_string(queryData->new_id) +","\
        + std::to_string(timestamp)+","\
        +"'"+ fromLanguage+ "',"\
        + "'" + toLanguage + "',"\
        + "'" + recognition + "',"\
        + "'" + translation + "');";
    exit = sqlite3_exec(DB, insert_translation_sql.c_str(), 0, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        qDebug()<< "failled! "<< messaggeError;
    }
    sqlite3_close(DB);
}

void DatabaseAPI::read_translation_data()
{
    if (this->connect())return;
    std::string query = "SELECT * FROM translation_record";
    char* messaggeError;
    int exit = 0;
    dbMemory* queryData = dbMemory::getInstance();
    queryData->clearData();
    sqlite3_exec(DB, query.c_str(), query_callback, 0, &messaggeError);
    sqlite3_close(DB);
}

void DatabaseAPI::delete_translation_data(std::vector<int> IDs) {
    if (this->connect())return;
    for (int i = 0; i < IDs.size(); i++) {
        std::string delete_sql = "DELETE from translation_record where ID=" + std::to_string(IDs.at(i)) + ";";
        sqlite3_exec(DB, delete_sql.c_str(), 0, 0, 0);
    }
    sqlite3_close(DB);
}

void DatabaseAPI::read_engine_state() {
    if (this->connect())return;
    std::string query = "SELECT * FROM speech_enigne_state";
    char* messaggeError;
    int exit = 0;
    dbMemory* queryData = dbMemory::getInstance();
    queryData->clearData();
    sqlite3_exec(DB, query.c_str(), query_callback, 0, &messaggeError);
    sqlite3_close(DB);
}

void DatabaseAPI::save_engine_state(std::string key, std::string region, std::string fromLanguage, std::string toLanguage, std::string recordState, std::string engineMode, std::string displaySingleLanguage)
{
    if (this->connect())return;
    std::string check_table_sql = "SELECT count(*)from speech_enigne_state;";
    dbMemory* memo = dbMemory::getInstance();
    memo->clearData();
    sqlite3_exec(DB, check_table_sql.c_str(), query_callback, 0, 0);
    if (memo->queryResult.at(0).at(0)=="0") {
        std::string insert_sql = "INSERT INTO speech_enigne_state VALUES(1,'" + key + "','" + region + "','" + fromLanguage + "','" + toLanguage + "','" + recordState + "','"+ engineMode + "','" + displaySingleLanguage +"');";
        char* messaggeError;
        int exit = sqlite3_exec(DB, insert_sql.c_str(), 0, 0, &messaggeError);
        if (exit != SQLITE_OK) {
            qDebug() << "Error insert Table:" << messaggeError;
        }
    }
    else {
        std::string update_sql = "UPDATE speech_enigne_state set key='" + key + "',"\
            "region='" + region + "',"\
            "fromLanguage='" + fromLanguage + "',"\
            "toLanguage='" + toLanguage + "',"\
            "recordState='" + recordState + "',"\
            "engineMode='" + engineMode + "',"\
            "displaySingleLanguage='" + displaySingleLanguage + "';";
        char* messaggeError;
        int exit = sqlite3_exec(DB, update_sql.c_str(), 0, 0, &messaggeError);
        if (exit != SQLITE_OK) {
            qDebug()<< "Error update Table:" <<messaggeError;
        }
    }
    sqlite3_close(DB);
}