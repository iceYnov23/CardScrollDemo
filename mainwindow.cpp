#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cardwidget.h"
#include "flowlayout.h"

#include <QScrollBar>
#include <QApplication>
#include <QEvent>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initUIs();
    this->initConnects();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUIs()
{
    // 给滚动区域的内容容器装 FlowLayout
    FlowLayout* flow = new FlowLayout(ui->scrollAreaWidgetContents, 12, 12, 12);
    ui->scrollAreaWidgetContents->setLayout(flow);
    ui->scrollAreaWidgetContents->setStyleSheet("background-color:#f4f5f7;");

    // 关键：监听容器自身的 resize 事件，宽度变化时立即重排
    ui->scrollAreaWidgetContents->installEventFilter(this);

    for (int i = 0; i < 6; ++i) addCard();
    relayoutCardsForCurrentWidth();
}

void MainWindow::initConnects()
{
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::onAddCardClicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
}

void MainWindow::onAddCardClicked()
{
    addCard();
    relayoutCardsForCurrentWidth();
}

void MainWindow::onClearClicked()
{
    FlowLayout* flow = static_cast<FlowLayout*>(ui->scrollAreaWidgetContents->layout());
    if (flow) {
        while (QLayoutItem* item = flow->takeAt(0)) {
            if (QWidget* w = item->widget()) w->deleteLater();
            delete item;
        }
    }
    m_cards.clear();
    m_cardCounter = 0;

    // 清空后让容器收回最小尺寸，避免残留空白
    ui->scrollAreaWidgetContents->adjustSize();
    ui->scrollAreaWidgetContents->layout()->activate();
}

void MainWindow::addCard()
{
    ++m_cardCounter;
    CardWidget* card = new CardWidget(
        m_cardCounter,
        QString(str("卡片 %1")).arg(m_cardCounter),
        str("自定义卡片样式示例：宽高比 1:1.2，"
        "横向流式排布，窗口缩放时整块等比跟随。"),
        ui->scrollAreaWidgetContents);

    static_cast<FlowLayout*>(ui->scrollAreaWidgetContents->layout())->addWidget(card);
    m_cards.append(card);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    relayoutCardsForCurrentWidth();
}

bool MainWindow::eventFilter(QObject* obj, QEvent* e)
{
    if (obj == ui->scrollAreaWidgetContents && e->type() == QEvent::Resize) {
        relayoutCardsForCurrentWidth();
    }
    return QMainWindow::eventFilter(obj, e);
}

// ======================= 关键修改 =======================
// 迭代收敛：滚动条出现/消失会改变视口宽度，从而改变列数，
// 单次计算只能到达"中间态"，导致临界点错位。
// 这里最多迭代 5 次，直到 cardW 稳定为止。
void MainWindow::relayoutCardsForCurrentWidth()
{
    if (m_cards.isEmpty()) return;

    const int minCardW = 160;
    const int maxCardW = 420;
    const int hSpacing = 12;
    const int flowMargin = 12;

    int cardW = -1;

    for (int iter = 0; iter < 5; ++iter) {
        int avail = ui->scrollArea->viewport()->width() - flowMargin * 2;
        if (avail <= 0) return;

        int cols = qMax(1, (avail + hSpacing) / (minCardW + hSpacing));
        int newW = (avail - (cols - 1) * hSpacing) / cols;
        if (newW > maxCardW) newW = maxCardW;
        newW = qMax(minCardW, newW);

        // 收敛判断：宽度变化小于 2px 即认为稳定
        if (cardW >= 0 && qAbs(newW - cardW) < 2) {
            cardW = newW;
            break;
        }
        cardW = newW;

        for (CardWidget* c : m_cards)
            c->setLogicalWidth(cardW);

        // 强制结算一次布局，让滚动条状态刷新
        ui->scrollAreaWidgetContents->layout()->activate();
        // 把队列中挂起的 resize/layout 事件 flush 掉，才能读到最新视口宽度
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    // 最终再同步一次，防止收敛退出时还差最后一次更新
    for (CardWidget* c : m_cards)
        c->setLogicalWidth(cardW);

    ui->scrollAreaWidgetContents->layout()->activate();
}
// =======================================================