#include "ModuleManual.h"
#include "ui_ModuleManual.h"

#include <QKeyEvent>

using namespace zara;

ModuleManual::ModuleManual(QWidget *parent) : QWidget(parent) { init(); }

void ModuleManual::setSourcePath(const QUrl &source)
{
    ui->textBrowser_Manual->setSource(source);
}
QUrl ModuleManual::sourcePath() const noexcept
{
    return ui->textBrowser_Manual->source();
}

void ModuleManual::init()
{
    ui = new Ui::ModuleManual;
    ui->setupUi(this);
}

