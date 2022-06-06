#include "FilterSettings.h"
#include "ui_FilterSettings.h"

#include "DisplayFilter.h"
#include "FilterEditor.h"

#include <QFile>

FilterSettings::FilterSettings(QWidget *parent) : QDialog(parent) { init(); }

QList<DisplayFilter> FilterSettings::activeFilters() const noexcept
{
    return m_Filters;
}

void FilterSettings::loadFilters()
{
    clearFilters();

    QFile filtersFile("Data/DisplayFilters.dat");

    if (!filtersFile.open(QFile::ReadOnly))
        return;

    QDataStream in(&filtersFile);
    while (!in.atEnd())
    {
        QString expression;
        DisplayFilter::Mode mode;
        DisplayFilter::Type type;

        in >> expression >> mode >> type;

        addFilter({ expression, type, mode });
    }
}
void FilterSettings::saveFilters()
{
    QFile filtersFile("Data/DisplayFilters.dat");

    if (!filtersFile.open(QFile::WriteOnly))
        return;

    filtersFile.resize(0);
    QDataStream out(&filtersFile);

    foreach(const auto &filter, m_Filters)
        out << filter.expression() << filter.mode() << filter.type();
}
void FilterSettings::clearFilters()
{
    ui->treeWidget_Filters->clear();
    m_Filters.clear();
}

void FilterSettings::accept()
{
    saveFilters();
    emit filtersChanged(m_Filters);

    QDialog::accept();
}
void FilterSettings::reject()
{
    loadFilters();
    QDialog::reject();
}

void FilterSettings::init()
{
    ui = new Ui::FilterSettings;
    ui->setupUi(this);

    m_FilterEditor = new FilterEditor(this);

    auto createFilter([=]()
    {
        DisplayFilter filter;

        if (m_FilterEditor->editFilter(filter))
            addFilter(filter);
    });

    loadFilters();

    connect(ui->pushButton_Add    , &QPushButton::clicked          , this, createFilter                  );
    connect(ui->pushButton_Remove , &QPushButton::clicked          , this, &FilterSettings::deleteFilters);
    connect(ui->pushButton_Clear  , &QPushButton::clicked          , this, &FilterSettings::clearFilters );
    connect(ui->treeWidget_Filters, &QTreeWidget::itemDoubleClicked, this, &FilterSettings::editFilter   );

    QStringList filters({ "[^ -~]", "\\[as-\\w{3,}\\]", "#\\d{1,}_UNNAMED!", "Unknown (Song|Artist)" });
    foreach (auto button, ui->groupBox_FilterPresets->findChildren<QPushButton*>())
    {
        button->setProperty("filter", filters.takeLast());
        connect(button, &QPushButton::clicked, this, [this, button]{ addFilter( { button->property("filter").toString(), DisplayFilter::REG_EX, DisplayFilter::EXCLUDE } ); });
    }
}

void FilterSettings::addFilter(const DisplayFilter &filter) noexcept
{
    QString expression(filter.expression());

    if (expression.isEmpty())
        return;

    for (int i{}, filtersCount(m_Filters.size()); i < filtersCount; ++i)
        if (m_Filters[i].expression() == expression)
            return;

    m_Filters.append(filter);

    QTreeWidgetItem *item(new QTreeWidgetItem());

    item->setText(0, filter.expression());
    item->setText(1, DisplayFilter::typeToString(filter.type()));
    item->setText(2, DisplayFilter::modeToString(filter.mode()));

    ui->treeWidget_Filters->addTopLevelItem(item);

    for (int i{}, columns(ui->treeWidget_Filters->columnCount()); i < columns; ++i)
        ui->treeWidget_Filters->resizeColumnToContents(i);
}
void FilterSettings::editFilter(QTreeWidgetItem *item)
{
    DisplayFilter &filter(m_Filters[ui->treeWidget_Filters->indexOfTopLevelItem(item)]);

    if (m_FilterEditor->editFilter(filter))
    {
        item->setText(0, filter.expression());
        item->setText(1, DisplayFilter::typeToString(filter.type()));
        item->setText(2, DisplayFilter::modeToString(filter.mode()));

        for (int i{}, columns(ui->treeWidget_Filters->columnCount()); i < columns; ++i)
            ui->treeWidget_Filters->resizeColumnToContents(i);
    }
}
void FilterSettings::deleteFilters()
{
    foreach(auto item, ui->treeWidget_Filters->selectedItems())
    {
        auto row(ui->treeWidget_Filters->indexOfTopLevelItem(item));

        m_Filters.removeAt(row);
        delete item;
    }
}
