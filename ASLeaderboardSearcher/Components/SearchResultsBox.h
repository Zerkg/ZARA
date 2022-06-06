#ifndef SEARCHRESULTSBOX_H
#define SEARCHRESULTSBOX_H

#include <QWidget>

#include "DisplayFilter.h"

namespace Ui { class SearchResultsBox; }

class ASPhpArtistsQuery;
class ASPhpSongsQuery;

struct SearchData;

class SearchResultsBox : public QWidget
{
    Q_OBJECT

public:

    enum ResultType
    {
        SONG,
        ARTIST
    };

    explicit SearchResultsBox(QWidget *parent = nullptr);

    void setSource(ASPhpArtistsQuery *artists, ASPhpSongsQuery *songs);

public slots:

    void displaySearchResults();
    void clear();
    void installDisplayFilters(const QList<DisplayFilter> &filters);

private:

    void init();

    Ui::SearchResultsBox *ui;

    ASPhpArtistsQuery *m_Artists;
    ASPhpSongsQuery   *m_Songs  ;

    QList<DisplayFilter> m_Filters;

signals:

    void resultSelected(const SearchData&, ResultType);

};

#endif // SEARCHRESULTSBOX_H
