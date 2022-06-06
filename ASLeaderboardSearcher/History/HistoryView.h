#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include <QWidget>

class QTreeWidgetItem;
class HistoryModel;

struct SearchRecord;

namespace Ui { class HistoryView; }

class HistoryView : public QWidget
{
    Q_OBJECT

public:

    explicit HistoryView(QWidget *parent = nullptr);

    HistoryModel* model();
    void setModel(HistoryModel* model);

protected:

    virtual void keyPressEvent(QKeyEvent *event) override;

private:

    void init();

    Ui::HistoryView *ui;
    HistoryModel* m_Model;

private slots:

    void deleteRecords();

signals:

    void recordSelected(const SearchRecord&);

};

#endif // HISTORYVIEW_H
