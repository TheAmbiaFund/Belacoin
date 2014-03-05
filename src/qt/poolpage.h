#ifndef POOLPAGE_H
#define POOLPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QTimer>

namespace Ui {
    class PoolPage;
}
class ClientModel;
class WalletModel;
class PoolViewDelegate;

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Pool page widget */
class PoolPage : public QWidget
{
    Q_OBJECT

public:
    explicit PoolPage(QWidget *parent = 0);
    ~PoolPage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);

public slots:

signals:

private:
    Ui::PoolPage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;

    PoolViewDelegate *pooldelegate;
    QNetworkAccessManager *nam;

private slots:
    void finished(QNetworkReply *reply);
    void DoHttpGet();
};

#endif // POOLPAGE_H
