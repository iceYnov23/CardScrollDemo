#include "cardwidget.h"
#include <QVBoxLayout>
#include <QPainter>

CardWidget::CardWidget(int index, const QString& title,
    const QString& desc, QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // 关键：不再调用 setMinimumSize，避免与 minimumSizeHint 冲突

    m_titleLabel = new QLabel(QString("[%1] %2").arg(index).arg(title), this);
    QFont f = m_titleLabel->font();
    f.setBold(true);
    f.setPointSize(11);
    m_titleLabel->setFont(f);

    m_descLabel = new QLabel(desc, this);
    m_descLabel->setWordWrap(true);
    m_descLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->setContentsMargins(14, 12, 14, 12);
    lay->setSpacing(8);
    lay->addWidget(m_titleLabel);
    lay->addWidget(m_descLabel, 1);
}

QSize CardWidget::sizeHint() const
{
    return QSize(m_logicalWidth,
        qRound(m_logicalWidth * AspectRatio));
}

QSize CardWidget::minimumSizeHint() const
{
    // 关键：最小尺寸也用同一比例，避免与 sizeHint 跨度太大导致布局误判
    return QSize(80, qRound(80 * AspectRatio));
}

void CardWidget::setLogicalWidth(int w)
{
    w = qMax(80, w);
    if (w == m_logicalWidth) return;
    m_logicalWidth = w;
    updateGeometry();
    update();
}

void CardWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect r = rect().adjusted(0, 0, -1, -1);
    p.setPen(QPen(QColor(220, 220, 220), 1));
    p.setBrush(Qt::white);
    p.drawRoundedRect(r, 8, 8);
}