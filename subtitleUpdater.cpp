#pragma once
#include "stdafx.h"
#include "subtitleUpdater.h"
#include "AzureSpeechAPI.h"

subtitleUpdater::subtitleUpdater() {}

subtitleUpdater::~subtitleUpdater() {}

void subtitleUpdater::process() {
	AzureSpeechAPI *speech = AzureSpeechAPI::getInstance();
	speech->startRecognition(this);
}

void subtitleUpdater::saveConfig(std::string key, std::string region, std::string detectionLanguage, std::string translationLanguage, bool recordState, int engineMode, bool showSingleLanguage) {
	AzureSpeechAPI* speech = AzureSpeechAPI::getInstance();
	speech->setKey(key);
	speech->setRegion(region);
	speech->setDetectionLanguageCode(detectionLanguage);
	speech->setTransaltionLanguageCode(translationLanguage);
	speech->setSavingResultState(recordState);
	speech->setEngineMode(engineMode);
	speech->setShowSingleLanguage(showSingleLanguage);
}

void subtitleUpdater::close() {
	AzureSpeechAPI* speech = AzureSpeechAPI::getInstance();
	speech->stopRecognition();
	emit finished();
}

void subtitleUpdater::updateEngineMode(int engineMode)
{
	AzureSpeechAPI* speech = AzureSpeechAPI::getInstance();
	speech->setEngineMode(engineMode);
}
