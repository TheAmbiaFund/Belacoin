#include "voucherpage.h"
#include "ui_voucherpage.h"

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

class VoucherViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    VoucherViewDelegate(): QAbstractItemDelegate(), unit(BitcoinUnits::BTC) { }

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
#include "voucherpage.moc"

VoucherPage::VoucherPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VoucherPage),
    clientModel(0),
    walletModel(0),
    voucherdelegate(new VoucherViewDelegate())
{
    ui->setupUi(this);
    nam = new QNetworkAccessManager(this);
    ui->textBrowser->setHidden(true);
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
    connect(ui->submitButton,SIGNAL(clicked()),this,SLOT(DoHttpGet()));
}

VoucherPage::~VoucherPage()
{
    delete ui;
}

void VoucherPage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
    }
}

void VoucherPage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
}

void VoucherPage::finished(QNetworkReply *reply) {
  ui->textBrowser->setHidden(false);
  if(reply->error() == QNetworkReply::NoError) {
    ui->textBrowser->setText(reply->readAll());
  } else {
    ui->textBrowser->setText(reply->errorString());
  }
}

void VoucherPage::DoHttpGet() {
  QString url = "http://srv.bellacoin.com/voucher.php?c=l&w=";
  QString uwallet = ui->dataLine->text();
  QString vouchercode = ui->voucherCode->text();
  QString final = url + uwallet + "&vc=" + vouchercode;
  QByteArray postData;
  postData.append(uwallet.toAscii());
  postData.append(vouchercode.toAscii());
  nam->get(QNetworkRequest(QUrl(final)));
}
