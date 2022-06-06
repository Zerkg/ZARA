#include "SearchResultsBox.h"
#include "ui_SearchResultsBox.h"

#include "ASPhpQuery.h"
#include "FilterSettings.h"

#include "DisplayFilter.h"

SearchResultsBox::SearchResultsBox(QWidget *parent) : QWidget(parent) { init(); }

void SearchResultsBox::setSource(ASPhpArtistsQuery *artists, ASPhpSongsQuery *songs)
{
    m_Artists = artists;
    m_Songs   = songs  ;

    connect(artists, &ASPhpQuery::dataProcessed, this, &SearchResultsBox::displaySearchResults);
    connect(songs  , &ASPhpQuery::dataProcessed, this, &SearchResultsBox::displaySearchResults);
}
void SearchResultsBox::displaySearchResults()
{
    ASPhpGetSongsQuery *source;

    if (ui->radioButton_Artists->isChecked())
    {
        if (sender() == m_Songs)
            return;

        source = m_Artists;
    }
    else
    {
        if (sender() == m_Artists)
            return;

        source = m_Songs;
    }

    clear();

    if (!source->isSuccessfull())
    {
        ui->label_Results->setText("No results found.");
        return;
    }

    auto results(source->searchResults());

    QString resultsText(QString::number(results->size()) + " result" + (results->size() == 1 ? ":" : "s:"));

    size_t hided{};
    for (const auto &result : *results)
        if (std::find_if(m_Filters.cbegin(), m_Filters.cend(), [&result](const auto &f){ return !f.isValid(result.name); }) == m_Filters.cend())
            ui->comboBox_Results->addItem(result.name, result.songID);
        else
            ++hided;

    if (hided)
        resultsText.append("(" + QString::number(hided) + " hided)");

    ui->label_Results->setText(resultsText);
}

void SearchResultsBox::clear()
{
    ui->comboBox_Results->clear();
}

void SearchResultsBox::installDisplayFilters(const QList<DisplayFilter> &filters)
{
    m_Filters = filters;
    displaySearchResults();
}

void SearchResultsBox::init()
{
    ui = new Ui::SearchResultsBox;
    ui->setupUi(this);

    m_Artists = nullptr;
    m_Songs   = nullptr;

    FilterSettings *filterSettings(new FilterSettings(this));

    m_Filters = filterSettings->activeFilters();

    auto onRecordSelected([this](int recordPos)
    {
        if (recordPos < 0)
            return;

        auto id  (ui->comboBox_Results->itemData(recordPos).toInt());
        auto text(ui->comboBox_Results->itemText(recordPos)        );

        emit resultSelected({ id, text }, ui->radioButton_Artists->isChecked() ? ARTIST : SONG);
    });

    connect(ui->comboBox_Results, QOverload<int>::of(&QComboBox::activated), this, onRecordSelected);

    connect(ui->pushButton_OpenFilters, &QPushButton::clicked, filterSettings, &QDialog::exec);
    connect(filterSettings, &FilterSettings::filtersChanged, this, &SearchResultsBox::installDisplayFilters);

    connect(ui->radioButton_Songs  , &QRadioButton::toggled, this, &SearchResultsBox::displaySearchResults);
    connect(ui->radioButton_Artists, &QRadioButton::toggled, this, &SearchResultsBox::displaySearchResults);
}
