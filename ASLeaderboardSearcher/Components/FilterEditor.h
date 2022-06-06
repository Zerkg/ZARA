#ifndef FILTEREDITOR_H
#define FILTEREDITOR_H

#include <QDialog>

namespace Ui {
class FilterEditor;
}

class DisplayFilter;

class FilterEditor : public QDialog
{
    Q_OBJECT

public:

    explicit FilterEditor(QWidget *parent = nullptr);

public slots:

    bool editFilter(DisplayFilter &filter);

protected:

private:

    Ui::FilterEditor *ui;

};

#endif // FILTEREDITOR_H
