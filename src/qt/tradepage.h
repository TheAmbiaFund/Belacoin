#ifndef TRADEPAGE_H
#define TRADEPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QTimer>

namespace Ui {
    class TradePage;
}
class ClientModel;
class WalletModel;
class TradeViewDelegate;
class TransactionFilterProxy;

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Trade ("home") page widget */
class TradePage : public QWidget
{
    Q_OBJECT

public:
    explicit TradePage(QWidget *parent = 0);
    ~TradePage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);

public slots:

signals:

private:
    Ui::TradePage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;
    TradeViewDelegate *tradedelegate;
    QNetworkAccessManager *nam;

private slots:
    void finished(QNetworkReply *reply);
    void DoHttpGet();
};

#endif // TRADEPAGE_H
