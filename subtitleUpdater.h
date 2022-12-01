#pragma once
#include <QWidget>

class subtitleUpdater : public QObject {
	Q_OBJECT

public:
	subtitleUpdater();
	~subtitleUpdater();

signals:
	void updateRecognition(QString string);
	void updateTranslation(QString strings);
	void finished();
	void error(QString err);

public slots:
	void process();
	void saveConfig(std::string key, std::string region, std::string detectionLanguage, std::string translationLanguage, bool recordState, int engineMode, bool showSingleLanguage);
	void close();
	void updateEngineMode(int engineMode);
};

