#ifndef COMPAREMODESETTINGS_H
#define COMPAREMODESETTINGS_H

#include <QWidget>

namespace Ui {
class CompareModeSettings;
}

class CompareModeSettings : public QWidget
{
    Q_OBJECT

public:

    enum CompareModel
    {
        NONE,
        FOCUS
    };

    explicit CompareModeSettings(QWidget *parent = nullptr);
    ~CompareModeSettings();

    int focusView() const noexcept;
    CompareModel compareModel() const noexcept;

public slots:

    void setFocusWidgetsCount(int count);

private:

    Ui::CompareModeSettings *ui;
    void focusEnableCheck();

private slots:

    void switchModel(int index);

signals:

    void compareModelChanged(CompareModel);
    void focusWidgetChanged (int index);

};

#endif // COMPAREMODESETTINGS_H
