#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QLabel>

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(int index,
        const QString& title,
        const QString& desc,
        QWidget* parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void setLogicalWidth(int w);

    static constexpr double AspectRatio = 1.2; // ¸ß / ¿í = 1.2

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QLabel* m_titleLabel;
    QLabel* m_descLabel;
    int     m_logicalWidth = 240;
};

#endif // CARDWIDGET_H