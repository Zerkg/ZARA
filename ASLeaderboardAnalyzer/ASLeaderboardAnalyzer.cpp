#include "ASLeaderboardAnalyzer.h"
#include "ui_ASLeaderboardAnalyzer.h"

#include "ASLeaderboard.h"
#include "ASPlayerStatsController.h"
#include "CompareModeSettings.h"

//#include <QDebug>

ASLeaderboardAnalyzer::ASLeaderboardAnalyzer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ASLeaderboardAnalyzer)
{
    ui->setupUi(this);
    init();
}
ASLeaderboardAnalyzer::~ASLeaderboardAnalyzer() { delete ui; }

void ASLeaderboardAnalyzer::timerEvent(QTimerEvent *event)
{
    killTimer(event->timerId());
    adjustSize();
}

void ASLeaderboardAnalyzer::moveEvent(QMoveEvent *)
{
    QPoint coordinates(QWidget::mapToGlobal(QPoint(width() +1, 80)));
    m_CompareSettings->move(coordinates);
}
void ASLeaderboardAnalyzer::resizeEvent(QResizeEvent *)
{
    QPoint coordinates(QWidget::mapToGlobal(QPoint(width() +1, 80)));
    m_CompareSettings->move(coordinates);
}

void ASLeaderboardAnalyzer::init()
{       
    m_CompareSettings = new CompareModeSettings(this);
    m_CompareSettings->hide();

    DataController *controller(new DataController(this));
    controller->setLeaderboardSelector(ui->widget_Selector);
    ui->widget_GuiController->setDataController(controller);


    auto compareMenuController([=]
    {
        static QPixmap arrowLeft (":/Data/Images/DoubleArrowLeft.png" );
        static QPixmap arrowRight(":/Data/Images/DoubleArrowRight.png");

        m_CompareSettings->setVisible(!m_CompareSettings->isVisible());
        arrowLeft.swap(arrowRight);

        ui->pushButton_ComparingSettings->setIcon(arrowRight);
    });

    auto updatePlayersList([=]()
    {
        int size(ui->widget_Selector->leaderboard()->size());

        if (size < 7)
            ui->spinBox_WidgetsCount->setMaximum(size);
        else
            ui->spinBox_WidgetsCount->setMaximum(6);
    });

    QObject::connect(ui->pushButton_ComparingSettings, &QPushButton::clicked, this, compareMenuController);

    QObject::connect(ui->widget_Searcher     , &ASLeaderboardSearcher::songSelected       , ui->widget_Selector     , &ASLeaderboardSelector::loadLeaderboard                                           );
    QObject::connect(ui->widget_Selector     , &ASLeaderboardSelector::modeChanged        , this                    , updatePlayersList);

    QObject::connect(ui->spinBox_WidgetsCount, QOverload<int>::of(&QSpinBox::valueChanged), ui->widget_GuiController, &GuiController        ::setViewsCount  );

    QObject::connect(ui->widget_GuiController, &GuiController       ::viewsCountChanged   , m_CompareSettings       , &CompareModeSettings::setFocusWidgetsCount);
    QObject::connect(ui->widget_GuiController, &GuiController       ::viewsCountChanged   , this       , [=](){ startTimer(100); });

    QObject::connect(m_CompareSettings, &CompareModeSettings::compareModelChanged, controller, &DataController::setCompareModel);
    QObject::connect(m_CompareSettings, &CompareModeSettings::focusWidgetChanged , controller, &DataController::setFocusModel  );
}



























