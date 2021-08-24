#ifndef ANALYZERCORE_H
#define ANALYZERCORE_H

#include <QObject>

#include <QHash>

class AnalyzerCore : public QObject
{
    Q_OBJECT

public:

    enum AnalyzeType
    {
        BONUSES
    };
    struct AnalyzeData
    {
        double boost;
        double missed;

    };

    AnalyzerCore();

protected:

    virtual void analyzeBonuses();

    QHash<AnalyzeType, AnalyzeData> m_Analyze;

signals:

    void analyzeReady();

};

class PointmanAnalyzer : public AnalyzerCore
{

};
class EraserAnalyzer : public AnalyzerCore
{

};
class PusherAnalyzer : public AnalyzerCore
{

};
class DoubleVisionAnalyzer : public AnalyzerCore
{

};
class MonoAnalyzer : public AnalyzerCore
{

};
class VegasAnalyzer : public AnalyzerCore
{

};

#endif // ANALYZERCORE_H
