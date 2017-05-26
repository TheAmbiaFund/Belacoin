#include "poolpage.h"
#include "ui_poolpage.h"

#include "clientmodel.h"
#include "walletmodel.h"
#include "bitcoinunits.h"
#include "optionsmodel.h"
#include "guiutil.h"
#include "guiconstants.h"

#include <QAbstractItemDelegate>
#include <QPainter>

#define DECORATION_SIZE 64
#define NUM_ITEMS 3

class PoolViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    PoolViewDelegate(): QAbstractItemDelegate(), unit(BitcoinUnits::BTC)
    {

    }

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const
    {
        painter->save();

        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        QRect mainRect = option.rect;
        QRect decorationRect(mainRect.topLeft(), QSize(DECORATION_SIZE, DECORATION_SIZE));
        icon.paint(painter, decorationRect);

        QColor foreground = option.palette.color(QPalette::Text);
        painter->setPen(foreground);
        painter->setPen(foreground);
        painter->setPen(option.palette.color(QPalette::Text));

        painter->restore();
    }

    inline QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        return QSize(DECORATION_SIZE, DECORATION_SIZE);
    }

    int unit;

};
#include "poolpage.moc"

PoolPage::PoolPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PoolPage),
    clientModel(0),
    walletModel(0),
    pooldelegate(new PoolViewDelegate())
{
    ui->setupUi(this);
    ui->setupUi(this);
    nam = new QNetworkAccessManager(this);
    DoHttpGet();
    QTimer *timer = new QTimer(this);
    ui->textBrowser->setHidden(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(DoHttpGet()));
    timer->start(35000);
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
}

PoolPage::~PoolPage()
{
    delete ui;
}

void PoolPage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
    }
}

void PoolPage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
}

void PoolPage::finished(QNetworkReply *reply) {
  ui->textBrowser->setHidden(false);
  if(reply->error() == QNetworkReply::NoError) {
    ui->textBrowser->setText(reply->readAll());
  } else {
    ui->textBrowser->setText(reply->errorString());
  }
}

void PoolPage::DoHttpGet() {
  QString url = "http://srv.belacoin.org/pools.php?c=l";
  nam->get(QNetworkRequest(QUrl(url)));
}
