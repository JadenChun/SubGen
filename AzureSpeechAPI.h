#pragma once
#include "subtitleUpdater.h"
#include <RtAudio.h>
#include <speechapi_cxx.h>
#include "DatabaseAPIClass.h"

struct language {
	std::string name;
	std::string recognitionCode;
	std::string translationCode;

	language(std::string _name, std::string _recognitionCode, std::string _translationCode) {
		name = _name;
		recognitionCode = _recognitionCode;
		translationCode = _translationCode;
	}
};

using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;
using namespace Microsoft::CognitiveServices::Speech::Translation;

class AzureSpeechAPI {
private:
	static AzureSpeechAPI* instance;
	AzureSpeechAPI();
	~AzureSpeechAPI();

	std::shared_ptr<TranslationRecognizer> azureTranslator;
	std::shared_ptr<SpeechRecognizer> azureRecognizer;
	RtAudio* loopback = new RtAudio();
	RtAudio* playback = new RtAudio();
	subtitleUpdater *updater;
	bool SAVING__RESULT__STATE = false;
	int ENGINE__MODE = 0;
	int DISPOSE_FLAG;
	bool SHOW__SINGLE__LANGUAGE = true;
	DatabaseAPI* DBAPI = new DatabaseAPI();
	std::string SPEECH__SUBSCRIPTION__KEY;
	std::string SPEECH__SERVICE__REGION;
	std::string detectionLanguage = "en-us";//(exp:ja-JP,ko-KR)
	std::string translationLanguage;//(exp: en)
	std::map<std::string, language> supportedLanguage = {
		{"German",language("German","de-DE","de")},
		{"English(US)",language("English","en-US","en")},
		{"English(United Kingdom)",language("English","en-GB","en")},
		{"English(India)",language("English","en-IN","en")},
		{"Spanish(Spain)",language("Spanish","es-ES","es")},
		{"Filipino",language("Filipino","fil-PH","fil")},
		{"French",language("French","fr-FR","fr")},
		{"Japanese",language("Japanese","ja-JP","ja")},
		{"Korean",language("Korean","ko-KR","ko")},
		{"Malay",language("Malay","ms-MY","ms")},
		{"Russian",language("Russian","ru-RU","ru")},
		{"Thai",language("Thai","th-TH","th")},
		{"Mandarin(Chinese)",language("Chinese(Simplified)","zh-CN","zh-Hans")},
		{"Cantonese",language("Cantonese","zh-HK","yue")},
		{"Mandarin(Taiwanese)",language("Chinese(Traditional)","zh-TW","zh-Hant")}
	};
	void lunchRecognitionMode(auto audioCongif);
	void lunchTranslationMode(auto audioCongif);

public:
	static AzureSpeechAPI* getInstance() {
		if (!instance)
			instance = new AzureSpeechAPI;
		return instance;
	}

	void startRecognition(subtitleUpdater* updater);
	void stopRecognition();
	std::string getKey();
	std::string getRegion();
	std::string getDetectionLanguageCode();
	std::string getTranslationLanguageCode();
	bool getSavingResultState();
	std::map<std::string, language> getSupportedLanguage();
	int getEngineMode();
	bool getShowSingleLanguage();
	void setKey(std::string SPEECH__SUBSCRIPTION__KEY);
	void setRegion(std::string SPEECH__SERVICE__REGION);
	void setDetectionLanguageCode(std::string dectectionLanguage);
	void setTransaltionLanguageCode(std::string translationLanguage);
	void setSavingResultState(bool state);
	void setEngineMode(int mode);
	void setShowSingleLanguage(bool state);
};