#ifndef ASSCOREANALYZER_H
#define ASSCOREANALYZER_H

#include "ASScoreAnalyzer_global.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ASScoreAnalyzer; }
QT_END_NAMESPACE


class ASSCOREANALYZER_LIBRARY_EXPORT ASScoreAnalyzer : public QMainWindow
{
    Q_OBJECT

public:

    ASScoreAnalyzer(QWidget *parent = nullptr);
    ~ASScoreAnalyzer();

private:

    Ui::ASScoreAnalyzer *ui;

    void init();

private slots:

    void displayLeaderboard();
    void displayAnalyze();

};
#endif // ASSCOREANALYZER_H
