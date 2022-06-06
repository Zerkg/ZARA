#ifndef MODULEMANUAL_H
#define MODULEMANUAL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ModuleManual; }
QT_END_NAMESPACE

namespace zara
{
class ModuleManual : public QWidget
{
    Q_OBJECT

public:

    ModuleManual(QWidget *parent = nullptr);

    void setSourcePath(const QUrl &source);
    QUrl sourcePath() const noexcept;

private:

    void init();

    Ui::ModuleManual *ui;

};
}
#endif // MODULEMANUAL_H
