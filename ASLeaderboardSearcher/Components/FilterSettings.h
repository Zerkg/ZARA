#ifndef FILTERSETTINGS_H
#define FILTERSETTINGS_H

#include "DisplayFilter.h"

#include <QDialog>

class FilterEditor   ;
class QTreeWidgetItem;

namespace Ui { class FilterSettings; }

class FilterSettings : public QDialog
{
    Q_OBJECT

public:

    explicit FilterSettings(QWidget *parent = nullptr);
    QList<DisplayFilter> activeFilters() const noexcept;

public slots:

    void loadFilters ();
    void saveFilters ();
    void clearFilters();

protected:

    virtual void accept() override;
    virtual void reject() override;

private:

    void init();

    Ui::FilterSettings *ui;
    FilterEditor *m_FilterEditor;
    QList<DisplayFilter> m_Filters;

private slots:

    void addFilter(const DisplayFilter& filter) noexcept;
    void editFilter(QTreeWidgetItem *item);
    void deleteFilters();

signals:

    void filtersChanged(const QList<DisplayFilter> &);

};

#endif // FILTERSETTINGS_H
