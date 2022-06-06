#ifndef ASPLAYERSTATSVIEW_H
#define ASPLAYERSTATSVIEW_H

#include <QWidget>

class ASAbstractPlayerStatsModel;
class QStringListModel;

namespace Ui {
class ASPlayerStatsView;
}

class ASPlayerStatsView : public QWidget
{
Q_OBJECT
friend class ASMultistatsWidget;
public:

    explicit ASPlayerStatsView(QWidget *parent = nullptr);
    ~ASPlayerStatsView();

    void setModel(ASAbstractPlayerStatsModel *model);
    ASAbstractPlayerStatsModel *getModel();

    void setPlayersList(QStringListModel *model);

public slots:

    void updateView();

private:

    void init();

    Ui::ASPlayerStatsView *ui;
    ASAbstractPlayerStatsModel *m_PlayerStatsModel;

signals:

    void currentPlayerChanged(int);

};


#endif // ASPLAYERSTATSVIEW_H
