#include "stdafx.h"
#include "subtitlegui.h"
#include <QtWidgets/QApplication>
#include "DatabaseAPIClass.h"
#include "AzureSpeechAPI.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DatabaseAPI *dbAPI = new DatabaseAPI();
    dbAPI->createTable();
    dbAPI->read_engine_state();
    dbMemory *queryMemory = dbMemory::getInstance();
    if (queryMemory->queryResult.size() > 0) {
        std::string key = queryMemory->queryResult.at(0).at(1);
        std::string region = queryMemory->queryResult.at(0).at(2);
        std::string fromLanguage = queryMemory->queryResult.at(0).at(3);
        std::string toLanguage = queryMemory->queryResult.at(0).at(4);
        bool recordState = (queryMemory->queryResult.at(0).at(5) == "0") ? false : true;
        int engineMode = stoi(queryMemory->queryResult.at(0).at(6));
        bool showSingleLanguage = (queryMemory->queryResult.at(0).at(7) == "0") ? false : true;
        AzureSpeechAPI* speechEngine = AzureSpeechAPI::getInstance();
        speechEngine->setKey(key);
        speechEngine->setRegion(region);
        speechEngine->setDetectionLanguageCode(fromLanguage);
        speechEngine->setTransaltionLanguageCode(toLanguage);
        speechEngine->setSavingResultState(recordState);
        speechEngine->setEngineMode(engineMode);
        speechEngine->setShowSingleLanguage(showSingleLanguage);
    }
    SubtitleGUI w;
    w.show();
    return a.exec();
}
