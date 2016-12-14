#include "explorerpage.h"
#include "ui_explorerpage.h"

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

class ExplorerViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    ExplorerViewDelegate(): QAbstractItemDelegate(), unit(BitcoinUnits::BTC) { }

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
#include "explorerpage.moc"

ExplorerPage::ExplorerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerPage),
    clientModel(0),
    walletModel(0),
    explorerdelegate(new ExplorerViewDelegate())
{
    ui->setupUi(this);
    nam = new QNetworkAccessManager(this);
    ui->textBrowser->setHidden(true);
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
    connect(ui->submitButton,SIGNAL(clicked()),this,SLOT(DoHttpGet()));
}

ExplorerPage::~ExplorerPage()
{
    delete ui;
}

void ExplorerPage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
    }
}

void ExplorerPage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
}

void ExplorerPage::finished(QNetworkReply *reply) {
  ui->textBrowser->setHidden(false);
  if(reply->error() == QNetworkReply::NoError) {
    ui->textBrowser->setText(reply->readAll());
  } else {
    ui->textBrowser->setText(reply->errorString());
  }
}

void ExplorerPage::DoHttpGet() {
  QString url = "http://srv.belacoin.org/explore.php?c=l&b=";
  QString data = ui->dataLine->text();
  QString final = url + data;
  QByteArray postData;
#if QT_VERSION < 0x050000
  postData.append(data.toAscii());
#else
  postData.append(data.toLatin1());
#endif
  nam->get(QNetworkRequest(QUrl(final)));
}
