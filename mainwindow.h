#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

#define str(x) QString::fromLocal8Bit(x)

namespace Ui
{
    class MainWindow; 
}
class CardWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void initUIs();
    void initConnects();

protected:
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* e) override;   // ÐÂÔö

private slots:
    void onAddCardClicked();
    void onClearClicked();

private:
    void addCard();
    void relayoutCardsForCurrentWidth();

    Ui::MainWindow* ui;
    int m_cardCounter = 0;
    QList<CardWidget*> m_cards;
};

#endif // MAINWINDOW_H