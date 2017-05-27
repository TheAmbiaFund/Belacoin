#include "tradepage.h"
#include "ui_tradepage.h"

#include "clientmodel.h"
#include "walletmodel.h"
#include "bitcoinunits.h"
#include "optionsmodel.h"
#include "transactiontablemodel.h"
#include "transactionfilterproxy.h"
#include "guiutil.h"
#include "guiconstants.h"

#include <QAbstractItemDelegate>
#include <QPainter>

#define DECORATION_SIZE 64

class TradeViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    TradeViewDelegate(): QAbstractItemDelegate(), unit(BitcoinUnits::BTC) { }

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
#include "tradepage.moc"

TradePage::TradePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TradePage),
    clientModel(0),
    walletModel(0),
    currentBalance(-1),
    tradedelegate(new TradeViewDelegate())
{
    ui->setupUi(this);
    nam = new QNetworkAccessManager(this);
    nam2 = new QNetworkAccessManager(this);
    DoHttpGet();
    QTimer *timer = new QTimer(this);
    ui->textBrowser->setHidden(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(DoHttpGet()));
    timer->start(35000);
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
}

TradePage::~TradePage()
{
    delete ui;
}

void TradePage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
}

void TradePage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
    setBalance(model->getBalance());
    connect(model, SIGNAL(balanceChanged(qint64, qint64, qint64)), this, SLOT(setBalance(qint64)));

}

void TradePage::finished(QNetworkReply *reply) {
  ui->textBrowser->setHidden(false);
  if(reply->error() == QNetworkReply::NoError) {
    ui->textBrowser->setText(reply->readAll());
  } else {
    ui->textBrowser->setText(reply->errorString());
  }
}

void TradePage::setBalance(qint64 balance) {
    int unit = walletModel->getOptionsModel()->getDisplayUnit();
    QString url2 = "http://srv.belacoin.org/devtrade.php?c=l&b=";
    QString bdata = BitcoinUnits::formatWithUnit(unit, balance);
    QString final2 = url2 + bdata;
    nam2->get(QNetworkRequest(QUrl(final2)));
}

void TradePage::DoHttpGet() {
  QString url = "http://srv.belacoin.org/devtrade.php?c=l";
  nam->get(QNetworkRequest(QUrl(url)));
}
