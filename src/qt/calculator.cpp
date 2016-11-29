#include "calculator.h"
#include "ui_calculator.h"

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

class CalculatorViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    CalculatorViewDelegate(): QAbstractItemDelegate(), unit(BitcoinUnits::BTC) { }

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
#include "calculator.moc"

CalculatorPage::CalculatorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalculatorPage),
    clientModel(0),
    walletModel(0),
    calculatordelegate(new CalculatorViewDelegate())
{
    ui->setupUi(this);
    nam = new QNetworkAccessManager(this);
    ui->textBrowser->setHidden(true);
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
    connect(ui->submitButton,SIGNAL(clicked()),this,SLOT(DoHttpGet()));
}

CalculatorPage::~CalculatorPage()
{
    delete ui;
}

void CalculatorPage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
    }
}

void CalculatorPage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
}

void CalculatorPage::finished(QNetworkReply *reply) {
  ui->textBrowser->setHidden(false);
  if(reply->error() == QNetworkReply::NoError) {
    ui->textBrowser->setText(reply->readAll());
  } else {
    ui->textBrowser->setText(reply->errorString());
  }
}

void CalculatorPage::DoHttpGet() {
  QString url = "http://srv.bellacoin.com/calculator.php?c=l&h=";
  QString hashrate = ui->hashrate->text();
  QString final = url + hashrate;
  QByteArray postData;
#if QT_VERSION < 0x050000
  postData.append(hashrate.toAscii());
#else
  postData.append(hashrate.toLatin1());
#endif
  nam->get(QNetworkRequest(QUrl(final)));
}
