#ifndef VIDEOQUALITYJUDGEMENT_BACKEND_H
#define VIDEOQUALITYJUDGEMENT_BACKEND_H
#include <QObject>
#include <QString>

using namespace std;

class backend: public QObject {
    Q_OBJECT
public slots:
    void getFilePath(QString q_file_path);
    void compute(QString file_size, QString duration);
signals:
    void returnResult(QString result, QString code_ratio);
    void returnFileParams(QString file_size, QString duration);
};

#endif
