#include "FilterEditor.h"
#include "ui_FilterEditor.h"

#include "DisplayFilter.h"

FilterEditor::FilterEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterEditor)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

bool FilterEditor::editFilter(DisplayFilter &filter)
{
    ui->lineEdit_Input->setText        (filter.expression());
    ui->comboBox_Mode ->setCurrentIndex(filter.mode      ());
    ui->comboBox_Type ->setCurrentIndex(filter.type      ());

    if (exec() == QDialog::Accepted)
    {
        QString input(ui->lineEdit_Input->text());

        if (input.isEmpty())
            return false;

        filter.setExpression(input);
        filter.setMode(static_cast<DisplayFilter::Mode>(ui->comboBox_Mode->currentIndex()));
        filter.setType(static_cast<DisplayFilter::Type>(ui->comboBox_Type->currentIndex()));

        return true;
    }
    return false;
}
