#ifndef ASLEADERBOARDSELECTOR_H
#define ASLEADERBOARDSELECTOR_H

#include "ASScoreSelector_global.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ASScoreSelector; }
QT_END_NAMESPACE

struct PlayerStats;

class QTreeWidget    ;
class QTreeWidgetItem;

class ASPhpLeaderboardQuery;

class ASSCORESELECTOR_EXPORT ASScoreSelector : public QWidget
{
    Q_OBJECT

public:

    explicit ASScoreSelector(QWidget *parent = nullptr) noexcept;
    ~ASScoreSelector() noexcept;

    void setSelectionLimit(quint8 limit) noexcept;

    const ASPhpLeaderboardQuery* source() const noexcept;

public slots:

    void loadLeaderboard(int id) noexcept;
    void reset() noexcept;

protected:

    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private:

    Ui::ASScoreSelector *ui;

    int m_LeaderboardId;
    quint8 m_SelectedItemsCount;
    quint8 m_MaxSelectedItemsCount;


    ASPhpLeaderboardQuery *m_LeaderboardQuery;
    QVector<QTreeWidget*> m_Leaderboards;

private slots:

    void onItemClick(QTreeWidgetItem *item);
    void processQueryData() noexcept;

signals:

    void leaderboardLoaded(int);

    void scoreSelected  (const PlayerStats&);
    void scoreDeselected(const PlayerStats&);

};
#endif // ASLEADERBOARDSELECTOR_H
