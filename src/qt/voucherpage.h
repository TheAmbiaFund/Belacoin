#ifndef VOUCHERPAGE_H
#define VOUCHERPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QTimer>

namespace Ui {
    class VoucherPage;
}
class ClientModel;
class WalletModel;
class VoucherViewDelegate;

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** voucher page widget */
class VoucherPage : public QWidget
{
    Q_OBJECT

public:
    explicit VoucherPage(QWidget *parent = 0);
    ~VoucherPage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);

public slots:

signals:

private:
    Ui::VoucherPage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;

    VoucherViewDelegate *voucherdelegate;
    QNetworkAccessManager *nam;

private slots:
    void finished(QNetworkReply *reply);
    void DoHttpGet();
};

#endif // VOUCHERPAGE_H
