#pragma once
#include "stdafx.h"
#include "AzureSpeechAPI.h"
#include "GetDefaultOutputDeviceID.cpp"
#include "RtAudioCallbackFunction.h"

AzureSpeechAPI* AzureSpeechAPI::instance = 0;

AzureSpeechAPI::AzureSpeechAPI(){
    this->updater = nullptr;
}
AzureSpeechAPI::~AzureSpeechAPI(){}

void AzureSpeechAPI::stopRecognition() {
    if (DISPOSE_FLAG == 0) {
        azureRecognizer->StopContinuousRecognitionAsync().get();
    }else {
        azureTranslator->StopContinuousRecognitionAsync().get();
    }
}

void AzureSpeechAPI::startRecognition(subtitleUpdater* updater)
{
    this->updater = updater;
    //find virtual mic that monitor the speaker output
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    LPWSTR routing = ListEndpoints();
    std::wstring ws(routing);
    std::string virtualMic = std::string(ws.begin(), ws.end());
    auto audioConfig = AudioConfig::FromMicrophoneInput(virtualMic);
    
    if (ENGINE__MODE==0) {
        lunchRecognitionMode(audioConfig);
    }
    else {
        lunchTranslationMode(audioConfig);
    }
    DISPOSE_FLAG = ENGINE__MODE;

    std::promise<void> recognitionEnd;

    //stream speaker output with RtAudio
    if (!loopback->isStreamOpen() && !playback->isStreamOpen()) {
        RtAudio::StreamParameters iParams, oParams;
        iParams.deviceId = loopback->getDefaultOutputDevice();
        iParams.nChannels = 2;

        unsigned int devices = playback->getDeviceCount();
        RtAudio::DeviceInfo info;
        unsigned int speaker = 0;
        for (unsigned int i = 0; i < devices; i++) {
            info = playback->getDeviceInfo(i);
            std::size_t found = info.name.find("CABLE Input");
            if (found != std::string::npos) {
                speaker = i;
            }
        }
        oParams.deviceId = speaker;
        oParams.nChannels = 2;
        unsigned int sampleRate = 44100;
        unsigned int bufferFrames = 100;

        loopback->openStream(nullptr, &iParams, RTAUDIO_FLOAT32, sampleRate, &bufferFrames, &readSpeakerLoopback);
        playback->openStream(&oParams, nullptr, RTAUDIO_FLOAT32, sampleRate, &bufferFrames, &playSpeakerSound);

        loopback->startStream();
        playback->startStream();
    }
    recognitionEnd.get_future().get();
}

void AzureSpeechAPI::lunchRecognitionMode(auto audioConfig) {
    auto recognitionConfig = SpeechConfig::FromSubscription(SPEECH__SUBSCRIPTION__KEY, SPEECH__SERVICE__REGION);
    recognitionConfig->SetSpeechRecognitionLanguage(detectionLanguage);
    azureRecognizer = SpeechRecognizer::FromConfig(recognitionConfig, audioConfig);

    azureRecognizer->Recognizing.Connect([&](const SpeechRecognitionEventArgs& e)
    {
        updater->updateRecognition(QString::fromStdString(e.Result->Text));
    });
    if (SAVING__RESULT__STATE) {
        azureRecognizer->Recognized.Connect([&](const SpeechRecognitionEventArgs& e)
        {
            std::string recognitionLanguage;
            std::string recognizedText = e.Result->Text;
            if (recognizedText == "")return;
            for (auto& index : supportedLanguage) {
                if (index.second.recognitionCode == detectionLanguage) {
                    recognitionLanguage = index.second.name;
                }
            }
            DBAPI->save_recognition_data(time(0), recognitionLanguage, recognizedText);
        });
    }
    azureRecognizer->StartContinuousRecognitionAsync().get();
}

void AzureSpeechAPI::lunchTranslationMode(auto audioConfig) {
    //setup azure connection
    auto translationConfig = SpeechTranslationConfig::FromSubscription(SPEECH__SUBSCRIPTION__KEY, SPEECH__SERVICE__REGION);
    translationConfig->SetSpeechRecognitionLanguage(detectionLanguage);
    auto toLanguages = { translationLanguage };
    for (auto language : toLanguages) {
        translationConfig->AddTargetLanguage(language);
    }
    azureTranslator = TranslationRecognizer::FromConfig(translationConfig, audioConfig);

    //prepare callback
    if (SHOW__SINGLE__LANGUAGE) {
        azureTranslator->Recognizing.Connect([&](const TranslationRecognitionEventArgs& e)
        {
            for (auto pair : e.Result->Translations)
            {
                auto language = pair.first;
                auto translation = pair.second;
                updater->updateRecognition(QString::fromStdString(translation));
            }
        });
    }
    else {
        azureTranslator->Recognizing.Connect([&](const TranslationRecognitionEventArgs& e)
            {
                updater->updateRecognition(QString::fromStdString(e.Result->Text));
                for (auto pair : e.Result->Translations)
                {
                    auto language = pair.first;
                    auto translation = pair.second;
                    updater->updateTranslation(QString::fromStdString(translation));
                }
            });
    }
    if (SAVING__RESULT__STATE) {
        azureTranslator->Recognized.Connect([&](const TranslationRecognitionEventArgs& e) {
            std::string fromLanguage;
            std::string recognizedText = e.Result->Text;
            std::string toLanguage;
            std::string translatedText;
            for (auto pair : e.Result->Translations)
            {
                toLanguage = pair.first;
                translatedText = pair.second;
            }
            if (recognizedText == translatedText)return;
            for (auto& index : supportedLanguage) {
                if (index.second.translationCode == toLanguage) {
                    toLanguage = index.second.name;
                }
                if (index.second.recognitionCode == detectionLanguage) {
                    fromLanguage = index.second.name;
                }
            }
            DBAPI->save_translation_data(time(0), fromLanguage, toLanguage, recognizedText, translatedText);
        });
    }
    azureTranslator->StartContinuousRecognitionAsync().get();
}

std::string AzureSpeechAPI::getKey()
{
    return SPEECH__SUBSCRIPTION__KEY;
}

std::string AzureSpeechAPI::getRegion()
{
    return SPEECH__SERVICE__REGION;
}

std::string AzureSpeechAPI::getDetectionLanguageCode()
{
    return detectionLanguage;
}

std::string AzureSpeechAPI::getTranslationLanguageCode()
{
    return translationLanguage;
}

bool AzureSpeechAPI::getSavingResultState()
{
    return this->SAVING__RESULT__STATE;
}

void AzureSpeechAPI::setKey(std::string SPEECH__SUBSCRIPTION__KEY)
{
    this->SPEECH__SUBSCRIPTION__KEY = SPEECH__SUBSCRIPTION__KEY;
}

void AzureSpeechAPI::setRegion(std::string SPEECH__SERVICE__REGION)
{
    this->SPEECH__SERVICE__REGION = SPEECH__SERVICE__REGION;
}

void AzureSpeechAPI::setDetectionLanguageCode(std::string detectionLanguage)
{
    this->detectionLanguage = detectionLanguage;
}

void AzureSpeechAPI::setTransaltionLanguageCode(std::string translationLanguage)
{
    this->translationLanguage = translationLanguage;
}

void AzureSpeechAPI::setSavingResultState(bool state)
{
    this->SAVING__RESULT__STATE = state;
}

int AzureSpeechAPI::getEngineMode()
{
    return ENGINE__MODE;
}

bool AzureSpeechAPI::getShowSingleLanguage()
{
    return SHOW__SINGLE__LANGUAGE;
}

void AzureSpeechAPI::setShowSingleLanguage(bool state)
{
    this->SHOW__SINGLE__LANGUAGE = state;
}

void AzureSpeechAPI::setEngineMode(int mode) 
{
    this->ENGINE__MODE = mode;
}

std::map<std::string, language> AzureSpeechAPI::getSupportedLanguage()
{
    return supportedLanguage;
}

