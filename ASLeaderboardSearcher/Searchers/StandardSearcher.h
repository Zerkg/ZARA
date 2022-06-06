#ifndef STANDARDSEARCHER_H
#define STANDARDSEARCHER_H

#include "DataTypes.h"

#include <QWidget>

namespace Ui { class StandardSearcher; }

struct SearchData;

class ASPhpArtistsQuery;
class ASPhpSongsQuery;

class QCompleter;

class StandardSearcher : public QWidget
{
Q_OBJECT
public:

    explicit StandardSearcher(QWidget *parent = nullptr);
    ~StandardSearcher();    

    void setCompleter(QCompleter *c);

private:

    int parseUrl(const QString& url) const;

    void dispatchUrlId   (int id             );
    void dispatchResultId(const SearchData& data, int type);

    void init();

    Ui::StandardSearcher *ui;

    ASPhpSongsQuery  * m_SongsSearcher  ;
    ASPhpArtistsQuery* m_ArtistsSearcher;

private slots:

    void search();

signals:

    void resultSelected(const SearchRecord&);

};

#endif // STANDARDSEARCHER_H
