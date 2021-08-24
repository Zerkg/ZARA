#include "CompareModeSettings.h"
#include "ui_CompareModeSettings.h"

CompareModeSettings::CompareModeSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompareModeSettings)
{
    ui->setupUi(this);
    setWindowFlag(Qt::ToolTip);

    QObject::connect(ui->comboBox_Models, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CompareModeSettings::switchModel       );
    QObject::connect(ui->comboBox_Focus , QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int i){ if (i > -1) emit focusWidgetChanged(i); });
}

CompareModeSettings::~CompareModeSettings()
{
    delete ui;
}

int CompareModeSettings::focusView() const noexcept
{
    return ui->comboBox_Focus->currentIndex();
}

CompareModeSettings::CompareModel CompareModeSettings::compareModel() const noexcept
{
    return static_cast<CompareModel>(ui->comboBox_Models->currentIndex());
}

void CompareModeSettings::setFocusWidgetsCount(int count)
{
    std::function<void()> updater;

    if (int size(ui->comboBox_Focus->count()); count > size)
        updater = [=](){ ui->comboBox_Focus->addItem("Widget " + QString::number(ui->comboBox_Focus->count() + 1)); };
    else if (count < size)
    {
        ui->comboBox_Focus->setCurrentIndex(0);
        updater = [=](){ ui->comboBox_Focus->removeItem(ui->comboBox_Focus->count() - 1); };
    }

    while (ui->comboBox_Focus->count() != count)
        updater();

    focusEnableCheck();
}

void CompareModeSettings::focusEnableCheck()
{
    bool setEnable(ui->comboBox_Focus->count() > 2 && ui->comboBox_Models->currentIndex() == FOCUS);

    ui->label_Focus   ->setEnabled(setEnable);
    ui->comboBox_Focus->setEnabled(setEnable);
}

void CompareModeSettings::switchModel(int index)
{
    if (index != -1) emit compareModelChanged(static_cast<CompareModel>(index));
    focusEnableCheck();
}
