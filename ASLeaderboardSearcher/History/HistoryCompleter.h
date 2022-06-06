#ifndef HISTORYCOMPLETER_H
#define HISTORYCOMPLETER_H

#include <QObject>

class QCompleter;
class HistoryModel;
struct SearchRecord;

class CompleterManager : public QObject
{
    Q_OBJECT
public:

    explicit CompleterManager(QObject *parent = nullptr);

    void setHistoryModel(const HistoryModel *model);
    QCompleter *completer() const;

private:

    QCompleter *m_Completer;

private slots:

    void tryAdd(const SearchRecord& record);

};

#endif // HISTORYCOMPLETER_H
